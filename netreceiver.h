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
        void SendStringToMain (QString);
        
    public slots:
        void ReceivePacket (void);
        
    private:
        udp_crtc *udp_o;
        
};

#endif // NETRECEIVER_H
