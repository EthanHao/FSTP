/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SocketInfo.h
 * Author: ethan
 *
 * Created on January 30, 2017, 5:27 PM
 */

#ifndef SOCKETINFO_H
#define SOCKETINFO_H
//store everything related to a socket handle
//like we can assign an unique name for this socket
//the ip address, port, receive buffer, send buffer etc these informations directly related to a socket
class SocketInfo {
public:
    SocketInfo();
    SocketInfo(const SocketInfo& orig);
    virtual ~SocketInfo();
private:

};

#endif /* SOCKETINFO_H */

