/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DataDealer.h
 * Author: ethan
 *
 * Created on February 14, 2017, 7:36 PM
 */

#ifndef DATADEALER_H
#define DATADEALER_H

#include "ErrorCode.h"
#include "EPollObject.h"
#include <memory>
#include <atomic>
#include "Exception.h"
#include <thread>
#include "SocketInfo.h"
#include "ServerInfo.h"
namespace CTCPSERVER {

    class DataDealer {
    public:
        DataDealer(int nNum, const ServerInfo& nBackendServer) throw (EpollExceptionCreateFailed&,
                std::bad_alloc&,
                ThreadExceptionCreateFailed&,
                SocketExceptionCreateFailed&,
                SocketExceptionP2NWrongFormat&,
                SocketExceptionP2NFailed&,
                SocketExceptionConnectFailed&,
                SocketExceptionSetOptionFailed&);
        DataDealer(const DataDealer& orig) = delete;
        virtual ~DataDealer();

        //Connect to the backend Server
        eErrorCode Connect()throw (SocketExceptionCreateFailed&,
                SocketExceptionP2NWrongFormat&,
                SocketExceptionP2NFailed&,
                SocketExceptionConnectFailed&,
                SocketExceptionSetOptionFailed&);

        //Run this Dealer in his own thread
        eErrorCode Run() throw (ThreadExceptionCreateFailed&);
        //the callback function of thread
        void TheadCallback();

        //Stop Data Reading and Writing , means stopping the thread
        inline bool Stop() {
            if (mbRunning == false || !mThread.joinable())
                return false;
            mbRunning = false;
            return true;
        }
        
        inline bool StopAndWait() {
            if(mbRunning == false || !mThread.joinable())
                return false;
            mThread.join();
            mbRunning = false;
            return true;
        }
        //add a socket to this dealer
        eErrorCode AddSocketItem(int nfd, SocketInfo * const npSocketInfo) throw(EpollExceptionCtlFailed&);
        //delete a socket from this dealer
        eErrorCode DeleteSoecktItem(int nfd) throw(EpollExceptionCtlFailed&);
        
        //Data things
        eErrorCode DataReadAndWritting(const struct epoll_event& npEvent);
    private:
        const int mnMaxNumOfSocket;
        //std::mutex mMutex;
        std::unique_ptr<EPollObject> mpEpollObject;
        std::unique_ptr<struct epoll_event[]> mpEpollEvents; 
        
        //thread content
        std::thread mThread;
        std::atomic<bool> mbRunning;
        
        //Backend Server, we could pass the data packet to the backend server via socket
        ServerInfo mBackendServer;
        int mBackendSocket;
        
    };
}
#endif /* DATADEALER_H */

