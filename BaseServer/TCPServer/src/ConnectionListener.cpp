/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ConnectionLister.cpp
 * Author: ethan
 * 
 * Created on January 30, 2017, 5:29 PM
 */
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ConnectionListener.h"
#include "SocketInfo.h"

ConnectionListener::ConnectionListener(int port, const std::vector<EPollObject*>& epolls) : mWorker(), mPort(port), mEpolls(epolls) {}

ConnectionListener::ConnectionListener(const ConnectionListener& orig) : mWorker(), mPort(orig.mPort), mEpolls(orig.mEpolls){}

ConnectionListener::~ConnectionListener() 
{
    for (unsigned int i = 0; i < mEpolls.size(); i++)
    {
        delete mEpolls[i];
    } 
}

//take a port number, 
void ConnectionListener::Listen(int port) 
{
    int sockfd;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
         
    {
        printf("ERROR opening socket\n");
    }
        
     memset(&serv_addr, 0, sizeof(serv_addr));
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(port);
     
    if (bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("ERROR on binding\n");
    }
              
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     mWorker = std::thread([=]{HandleConnections(sockfd, cli_addr, clilen);});
     
    
}

void ConnectionListener::Stop() 
{
    mWorker.join();
}

void ConnectionListener::HandleConnections(int sockfd, struct sockaddr_in cli_addr, socklen_t clilen)
{
    
    while(true)
    {
        int newSock;
        newSock = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        
        if (newSock < 0)
        {
            printf("Error on accept\n");
            continue;
        }
  
        
    }
    
    
    
}

void ConnectionListener::CreateSocketInfo(int fd)
{
    //SocketInfo* sock = new SocketInfo(fd);
    //get an epoll object
    //register this SocketInfo with that epoll object
    
}



