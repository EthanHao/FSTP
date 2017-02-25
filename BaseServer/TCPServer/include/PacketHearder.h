/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PacketHearder.h
 * Author: ethan
 *
 * Created on February 24, 2017, 9:26 AM
 */

#ifndef PACKETHEARDER_H
#define PACKETHEARDER_H

namespace CTCPSERVER {
//Using Pack(1) to pack the structure, and using Big-endian
#define PKG_HEADID 0x7caa5caa
#define PKG_TAILID 0xaa55ca81
#pragma pack(push)
#pragma pack(1)  

    struct PacketHeader {
        int mnHeadID;
        int mnContentLen;
        ushort mnContentType;
        ushort mnConpressType;
        long long mllSeq;
        

        //Test this buffer whether it is a valid package
        static bool IsValidPkg(const void* npBuffer, int nLen) {
            if (!npBuffer || nLen < (int) sizeof (PacketHeader) + sizeof(int))
                return false;

            const PacketHeader *lpHeader = (const PacketHeader*) npBuffer;
            if (lpHeader->mnHeadID != PKG_HEADID ||
                    nLen < (int) sizeof (PacketHeader) + lpHeader->mnContentLen + sizeof(int) ||
                    reinterpret_cast<int> ( npBuffer + sizeof (PacketHeader) + lpHeader->mnContentLen) != PKG_TAILID)
                return false;
            return true;
        }
        
    };
    
    
    template<struct T>
    struct Packet {
        PacketHeader mHeader;
        T mContent;
        int mnTailID;
    };
#pragma pack(pop)   
}

#endif /* PACKETHEARDER_H */

