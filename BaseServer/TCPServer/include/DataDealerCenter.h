/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DataDealerCenter.h
 * Author: ethan
 *
 * Created on February 14, 2017, 7:34 PM
 */

#ifndef DATADEALERCENTER_H
#define DATADEALERCENTER_H

//Create a bunch of Dealers 
//Dispatch socket to a specified dealer
//for simplicity this class could be singleton 
namespace CTCPSERVER {

    class DataDealerCenter {
    public:
        DataDealerCenter();
        DataDealerCenter(const DataDealerCenter& orig);
        virtual ~DataDealerCenter();
    private:

    };
}
#endif /* DATADEALERCENTER_H */

