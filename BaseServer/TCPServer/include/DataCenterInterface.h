/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IDateCenterInterface.h
 * Author: ethan
 *
 * Created on February 16, 2017, 5:36 PM
 */

#ifndef IDATECENTERINTERFACE_H
#define IDATECENTERINTERFACE_H

namespace CTCPSERVER {

    class IDataCenterInterface {
    public:
        //Add a Socket File Descriptor to deal with its reading or writing operation
        virtual eErrorCode DispatchSocket(int nfd)  throw(EpollExceptionCtlFailed&) = 0;
   
        //Free the resource associated with the socket descriptor
        virtual eErrorCode FreeSocketInfo(int nfd) = 0;
    };
}

#endif /* IDATECENTERINTERFACE_H */

