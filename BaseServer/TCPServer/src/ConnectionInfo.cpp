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
#include <unistd.h>
#include "StreamCheckerInterface.h"
namespace CTCPSERVER {

    ConnectionInfo::ConnectionInfo() {
        mnSocketHandle = 0;
        mnReadLen = 0;
        mnUnwrittenLen = 0;
    }

    void ConnectionInfo::Set(int nSocket) {
        mnSocketHandle = nSocket;

        return;
    }

    int ConnectionInfo::Send(const char* npBuf, int nLen) {
        //check the unwrittenbuf 
        //send the unwrittenbuf first
        //if the kernal buffer is full , than we append the input buffer to the unwritten buf and set Epollout to wait the next available 
        //---writting time.
        //else write the input buffer to the kernal buffer until return successfully, if this time the kernal is also full,we need to 
        //----copy the rest of the input buffer to the unwritten buffer and set the epollout to wait next time.
        return 0;
    }

    void ConnectionInfo::Recieve(void * npStreamChecker)
    throw (SocketExceptionWriteFaild&, LogicalExceptionInvalidObject&, LogicalExceptionNoEngoughBuffer&) {
        //Read the buffer from the kernal buffer
        //combine the mreadbuf with the buffer we just copyed from the kernal buffer
        //call the upper level callback function and passing in the combined buffer
        //set epollin  
        if (npStreamChecker == nullptr)
            throw new LogicalExceptionInvalidObject(errno);
        if (mnReadLen >= MAX_BUFFER_SIZE) //this should be big disaster
            throw new LogicalExceptionNoEngoughBuffer(errno);
        char lsBuffer[MAX_BUFFER_SIZE] = {0};
        //Copy the data reading before
        if (mnReadLen > 0 && mnReadLen < MAX_BUFFER_SIZE)
            memcpy(lsBuffer, mReadBuffer, mnReadLen);

        char * lp = lsBuffer + mnReadLen;
        int lnBufLen = MAX_BUFFER_SIZE - mnReadLen;
        while (true) {
            ssize_t lnReceiveBufferLength = ::read(mnSocketHandle, lp, lnBufLen);
            if (lnReceiveBufferLength < 0) {
                if (errno == EAGAIN /*|| errno == EWOULDBLOCK*/) {
                    //Finish reading the buffer
                    break;
                } else if (errno == EINTR) {
                    //Read function was interupt by the system call
                    continue;
                } else {
                    //Other Error 
                    throw new SocketExceptionWriteFaild(errno, mnSocketHandle, -1);
                }
            } else if ((lnReceiveBufferLength == 0)) {
                /* socket was closed by peer */
                throw new SocketExceptionWriteFaild(errno, mnSocketHandle, -2);
            } else {
                //Continue to read until EAGAIN
                lp += lnReceiveBufferLength;
                lnBufLen -= lnReceiveBufferLength;
            }
        }
        if (npStreamChecker) {
            IStreamChecker* lpStreamChecker = (static_cast<IStreamChecker*> (npStreamChecker));
            (*lpStreamChecker)(lsBuffer, MAX_BUFFER_SIZE - lnBufLen);
        }
    }
}
