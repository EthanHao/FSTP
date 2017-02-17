/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ConnectionLister.cpp
 * Author: ethan
 * 
 * Created on January 30, 2017, 5:29 PM
 */

#include "../include/ConnectionListener.h"
#include "../include/CommonMacro.h"
#include "SocketInfo.h"
#include <sys/types.h> /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
namespace CTCPSERVER {

    ConnectionListener::ConnectionListener(const std::string& nsIP, int nPort,int nMaxBacklogSize, std::shared_ptr<IDataCenterInterface> & npDataCenter) 
    throw(SocketExceptionCreateFailed&,
            SocketExceptionSetOptionFailed&,
            SocketExceptionBindFailed&,
            SocketExceptionListenFailed&,
            EpollExceptionCreateFailed&,
            EpollExceptionCtlFailed&,
            std::bad_alloc&,
            ThreadExceptionCreateFailed&):
             msIP(nsIP),
             mnPort(nPort),
             mnMaxBacklogSize(nMaxBacklogSize),
             mpEpollObject(nullptr),
             mpEpollEvents(nullptr),
             mpDataCenter(npDataCenter){
        
        //Create a socket file descriptor binding to the ip ,address,
        //Create a EpollObject with the maxBackupsize
        
        //TCP Stream IPV4 Socket
        mnListenFileDescriptor = ::socket(AF_INET, SOCK_STREAM, SOCK_NONBLOCK);
        if(FAILED(mnListenFileDescriptor))
            throw SocketExceptionCreateFailed(errno);
        
        //Set this socket descriptor SO_REUSEADDR ,means we can bind this (ip,port) inaddr_any imediatly 
        //after closing this address, because usually we cannot open it for some time after closing.
        //this flag may cause this TCP server unreliable , care must be taken.
        SocketInfo::SetReuseAddress(mnListenFileDescriptor);
        
        //Bind to the local ip and port
        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = ::htons(mnPort);
        addr.sin_addr.s_addr = inet_addr(msIP.c_str());
        if( FAILED(::bind(mnListenFileDescriptor, 
                 reinterpret_cast<struct sockaddr *>(&addr),
                  sizeof(struct sockaddr))) )
            throw new SocketExceptionBindFailed(errno);
        
        //Listen
        if( FAILED(::listen(mnListenFileDescriptor, nMaxBacklogSize)) )
             throw SocketExceptionListenFailed(errno);
        
        //Create EPoll Object
        //Add this file descriptor to this epoll object
        std::unique_ptr<EPollObject> lpEpoll(new EPollObject(mnMaxBacklogSize));
        mpEpollObject = std::move(lpEpoll); //take the ownership

        struct epoll_event ev;
        ev.events = EPOLLIN; /*default mode is level triggered mode*/
        ev.data.ptr = &mnListenFileDescriptor;
        mpEpollObject->AddFileDescriptor(mnListenFileDescriptor, ev);
        
        //allocate memory for the event array
        std::unique_ptr<struct epoll_event[]> lp(new epoll_event[mnMaxBacklogSize],std::default_delete<struct epoll_event[]>());
        mpEpollEvents = std::move(lp);
        
        //Run this thread
        Run();
    }

    ConnectionListener::~ConnectionListener() {
        //Stop And Wait
        StopAndWait();
    }
    
    void ConnectionListener::TheadCallback(){
        
        while(mbRunning && mpEpollObject && mpEpollEvents)
        {
            try{
            //Wait
            int nfds =  mpEpollObject->Wait(&mpEpollEvents[0],mnMaxBacklogSize);
            if(nfds == -1)
            {
                //Log the error and 
                mbRunning = false;
                break;
            }
            
            //accept this client one by one
            for(int i = 0; i < nfds ; i++)
                Accept(&mpEpollEvents[i]);
            }
            catch(SocketExceptionAcceptFailed& e){
                //Log the reason
            }
            catch(SocketExceptionSetOptionFailed& e){
                //Log the reason
            }
            catch(EpollExceptionWaitFailed& e){
                //Exit this thread,
                //Log the reason to exit the thread
            }
            catch(std::exception& e){
                //Log the reason
            }
        }
    }
    void ConnectionListener::Accept(epoll_event * npEvent) throw(SocketExceptionAcceptFailed&,SocketExceptionSetOptionFailed&){
        //accept
        //set nonblock io 
        if(npEvent == nullptr)
            return;
        
        //accept
        struct sockaddr_in lAddressIn;
        unsigned int lnAddressLength = sizeof(struct sockaddr_in);
        int lnNewFileDescriptor = ::accept(mnListenFileDescriptor, reinterpret_cast<struct sockaddr *>(&lAddressIn),&lnAddressLength);
        if(lnNewFileDescriptor)
            throw SocketExceptionAcceptFailed(errno);
        //set NON_BLOCKIO, Linger,NoDelay
        SocketInfo::SetLinger(lnNewFileDescriptor);
        SocketInfo::SetNoDelay(lnNewFileDescriptor);
        SocketInfo::SetNonBlock(lnNewFileDescriptor);
        
        //add to dataDealerCenter to handle reading and writing on this socket
        if(mpDataCenter)
            mpDataCenter->DispatchSocket(lnNewFileDescriptor);
    }
    eErrorCode ConnectionListener::Run() throw(ThreadExceptionCreateFailed&)
    {
        if(mbRunning || mThread.joinable())
            return eErrorCode::eIsRunning;
        mThread = std::thread(&ConnectionListener::TheadCallback,this);
        if(mThread.joinable())
        {
            mbRunning = true;
            return eErrorCode::eSuccess;
        }
        else throw ThreadExceptionCreateFailed(errno);
    }
}
