#include "netreceiver.h"


NetReceiver::NetReceiver(QObject *parent)
    : QObject{parent}
{
    udp_o = new Multicast (14350, true, "224.1.1.1");        
}
void NetReceiver::ReceivePacket(void)
{
    unsigned char buff_in [2000];
    PacketTS *ts = (PacketTS*)(buff_in + 42); 
    while (1) {         
        memset (buff_in, 0x00, 2000);
        int size = udp_o->GetPacket((char*)buff_in, 2000000);        
        if (size) {
        
            if (ts->key == 1) {// key
                emit SendStringToMain(ts->name);
                //pthread_mutex_lock(&mutex);            
                //channel_name_q.push(ts->name);
                //pthread_mutex_unlock(&mutex);            
                if (strcmp (ts->name, "Frauscher_TS") == 0) {
                    int index_first = sizeof (PacketHead) + 1 + strlen (ts->name) + 1 + 2 + 2;
                    //track_info = *((uint *)(buff_in + index_first));                         
                }            
                if (strcmp (ts->name, "Frauscher_TS_TI") == 0) {
                    int index_first = sizeof (PacketHead) + 1 + strlen (ts->name) + 1 + 2 + 2;                    
                    //memcpy ((void *)&axels_count, buff_in + index_first, 4);                    
                    //memcpy ((void *)&fse_com_errors, buff_in + index_first + 4, 4);                    
                }            
            } 
        } 
    }
}
