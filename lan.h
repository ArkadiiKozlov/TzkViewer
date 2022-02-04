#ifndef LAN_H
#define LAN_H

#include <sys/types.h>

#define PACKET_TIME_SIZE	4
#define ADDR3D_NETWORK_SIZE	4
#define ADDR3D_NODE_SIZE	6

struct Address3D {
    unsigned char Network[ADDR3D_NETWORK_SIZE];	
    unsigned char Node[ADDR3D_NODE_SIZE];	
    unsigned short Socket;		
};


struct PacketHead {
    short n; // для выравнивания 
    struct Address3D To, From;	 
    short Number;	
    short Length;	
    char t[PACKET_TIME_SIZE];	
    short Dest;	
    short PakLen;	
    short CurNum;	
    short NumPieces;	
};

#define TS_IDENTIFY               1    // ключ сетевого пакета ТС
#define TU_IDENTIFY               2    // ключ сетевого пакета ТУ
#define TIME_IDENTIFY             3    // time packet from satelite
#define ALIVE_IDENTIFY            5    // я жив!!!

struct PacketTS {
    unsigned char key;		
    char name[50];        			  
    short max_gr;	
    short max_imp;	    
};


struct packetTU {
    unsigned char key;		
    char name[50];
    unsigned  numberTU;
    long TU[55];	
};



#endif // LAN_H
