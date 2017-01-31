/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TcpServer.h
 * Author: ethan
 *
 * Created on January 30, 2017, 6:12 PM
 */

#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "IServer.h"

//Tcp server is a concrete Iserver.
//get a connection lister object
//get a group of IDataHandler

class TcpServer  {
public:
    TcpServer();
    TcpServer(const TcpServer& orig);
    virtual ~TcpServer();
private:

};

#endif /* TCPSERVER_H */

