/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ByteStreamEventHandler.h
 * Author: ethan
 *
 * Created on February 25, 2017, 5:55 PM
 */

#ifndef BYTESTREAMEVENTHANDLER_H
#define BYTESTREAMEVENTHANDLER_H

#include "EventHandleInterface.h"

namespace CTCPSERVER {
class BytestreamEevetHandler : public IEventHandler{
    virtual bool HandleEvent(const Event & nEvent,void * npReactor) ;

}

#endif /* BYTESTREAMEVENTHANDLER_H */

