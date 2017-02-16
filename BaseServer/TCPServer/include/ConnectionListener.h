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

#ifndef CONNECTIONLISTENER_H
#define CONNECTIONLISTENER_H

#include <vector>
#include <thread>
#include "EPollObject.h"
#include "SocketInfo.h"

class ConnectionListener {
public:
    ConnectionListener(int port, const std::vector<EPollObject*>& epolls);
    ConnectionListener(const ConnectionListener& orig);
    virtual ~ConnectionListener();

    int Listen(int port); 
    void Stop();
     
private:
    std::vector<EPollObject*> mEpolls; //vector of Epoll objects that this listener assigns sockets to
    std::thread mWorker;
    int mPort;
    bool running;
    
    void HandleConnections(int sockfd, struct sockaddr_in cli_addr, socklen_t clilen);
    void CreateSocketInfo(int fd);
    

};

#endif /* CONNECTIONLISTENER_H */

