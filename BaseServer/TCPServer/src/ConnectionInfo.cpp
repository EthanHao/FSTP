/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ConnectionInfo.cpp
 * Author: ethan
 * 
 * Created on January 30, 2017, 5:27 PM
 */

#include "ConnectionInfo.h"
namespace CTCPSERVER {

   
    ConnectionInfo::ConnectionInfo(){
        mnSocketHandle = 0;
        mnReadLen = 0;
        mnUnwrittenLen = 0;
    }
    void ConnectionInfo::Set(int nSocket){
        mnSocketHandle = nSocket;
        
        return;
    }
   
    int ConnectionInfo::Send(const char* npBuf, int nLen) {
        //check the unwrittenbuf 
        //send the unwrittenbuf first
        //if the kernal buffer is full , than we append the input buffer to the unwritten buf and set Epollout to wait the next available 
        //---writting time.
        //else write the input buffer to the kernal buffer until return successfully, if this time the kernal is also full,we need to 
        //----copy the rest of the input buffer to the unwritten buffer and set the epollout to wait next time.
        return 0;
    }
        
    int ConnectionInfo::Recieve(void * npStreamChecker){
        //Read the buffer from the kernal buffer
        //combine the mreadbuf with the buffer we just copyed from the kernal buffer
        //call the upper level callback function and passing in the combined buffer
        //set epollin
        return 0;
    }
}
