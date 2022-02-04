#ifndef _udp_crtc_h
#define _udp_crtc_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h> 
#include <sys/socket.h>
#include <sys/types.h>

////////////////////////////////////////////////////////////////////////////
// Note!
// Remember, you should up multicast network by the route utilit:
// route add -net 224.0.0.0 netmask 224.0.0.0 eth0
// group from 224.0.0.1 - 224.0.0.255 are rezerved, don't use.
////////////////////////////////////////////////////////////////////////////

#define	mtu_size	1500	// maximum transfer unit

class udp_crtc {
	protected:
		int sFd;// socket file descriptor
		int sockAddrSize;// socket address size
		struct sockaddr_in server_saddr;// server's socket address
		struct sockaddr_in client_saddr;// client's socket address
	public:
		udp_crtc ();
		~udp_crtc ();
		int GetPacket (char *);//blocking method
		int GetPacket (char *, int timeout);
		int GetPacket (char *_buff, int timeout, int &notSleepTime);
		int SndPacket (void *, int);
		char *GetClientIP (void);
		void tcpClParam (struct sockaddr_in *);
		int GetServerPort (void);
};

class Unicast : public udp_crtc {
	public:
		/**
		 * @brief Unicast модуль для работы с утью по протоколу unicast
		 * @param _port Порт приема/отправки сообщений
		 * @param _direction true - объект сервер, false - объект клиент
		 * @param _addr_server - адрес на который отправляются сообщения
		 */
		Unicast(unsigned long _port, bool _direction, const char *_addr_server = 0);
};

class Multicast : public udp_crtc {
	private:
		struct ip_mreq imr;// request structure
		struct in_addr ifaddr;// outgoing interface
	public:
		Multicast (unsigned long, bool, const char *, int = 1);
};

#endif

// End of file
