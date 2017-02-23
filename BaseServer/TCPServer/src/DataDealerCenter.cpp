/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DataDealerCenter.cpp
 * Author: ethan
 * 
 * Created on February 14, 2017, 7:34 PM
 */

#include "../include/DataDealerCenter.h"
namespace CTCPSERVER {

    DataDealerCenter::DataDealerCenter(int nNum,int nMaxSocketSizePerDealer)throw (EpollExceptionCreateFailed&,
                std::bad_alloc&,
                ThreadExceptionCreateFailed&):
                    mnNumOfDealers(nNum),
                    mnMaxSocketSizePerDealer(nMaxSocketSizePerDealer){
        
        //Allocate a chunk of memory to store the socket info list
        std::unique_ptr<MemoryPool<SocketInfo>> lpMemoryPool(new MemoryPool<SocketInfo>(mnMaxSocketSizePerDealer));
        mpMemoryPool = std::move(lpMemoryPool);
        
        //Create a bunch of dealer
        for(int i = 0 ; i < mnNumOfDealers ; i++)
        {
            std::unique_ptr<DataDealer> lpDeal(new DataDealer(nMaxSocketSizePerDealer));
            mpDealers.push_back(std::move(lpDeal));
        }
    }


    eErrorCode  DataDealerCenter::DispatchSocket(int nfd)throw(EpollExceptionCtlFailed&){
        //Lock
        std::lock_guard<std::mutex> lock(mMutex);
        
        //Check Paremeters
        if(!mpMemoryPool || mpDealers.size() != mnNumOfDealers)
            return eErrorCode::eInvalidObject;
        
        //Assign a index of socket info for this file descriptor
        SocketInfo * lpSocket = mpMemoryPool->alloc();
        if(lpSocket == nullptr)
            return eErrorCode::eMemoryPoolIsFull;
        //Add to the map
        mMapSocket[nfd] = lpSocket;
        //Assign this nfd to a Data Dealer
        return mpDealers[nfd%mnNumOfDealers]->AddSocketItem(nfd,lpSocket);
        
    }
    //Free the resource associated with the socket descriptor

    eErrorCode DataDealerCenter::FreeSocketInfo(int nfd) {
        //Lock
        std::lock_guard<std::mutex> lock(mMutex);
        
        //Check Parameters
        if (!mpMemoryPool || mMapSocket.find(nfd) != mMapSocket.end())
            return eErrorCode::eInvalidObject;
        //Free the memory from the memorypool
        mpMemoryPool->free(mMapSocket[nfd]);

        //Delete this nfd from the map
        mMapSocket.erase(nfd);
        return eErrorCode::eSuccess;
    }
    //Stop the dealer
    eErrorCode DataDealerCenter::StopDealers()
    {
        for(auto& p : mpDealers)
            p->StopAndWait();
        return eErrorCode::eSuccess;
    }
}

