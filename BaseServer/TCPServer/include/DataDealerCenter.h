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
#include "MemoryPool.h"
//Create a bunch of Dealers 
//Dispatch socket to a specified dealer
//for simplicity this class could be singleton 
namespace CTCPSERVER {

    class DataDealerCenter {
    public:
        DataDealerCenter(int nNum,int nMaxSocketSize)throw (EpollExceptionCreateFailed&,
                std::bad_alloc&,
                ThreadExceptionCreateFailed&);
        DataDealerCenter(const DataDealerCenter& orig) = delete;
        virtual ~DataDealerCenter();
        
        //Add a Socket File Descriptor to deal with its reading or writing operation
        eErrorCode DispatchSocket(int nfd);
    private:
        const int mnNumOfDealers;
        const int mnMaxSocketSizePerDealer;
        std::unique_ptr<MemoryPool<SocketInfo>> mpMemoryPool;
        std::map<int,SocketInfo*> mMapSocket;
        std::vector<std::unique_ptr<DataDealer>> mpDealers;
        
    };
}
#endif /* DATADEALERCENTER_H */

