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

    DataDealer::DataDealer(int nNum) throw (EpollExceptionCreateFailed&,
            std::bad_alloc&,
            ThreadExceptionCreateFailed&) :
            mnMaxNumOfSocket(nNum){
         std::unique_ptr<EPollObject> lpEpoll(new EPollObject(mnMaxNumOfSocket));
         mpEpollObject = std::move(lpEpoll);
         
         //allocate memory for the event array
        std::unique_ptr<struct epoll_event[]> lpEvents(new epoll_event[mnMaxNumOfSocket],std::default_delete<struct epoll_event[]>());
        mpEpollEvents = std::move(lpEvents);
        
        //RUn
        mbRunning = false;
        Run();
    }

    DataDealer::~DataDealer() {
    }
    
    //T
    void DataDealer::TheadCallback(){
        
        while(mbRunning && mpEpollObject && mpEpollEvents)
        {
            //Wait
            int nfds =  mpEpollObject->Wait(&mpEpollEvents[0],mnMaxNumOfSocket);
            if(nfds == -1)
            {
                //Log the error and 
                mbRunning = false;
                break;
            }
            
            //accept this client one by one
            for(int i = 0; i < nfds ; i++)
               ;
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
        struct epoll_event ev;
        ev.events = EPOLLIN; //use the level triggered mode to monitor the reading event
        ev.data.ptr = npSocketInfo;
        return mpEpollObject->AddFileDescriptor(nfd,ev);
    }
    //delete a socket from this dealer
    eErrorCode DataDealer::DeleteSoecktItem(int nfd) throw(EpollExceptionCtlFailed&) {
        if(!mpEpollObject)
            return eInvalidObject;
        struct epoll_event ev;
        return mpEpollObject->RemoveFileDescriptor(nfd,ev);

    }

}

