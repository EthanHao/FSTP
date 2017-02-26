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
    

}
