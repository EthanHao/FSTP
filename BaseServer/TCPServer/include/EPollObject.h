/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   EPollObject.h
 * Author: ethan
 *
 * Created on January 30, 2017, 5:26 PM
 */

#ifndef EPOLLOBJECT_H
#define EPOLLOBJECT_H

class EPollObject {
public:
    EPollObject();
    EPollObject(const EPollObject& orig);
    virtual ~EPollObject();
    
    //Add a file handle to this epoll object
    eErrorCode AddFileHandle();
    
    //remove a file handle from this epoll object
    eErrorCode RemoveFileHandle();
    
    
    //check this epoll object to get the file handles we are going to deal with
    //or there is something happened on this file handles.
    eErrorCode Wait();
private:

};

#endif /* EPOLLOBJECT_H */

