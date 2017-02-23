/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Exception.h
 * Author: ethan
 *
 * Created on February 14, 2017, 7:50 AM
 */

#ifndef EXCEPTION_H
#define EXCEPTION_H
#include <errno.h>
#include <string.h> 
#include <exception>
namespace CTCPSERVER {

    class EpollException : public std::exception {
    public:

        EpollException(const int nErrorNo, const int nEpollHandle = 0,const int nSocketHandle = 0) : 
            _errorno(nErrorNo), 
            _epollHandle(nEpollHandle),
            _socketHandle(nSocketHandle) {
        }

        const char* what() const throw () {
            return strerror(_errorno);
        }
        const int GetErrorNo() {
            return _errorno;
        }
        const int GetSocketHandle() {
            return _socketHandle;
        }
        const int GetEpollHandle() {
            return _epollHandle;
        }
    private:
        const int _errorno;
        const int _epollHandle;
        const int _socketHandle;
      
    };

    class EpollExceptionCreateFailed : public EpollException {
    public:
        EpollExceptionCreateFailed(const int nErrorNo,const int nEpollHandle = 0,const int nSocketHandle = 0) : 
            EpollException(nErrorNo,nEpollHandle,nSocketHandle) {
        }
    };

    class EpollExceptionCloseFailed : public EpollException {
    public:
        EpollExceptionCloseFailed(const int nErrorNo,const int nEpollHandle = 0,const int nSocketHandle = 0) : 
            EpollException(nErrorNo,nEpollHandle,nSocketHandle) {
        }

    };
    
    class EpollExceptionCtlFailed : public EpollException {
    public:
        EpollExceptionCtlFailed(const int nErrorNo,const int nEpollHandle = 0,const int nSocketHandle = 0) : 
            EpollException(nErrorNo,nEpollHandle,nSocketHandle) {
        }

    };
    
    class EpollExceptionWaitFailed : public EpollException {
    public:
        EpollExceptionWaitFailed(const int nErrorNo,const int nEpollHandle = 0,const int nSocketHandle = 0) : 
            EpollException(nErrorNo,nEpollHandle,nSocketHandle) {
        }

    };
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Socket Exception
    ////////////////////
    class SocketException : public std::exception {
    public:

        SocketException(const int nErrorNo,const int nSocketHandle = 0) : _errorno(nErrorNo),_socketHandle(nSocketHandle) {
        }

        const char* what() const throw () {
            return strerror(_errorno);
        }
        const int GetErrorNo() {
            return _errorno;
        }
        const int GetSocketHandle() {
            return _socketHandle;
        }
    private:
        const int _errorno;
        const int _socketHandle;

    };
    
    class SocketExceptionCreateFailed : public SocketException {
    public:
        SocketExceptionCreateFailed(const int nErrorNo,const int nSocketHandle = 0) : SocketException(nErrorNo,nSocketHandle) {
        }
    };
    class SocketExceptionSetOptionFailed : public SocketException {
    public:
        SocketExceptionSetOptionFailed(const int nErrorNo,const int nSocketHandle = 0) : SocketException(nErrorNo,nSocketHandle){
        }
    };
    class SocketExceptionBindFailed : public SocketException {
    public:
        SocketExceptionBindFailed(const int nErrorNo,const int nSocketHandle = 0) : SocketException(nErrorNo,nSocketHandle) {
        }
    };
    class SocketExceptionListenFailed : public SocketException {
    public:
        SocketExceptionListenFailed(const int nErrorNo,const int nSocketHandle = 0) : SocketException(nErrorNo,nSocketHandle){
        }
    };
     class SocketExceptionAcceptFailed : public SocketException {
    public:
        SocketExceptionAcceptFailed(const int nErrorNo,const int nSocketHandle = 0) : SocketException(nErrorNo,nSocketHandle) {
        }
    };
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Thread Exception
    class ThreadException : public std::exception {
    public:

        ThreadException(const int& nErrorNo) : _errorno(nErrorNo) {
        }

        const char* what() const throw () {
            return strerror(_errorno);
        }
        const int GetErrorNo() {
            return _errorno;
        }
    private:
        const int _errorno;

    };
    
    class ThreadExceptionCreateFailed : public ThreadException {
    public:
        ThreadExceptionCreateFailed(const int& nErrorNo) : ThreadException(nErrorNo) {
        }
    };
}

#endif /* EXCEPTION_H */

