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

#include "ConnectionLister.h"

ConnectionLister::ConnectionLister(const std::vector<EPollObject*>& epolls) : m(epolls) {}

ConnectionLister::ConnectionLister(const ConnectionLister& orig) : m(orig.m){}

ConnectionLister::~ConnectionLister() 
{
    for (unsigned int i = 0; i < m.size(); i++)
    {
        delete m[i];
    } 
}

void ConnectionLister::Listen(int port) {
    mPort = port;
    
}



