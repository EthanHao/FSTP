/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SocketInfo.cpp
 * Author: ethan
 * 
 * Created on January 30, 2017, 5:27 PM
 */

#include "SocketInfo.h"
namespace CTCPSERVER {

   
    SocketInfo::SocketInfo(){
        mnSocketHandle = 0;
        mnReadLen = 0;
        mnUnwrittenLen = 0;
    }
    void SocketInfo::Set(int nSocket){
        return;
    }

}
