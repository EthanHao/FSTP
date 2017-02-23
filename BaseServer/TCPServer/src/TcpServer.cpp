/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TcpServer.cpp
 * Author: ethan
 * 
 * Created on January 30, 2017, 6:12 PM
 */

#include "TcpServer.h"
#include "DataDealerCenter.h"
namespace CTCPSERVER {

    TcpServer::TcpServer(const std::string& nIP, int nPort,int nNumDealer,int nMaxSocketSizePerDealer,const std::vector<ServerInfo> & nBackServers)throw(SocketExceptionCreateFailed&,
                SocketExceptionSetOptionFailed&,
                SocketExceptionBindFailed&,
                SocketExceptionListenFailed&,
                EpollExceptionCreateFailed&,
                EpollExceptionCtlFailed&,
                std::bad_alloc&,
                ThreadExceptionCreateFailed&,
                LogicalExceptionTooManyBackendServer&,
                LogicalExceptionNoBackendServer&) {
        
        std::unique_ptr<IDataCenterInterface> lpCenter(new DataDealerCenter(nNumDealer,nMaxSocketSizePerDealer,nBackServers));
        mpDataCenter = std::move(lpCenter);
        
        std::unique_ptr<ConnectionListener> lpListener(new ConnectionListener(nIP,nPort,nNumDealer*nMaxSocketSizePerDealer,mpDataCenter.get()));
        mpListener = std::move(lpListener);
    }


    
    eErrorCode TcpServer::Stop(){
        if(mpListener)
            mpListener->StopAndWait();
        if(mpDataCenter)
            mpListener->StopAndWait();
    }
            
}
