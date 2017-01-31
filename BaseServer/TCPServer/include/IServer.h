/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IServer.h
 * Author: ethan
 *
 * Created on January 30, 2017, 5:21 PM
 */

#ifndef ISERVER_H
#define ISERVER_H

class IServer {
public:
    //Initialize this server
    eErrorCode Init() = 0;
    
    //Shutdown this server
    eErrorCode Shutdown() = 0;
};

#endif /* ISERVER_H */

