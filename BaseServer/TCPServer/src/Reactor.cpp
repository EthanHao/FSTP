/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Reactor.cpp
 * Author: ethan
 * 
 * Created on February 14, 2017, 7:36 PM
 */

#include "Reactor.h"
#include "MemoryPool.h"
#include <netinet/in.h>
#include <arpa/inet.h>
namespace CTCPSERVER {

    Reactor::Reactor(int nMaxNumOfSocket,int nNumOfWorkers) throw (EpollExceptionCreateFailed&,
            std::bad_alloc&,
            ThreadExceptionCreateFailed&) :
    mnMaxNumOfSocket(nMaxNumOfSocket + 1) {

      
        std::unique_ptr<EPollObject> lpEpoll(new EPollObject(mnMaxNumOfSocket));
        mpEpollObject = std::move(lpEpoll);

        //allocate memory for the event array
        std::unique_ptr<struct epoll_event[] > lpEvents(new epoll_event[mnMaxNumOfSocket], std::default_delete<struct epoll_event[]>());
        mpEpollEvents = std::move(lpEvents);

        //Allocate a chunk of memory to store the socket info list
        std::unique_ptr<MemoryPool < SocketInfo >> lpMemoryPool(new MemoryPool<SocketInfo>(mnMaxNumOfSocket));
        mpMemoryPool = std::move(lpMemoryPool);

        //Create the workers
        std::unique_ptr<ThreadPool<Event>> lpThreadPool(new ThreadPool<Event>(nNumOfWorkers));
        mpThreadPool = std::move(lpThreadPool);
        
        

        //Run
        mbRunning = false;
        Run();
    }

    Reactor::~Reactor() {
        //Wait the termination of the thread
        StopAndWait();
    }
    /*//Connect to the backend Server

    eErrorCode Reactor::Connect() throw (SocketExceptionCreateFailed&,
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
        mpEpollObject->AddFileDescriptor(mBackendSocket, lEvent);

    }*/


    //Thread call back function

    void Reactor::TheadCallback() {

        while (mbRunning && mpEpollObject && mpEpollEvents) {
            try {
                //Wait
                int nfds = mpEpollObject->Wait(&mpEpollEvents[0], mnMaxNumOfSocket);
                //accept this client one by one
                for (int i = 0; i < nfds; i++) {
                    try {
                        DataReadAndWritting(mpEpollEvents[i]);
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

    eErrorCode Reactor::DataReadAndWritting(struct epoll_event& npEvent) {
        {
            if (npEvent.data.fd < 0)
                throw SocketExceptionInvalidDescriptor(errno, npEvent.data.fd);
            //deal with the normal data about the clients
            if (npEvent.events & EPOLLIN) {
               
            } else if (npEvent.events & EPOLLOUT) {
              
            } else {
                throw EpollExceptionEventFailed(errno, mpEpollObject->GetEpollID(), npEvent.data.fd);
            }
        }
    }

    /*eErrorCode Reactor::ReadFromClient(int nSocketFD) {
        //Read the content to append to the existing buffer which has content reading previously
        //Parse the content 
        //For loop
        //  --Delete the invalid data from this stream
        //  --if we can get a valid packet from this content
        //  ----Deal this packet(including append the packet to the waiting list to send over to the backend)
        //  --else wait to the next time to get a valid packet
        

    }

    eErrorCode Reactor::ReadFromBackendServer(int nSocketFD) {

    }

    eErrorCode Reactor::WriteToClient(int nSocketFD) {
        //Send all the sending buffer in waiting list 
        //if write to the kernal buffer until it is full
        //Change the epollevent to epollout|epoollin

    }

    eErrorCode Reactor::WriteToBackendServer(int nSocketFD) {

    }
    */
    eErrorCode Reactor::Run() throw (ThreadExceptionCreateFailed&) {
        if (mbRunning || mThread.joinable())
            return eErrorCode::eIsRunning;
        mThread = std::thread(&Reactor::TheadCallback, this);
        if (mThread.joinable()) {
            mbRunning = true;
            return eErrorCode::eSuccess;
        } else throw ThreadExceptionCreateFailed(errno);
    }


    //add a socket to this dealer

    eErrorCode Reactor::AddSocketItem(int nfd)
    throw (LogicalExceptionNoEmptyRoonInMemoryPool&, EpollExceptionCtlFailed&) {

        //Because this function will be called in context of other threads
        //so we should keep the shared data safe.
        std::lock_guard<std::mutex> lock(mMutex);

        if (!mpEpollObject)
            return eInvalidObject;

        //Assign a index of socket info for this file descriptor
        SocketInfo * lpSocket = mpMemoryPool->alloc();
        if (lpSocket == nullptr)
            throw LogicalExceptionNoEmptyRoonInMemoryPool(errno, nfd);
        //Construct this chunk of memory
        lpSocket->Set(nfd);

        struct epoll_event ev;
        ev.events = EPOLLIN; //use the level triggered mode to monitor the reading event
        ev.data.ptr = lpSocket;
        try {
            mpEpollObject->AddFileDescriptor(nfd, ev);
        } catch (EpollExceptionCtlFailed& e) {
            mpMemoryPool->free(lpSocket);
            throw e;
        }
        return eErrorCode::eSuccess;
    }

    //delete a socket from this dealer

    eErrorCode Reactor::DeleteSoecktItem(int nfd) throw (EpollExceptionCtlFailed&) {

        std::lock_guard<std::mutex> lock(mMutex);
        if (!mpEpollObject)
            return eInvalidObject;

        //Check Parameters
        if (!mpMemoryPool || mMapSocket.find(nfd) != mMapSocket.end())
            return eErrorCode::eInvalidObject;
        //Free the memory from the memorypool
        mpMemoryPool->free(mMapSocket[nfd]);

        //Delete this nfd from the map
        mMapSocket.erase(nfd);

        struct epoll_event ev;
        return mpEpollObject->RemoveFileDescriptor(nfd, ev);
    }

}

