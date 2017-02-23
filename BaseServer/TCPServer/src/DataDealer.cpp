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
namespace CTCPSERVER {

    DataDealer::DataDealer(int nNum,const ServerInfo& nBackendServer ) throw (EpollExceptionCreateFailed&,
            std::bad_alloc&,
            ThreadExceptionCreateFailed&) :
            mnMaxNumOfSocket(nNum),
            mBackendServer(nBackendServer){
        
         std::unique_ptr<EPollObject> lpEpoll(new EPollObject(mnMaxNumOfSocket));
         mpEpollObject = std::move(lpEpoll);
         
         //allocate memory for the event array
        std::unique_ptr<struct epoll_event[]> lpEvents(new epoll_event[mnMaxNumOfSocket],std::default_delete<struct epoll_event[]>());
        mpEpollEvents = std::move(lpEvents);
        
        //Connect to the backend Server
        if(!mBackendServer.IsEnd()) {
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
    eErrorCode DataDealer::Connect(){
        mBackendSocket = ::socket(AF_INET, SOCK_STREAM, SOCK_NONBLOCK);
        if(FAILED(mBackendSocket))
            throw SocketExceptionCreateFailed(errno);
    }
    
    
    //Thread call back function
    void DataDealer::TheadCallback() {
        
        while(mbRunning && mpEpollObject && mpEpollEvents)
        {
             try {
                //Wait
                int nfds = mpEpollObject->Wait(&mpEpollEvents[0], mnMaxNumOfSocket);
                //accept this client one by one
                for (int i = 0; i < nfds; i++){
                    try {
                        //Accept(&mpEpollEvents[i]);
                    }  catch (std::exception& e) {
                        //Log the reason
                        //Continue to work
                    }
                }
            } catch (EpollExceptionWaitFailed& e) {
               
                //If it is system call interrupt then continue
                if(e.GetErrorNo() == EINTR)
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
    
    
    eErrorCode DataDealer::Run() throw(ThreadExceptionCreateFailed&)
    {
        if(mbRunning || mThread.joinable())
            return eErrorCode::eIsRunning;
        mThread = std::thread(&DataDealer::TheadCallback,this);
        if(mThread.joinable()) {
            mbRunning = true;
            return eErrorCode::eSuccess;
        } else throw ThreadExceptionCreateFailed(errno);
    }


    //add a socket to this dealer
    eErrorCode DataDealer::AddSocketItem(int nfd,SocketInfo * const npSocketInfo) throw(EpollExceptionCtlFailed&) {
        if(!mpEpollObject)
            return eInvalidObject;
       // std::lock_guard<std::mutex> lock(mMutex);
        struct epoll_event ev;
        ev.events = EPOLLIN; //use the level triggered mode to monitor the reading event
        ev.data.ptr = npSocketInfo;
        return mpEpollObject->AddFileDescriptor(nfd,ev);
    }
    
    //delete a socket from this dealer
    eErrorCode DataDealer::DeleteSoecktItem(int nfd) throw(EpollExceptionCtlFailed&) {
        if(!mpEpollObject)
            return eInvalidObject;
        //std::lock_guard<std::mutex> lock(mMutex);
        struct epoll_event ev;
        return mpEpollObject->RemoveFileDescriptor(nfd,ev);
    }

}

