
#include <unistd.h>

#include "udp-s.h"

udp_crtc::udp_crtc () {
	sockAddrSize = sizeof (sockaddr_in);
	if ((sFd = socket (AF_INET, SOCK_DGRAM, 0)) <= 0) {
		printf ("can't open socket");
		exit (1);
	}
}
udp_crtc::~udp_crtc () {
	close (sFd);
}


void udp_crtc::tcpClParam (struct sockaddr_in *tmpSockAddr) {
	memcpy (tmpSockAddr, &client_saddr, sizeof (struct sockaddr_in));
}
char *udp_crtc::GetClientIP (void) { 
	struct sockaddr_in tmp;
	memcpy (&tmp, &client_saddr, sizeof (struct sockaddr_in));
	return inet_ntoa (tmp.sin_addr);
}

int udp_crtc::GetServerPort (void) {
	return ntohs (server_saddr.sin_port);
}
int udp_crtc::GetPacket (char *_buff) {
	int size = recvfrom (sFd, _buff, mtu_size, 0, (struct sockaddr *) &client_saddr, (socklen_t *) &sockAddrSize );
	if (size <= 0) {
		printf ("udp rec error\n");
		return 0;
	}
	return size;
}
int udp_crtc::GetPacket (char *_buff, int timeout) {

	struct timeval timing;
	fd_set readfds;
	int retval;
	
	FD_ZERO(&readfds);
	FD_SET(sFd, &readfds);
	
	timing.tv_sec = timeout / 1000000;
	timing.tv_usec = timeout % 1000000;

	int maxvalue = sFd + 1;

	retval = select(maxvalue, &readfds, NULL, NULL, &timing);

	if( retval==-1 || 0==( FD_ISSET (sFd, &readfds )))
	    return 0;

	int size = recvfrom (sFd, _buff, mtu_size, 0, (struct sockaddr *) &client_saddr, (socklen_t *) &sockAddrSize );
	if (size <= 0) {
		printf ("udp rec error\n");
		return 0;
	}
	return size;
}
int udp_crtc::GetPacket (char *_buff, int timeout, int &notSleepTime) {

	struct timeval timing;
	fd_set readfds;
	int retval;
	
	FD_ZERO(&readfds);
	FD_SET(sFd, &readfds);
	
	timing.tv_sec = timeout / 1000000;
	timing.tv_usec = timeout % 1000000;
	
	int maxvalue = sFd + 1;

	retval = select(maxvalue, &readfds, NULL, NULL, &timing);
	
	notSleepTime = timing.tv_sec * 1000000 + timing.tv_usec;

	if ( retval==-1 || 0==( FD_ISSET (sFd, &readfds )))
	    return 0;


	int size = recvfrom (sFd, _buff, mtu_size, 0, (struct sockaddr *) &client_saddr, (socklen_t *) &sockAddrSize );
	if (size <= 0) {
		printf ("udp rec");
		return 0;
	}
	
	return size;
}
int udp_crtc::SndPacket (void *_buff, int _size) {
	int size = sendto (sFd, _buff, _size, 0, (struct sockaddr *) &server_saddr, sockAddrSize);
	if (size <= 0) {
		printf ("udp snd error\n");
		return 0;
	}
	return size;
}

Unicast::Unicast (unsigned long _port, bool _direction, const char *_addr_server) {
	if (_addr_server)
		printf ("Unicast %s: %s:%ld\n", _direction ? "rcv" : "snd", _addr_server, _port);
	else
		printf ("Unicast %s: 0.0.0.0:%ld\n", _direction ? "rcv" : "snd", _port);
/*	if ((sFd = socket (AF_INET, SOCK_DGRAM, 0)) <= 0) {
		perror ("can't open socket");
		exit (1);
	}
*/	int optval = 1;
	if (setsockopt (sFd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof (int)) != 0) {
		printf ("can't reuse requested port");
		exit (1);
	}
	bzero ((char *) &server_saddr, sizeof (struct sockaddr_in));
	server_saddr.sin_family = AF_INET;
	server_saddr.sin_port = htons (_port);

	if (_direction) {   // на приеме
//		printf ("Server Side...\n");
		if (bind (sFd, (struct sockaddr *) &server_saddr, sizeof (struct sockaddr_in)) != 0) {
			printf ("Binding udp socket");
			exit (1);
		}
		server_saddr.sin_addr.s_addr = _addr_server ? inet_addr (_addr_server) : htonl (INADDR_ANY);
		//server_saddr.sin_addr.s_addr = htonl (INADDR_ANY);
	}
	else // на передаче
		if (_addr_server)
			server_saddr.sin_addr.s_addr = inet_addr (_addr_server);
		else {
			printf ("ip addr not defined");
			exit (1);
		}
}

Multicast::Multicast (unsigned long _port, bool _direction, const char *_addr_group, int _ttl_out) {
	printf ("Multicast %s: %s:%ld\n", _direction ? "rcv" : "snd", _addr_group, _port);
//        imr.imr_multiaddr.s_addr = 0xe0010101;     /* 224.1.1.1 */
	imr.imr_multiaddr.s_addr = inet_addr (_addr_group);
	imr.imr_interface.s_addr = htonl (INADDR_ANY);
	ifaddr.s_addr = htonl (INADDR_ANY);
	bzero ((char *)&server_saddr, sizeof (struct sockaddr_in));
	server_saddr.sin_family = AF_INET;
	server_saddr.sin_port = htons (_port);
	server_saddr.sin_addr.s_addr = imr.imr_multiaddr.s_addr;
/*	if ((sFd = socket (AF_INET, SOCK_DGRAM, 0)) <= 0) {
		perror ("can't open socket");
		exit (1);
	}
*/        if (setsockopt (sFd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &imr, sizeof (struct ip_mreq)) != 0) {
		printf ("can't join group");
		exit (1);
	}
	if (setsockopt (sFd, IPPROTO_IP, IP_MULTICAST_IF, &ifaddr, sizeof (ifaddr)) != 0) {
		printf ("can't set multicast source interface");
		exit (1);
	}
	int optval = 1;
	if (setsockopt (sFd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof (int)) != 0) {
		printf ("can't reuse requested port");
		exit (1);
	}
        int nbuf = _ttl_out;
	if (setsockopt (sFd, IPPROTO_IP, IP_MULTICAST_TTL, &nbuf, sizeof (int)) != 0) {
		printf ("can't increase multicast packet TTL ");
		exit (1);
	}	   
	if (_direction) {// we are the server socket
//		printf ("Server Side...\n");
		if (bind (sFd, (struct sockaddr *) &server_saddr, sizeof (struct sockaddr_in)) != 0) {
			perror ("Binding multicast socket");
			exit (1);
		}
	}
}

// End of file
