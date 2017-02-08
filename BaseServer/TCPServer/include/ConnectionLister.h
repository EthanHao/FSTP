/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ConnectionLister.h
 * Author: ethan
 *
 * Created on January 30, 2017, 5:29 PM
 */

#ifndef CONNECTIONLISTER_H
#define CONNECTIONLISTER_H

#include <vector>
#include <thread>
#include "EPollObject.h"
#include "SocketInfo.h"

class ConnectionLister {
public:
    ConnectionLister(const std::vector<EPollObject*>& epolls);
    ConnectionLister(const ConnectionLister& orig);
    virtual ~ConnectionLister();

    void Listen(int port); 
    void Stop();
     
private:
    std::vector<EPollObject*> m; //vector of Epoll objects that this listener assigns sockets to
    std::thread mWorker;
    int mPort;
    void HandleConnection();
    SocketInfo* CreateSocket(int);
    

};

#endif /* CONNECTIONLISTER_H */

