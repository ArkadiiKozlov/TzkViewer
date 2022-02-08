#include <QDebug>
#include <unistd.h>
#include <QTime>
#include "netreceiver.h"

extern pthread_mutex_t mutex_here;

NetReceiver::NetReceiver(QObject *parent)
    : QObject{parent}
{
    udp_o = new Multicast (14350, true, "224.1.1.1");        
}
void NetReceiver::ReceivePacket(void)
{        
   QTime q_time = QTime::currentTime();
   pthread_mutex_lock(&mutex_here);
   
    while (1) {                 
        memset (buff_in, 0x00, 2000);        
        int size = udp_o->GetPacket((char*)buff_in, 2000000);        
        if (size) {
            if (ts->key == 1) {// key 
                QString q_str = ts->name;     
                q_time = q_time.currentTime();
                qDebug () << q_str << q_time.toString("hh:mm:ss.zzz");                          
                emit SendStringToMain(ts);
                pthread_mutex_lock(&mutex_here);
                
            }
        }
    }
}

//if (strcmp (ts->name, "Frauscher_TS") == 0) {
                    //int index_first = sizeof (PacketHead) + 1 + strlen (ts->name) + 1 + 2 + 2;
                    //track_info = *((uint *)(buff_in + index_first));                         
                //}            
                //if (strcmp (ts->name, "Frauscher_TS_TI") == 0) {
                    //int index_first = sizeof (PacketHead) + 1 + strlen (ts->name) + 1 + 2 + 2;                    
                    //memcpy ((void *)&axels_count, buff_in + index_first, 4);                    
                    //memcpy ((void *)&fse_com_errors, buff_in + index_first + 4, 4);                    
                //}            
//pthread_mutex_lock(&mutex);            
//channel_name_q.push(ts->name);
//pthread_mutex_unlock(&mutex);            

//q_str.append (QString("%1").arg((ushort)*(ts->name +strlen(ts->name) + 1),0,10));
//q_str.append (QString().number((ushort)*(ts->name +strlen(ts->name) + 1)));
//q_str.append ("size: ");                                
//q_str.append (QString::number((ushort)*(ts->name +strlen(ts->name) + 1)));                
//emit SendStringToMain(QString ("%1, %2 %3" ).arg(ts->name).arg ("size:").arg((ushort)*(ts->name +strlen(ts->name) + 1),0,10));                
