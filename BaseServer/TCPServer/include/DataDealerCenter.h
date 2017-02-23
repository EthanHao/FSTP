/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DataDealerCenter.h
 * Author: ethan
 *
 * Created on February 14, 2017, 7:34 PM
 */

#ifndef DATADEALERCENTER_H
#define DATADEALERCENTER_H
#include "ErrorCode.h"
#include "DataDealer.h"
#include "SocketInfo.h"
#include <vector>
#include <map>
#include <mutex>
#include "MemoryPool.h"
#include "DataCenterInterface.h"
//Create a bunch of Dealers 
//Dispatch socket to a specified dealer
//for simplicity this class could be singleton 
namespace CTCPSERVER {

    class DataDealerCenter : public IDataCenterInterface{
    public:
        DataDealerCenter(int nNum,int nMaxSocketSizePerDealer,const std::vector<ServerInfo> & nBackServers)throw (EpollExceptionCreateFailed&,
                std::bad_alloc&,
                ThreadExceptionCreateFailed&,
                LogicalExceptionTooManyBackendServer&,
                LogicalExceptionNoBackendServer&);
        DataDealerCenter(const DataDealerCenter& orig) = delete;
        virtual ~DataDealerCenter() = default;
        
        //Add a Socket File Descriptor to deal with its reading or writing operation
        virtual eErrorCode DispatchSocket(int nfd) throw(EpollExceptionCtlFailed&);
       //Free the resource associated with the socket descriptor
        virtual eErrorCode FreeSocketInfo(int nfd);
        //Stop the dealer
        eErrorCode StopDealers();
    private:

        const int mnNumOfDealers;
        const int mnMaxSocketSizePerDealer;
        std::unique_ptr<MemoryPool<SocketInfo>> mpMemoryPool;
        std::map<int,SocketInfo*> mMapSocket;
        std::vector<std::unique_ptr<DataDealer>> mpDealers;
        std::mutex mMutex;
        
    };
}
#endif /* DATADEALERCENTER_H */

