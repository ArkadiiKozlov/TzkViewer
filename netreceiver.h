#ifndef NETRECEIVER_H
#define NETRECEIVER_H

#include <QObject>
#include "./udp-s.h"
#include "lan.h"

class NetReceiver : public QObject
{
        Q_OBJECT
    public:
        explicit NetReceiver(QObject *parent = nullptr);
        
    signals:
        void SendStringToMain (unsigned char *, int);
        
    public slots:
        void ReceivePacket (void);
        
    private:
        udp_crtc *udp_o;
        unsigned char buff_in [2000];
        PacketTS *ts = (PacketTS*)(buff_in + 42); 
};

#endif // NETRECEIVER_H
