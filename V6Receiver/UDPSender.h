#pragma once

extern "C" {
#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <in6addr.h>
#include <ws2ipdef.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <WS2tcpip.h>
}
#include <inttypes.h>
#include <queue>
#include <iostream>


typedef struct RTHeader {
	double		  time;
	unsigned long packetnum;
} RTHeader_t;

typedef struct Report {
	double recBytes;
	double recPackets;
	double framerate;

} Report;

typedef struct Inputs {
	int keyA = 0;
	int keyB = 0;
	int x = 0;
	int y = 0;
	bool mouse = false;
	
	bool operator!=(const Inputs& other) const {
		return (keyA != other.keyA) ||
			(keyB != other.keyB);
		
	}

	void output() const {
		std::cout << "keyA: " << keyA << std::endl;
		std::cout << "keyB: " << keyB << std::endl;
		std::cout << "x: " << x << std::endl;
		std::cout << "y: " << y << std::endl;
		std::cout << "mouse: " << (mouse ? "true" : "false") << std::endl;
	
	}
};



class UDPSender
{

public:
	int sock = 0;
	struct sockaddr_in6 addr;
	unsigned int packetnum = 0;

	UDPSender() {};
	~UDPSender() {};
	void init(const char* address, int port);
	int startWinsock(void)
	{
		WSADATA wsa;
		return WSAStartup(MAKEWORD(2, 0), &wsa);
	}
	int send(Report report, int size);
	int sendInputs(Inputs inputs, int size);
	void closeSock();
	void start();
	void close();
};

