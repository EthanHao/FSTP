/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DataDealer.cpp
 * Author: ethan
 * 
 * Created on February 14, 2017, 7:36 PM
 */

#include "../include/DataDealer.h"
#include <netinet/in.h>
#include <arpa/inet.h>
namespace CTCPSERVER {

    DataDealer::DataDealer(int nNum, const ServerInfo& nBackendServer) throw (EpollExceptionCreateFailed&,
            std::bad_alloc&,
            ThreadExceptionCreateFailed&,
            SocketExceptionCreateFailed&,
            SocketExceptionP2NWrongFormat&,
            SocketExceptionP2NFailed&,
            SocketExceptionConnectFailed&,
            SocketExceptionSetOptionFailed&) :
    mnMaxNumOfSocket(nNum+1),
    mBackendServer(nBackendServer) {

        mBackendSocket = -1;
        std::unique_ptr<EPollObject> lpEpoll(new EPollObject(mnMaxNumOfSocket));
        mpEpollObject = std::move(lpEpoll);

        //allocate memory for the event array
        std::unique_ptr<struct epoll_event[] > lpEvents(new epoll_event[mnMaxNumOfSocket], std::default_delete<struct epoll_event[]>());
        mpEpollEvents = std::move(lpEvents);

        //Connect to the backend Server
        if (!mBackendServer.IsEnd()) {
            //we got the backend server, we must connect to backend server via socket. and after connecting,
            //should add this socket to the epoll
            Connect();
        }

        //Run
        mbRunning = false;
        Run();
    }

    DataDealer::~DataDealer() {
        //Wait the termination of the thread
        StopAndWait();
    }
    //Connect to the backend Server

    eErrorCode DataDealer::Connect() throw(SocketExceptionCreateFailed&,
            SocketExceptionP2NWrongFormat&,
            SocketExceptionP2NFailed&,
            SocketExceptionConnectFailed&,
            SocketExceptionSetOptionFailed&) {
       

        //Construct the serverAdd_in 
        struct sockaddr_in servaddr;
        bzero(&servaddr, sizeof (struct sockaddr_in));
        servaddr.sin_family = PF_INET;
        servaddr.sin_port = htons(mBackendServer.GetPort());
        int lnRet = inet_pton(AF_INET, mBackendServer.GetName().c_str(), &servaddr.sin_addr);
        if (lnRet == 0)
            throw SocketExceptionP2NWrongFormat(errno);
        if (lnRet < 0)
            throw SocketExceptionP2NFailed(errno);

         //Create a IPV4 TCP Socket
        mBackendSocket = ::socket(AF_INET, SOCK_STREAM, SOCK_NONBLOCK);
        if (FAILED(mBackendSocket))
            throw SocketExceptionCreateFailed(errno);
        
        //Connect
        lnRet = ::connect(mBackendSocket, (struct sockaddr *) &servaddr, sizeof (servaddr));
        if (FAILED(mBackendSocket))
            throw SocketExceptionConnectFailed(mBackendSocket);
        
        //Set NonBlocking IO
        SocketInfo::SetNonBlock(mBackendSocket);
        
        //Add to epoll ,using level_triggered and firstly monitorring EPOLL_OUT event
        epoll_event lEvent;
        lEvent.events = EPOLLOUT;
        lEvent.data.ptr = nullptr;
        mpEpollObject->AddFileDescriptor(mBackendSocket,lEvent);
        
    }


    //Thread call back function

    void DataDealer::TheadCallback() {

        while (mbRunning && mpEpollObject && mpEpollEvents) {
            try {
                //Wait
                int nfds = mpEpollObject->Wait(&mpEpollEvents[0], mnMaxNumOfSocket);
                //accept this client one by one
                for (int i = 0; i < nfds; i++) {
                    try {
                        DataReadAndWritting(&mpEpollEvents[i]);
                    } catch (std::exception& e) {
                        //Log the reason
                        //Continue to work
                    }
                }
            } catch (EpollExceptionWaitFailed& e) {

                //If it is system call interrupt then continue
                if (e.GetErrorNo() == EINTR)
                    continue;

                //Else Exit this thread,
                //Log the reason to exit the thread
                mbRunning = false;
                break;
            } catch (std::exception& e) {
                //Log the reason
                //Continue
            }
        }
    }

    //Data things
    eErrorCode DataDealer::DataReadAndWritting(const struct epoll_event& npEvent){
        if(!mBackendServer.IsEnd()) {
            if(npEvent.data.fd == mBackendSocket){
                if(npEvent.events & EPOLLIN) {
                    //Get response from backend server, we should send this response out to the specific client
                }else if(npEvent.events & EPOLLOUT) {
                    //Send the packet to the backend server
                }else {
                    throw EpollExceptionEventFailed(errno,mpEpollObject->GetEpollID(),npEvent.data.fd);
                }
            }
        } else {
            if( npEvent.data.fd < 0 ) 
                throw SocketExceptionInvalidDescriptor(errno,npEvent.data.fd );
            //deal with the normal data about the clients
            if (npEvent.events & EPOLLIN) {
                 //Read the packet from the client
            } else if (npEvent.events & EPOLLOUT) {
                //Send the packet to the client
            } else {
                throw EpollExceptionEventFailed(errno,mpEpollObject->GetEpollID(),npEvent.data.fd);
            }
        }
    }
    
    
    eErrorCode DataDealer::Run() throw (ThreadExceptionCreateFailed&) {
        if (mbRunning || mThread.joinable())
            return eErrorCode::eIsRunning;
        mThread = std::thread(&DataDealer::TheadCallback, this);
        if (mThread.joinable()) {
            mbRunning = true;
            return eErrorCode::eSuccess;
        } else throw ThreadExceptionCreateFailed(errno);
    }


    //add a socket to this dealer

    eErrorCode DataDealer::AddSocketItem(int nfd, SocketInfo * const npSocketInfo) throw (EpollExceptionCtlFailed&) {
        if (!mpEpollObject)
            return eInvalidObject;
        // std::lock_guard<std::mutex> lock(mMutex);
        struct epoll_event ev;
        ev.events = EPOLLIN; //use the level triggered mode to monitor the reading event
        ev.data.ptr = npSocketInfo;
        return mpEpollObject->AddFileDescriptor(nfd, ev);
    }

    //delete a socket from this dealer

    eErrorCode DataDealer::DeleteSoecktItem(int nfd) throw (EpollExceptionCtlFailed&) {
        if (!mpEpollObject)
            return eInvalidObject;
        //std::lock_guard<std::mutex> lock(mMutex);
        struct epoll_event ev;
        return mpEpollObject->RemoveFileDescriptor(nfd, ev);
    }

}

