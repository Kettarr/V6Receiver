


#pragma comment(lib, "ws2_32.lib")


#include "UDPSender.h"
#include <iostream>


extern "C" {
#include <stdio.h>
#include <time.h>
}




void UDPSender::init(const char* address, int port) {
	sock = socket(AF_INET6, SOCK_DGRAM, 0);

	struct addrinfo hints;

	memset(&addr, 0, sizeof(addr));
	memset(&hints, 0, sizeof(hints));

	hints.ai_family = AF_INET6;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = 0;

	struct addrinfo* result = NULL;
	auto dwRetval = getaddrinfo(address, nullptr, &hints, &result);
	if (dwRetval != 0) {
		printf("getaddrinfo failed with error: %d\n", dwRetval);
		return;
	}
	for (addrinfo* ptr = result; ptr != NULL; ptr = ptr->ai_next) {
		if (ptr->ai_family == AF_INET6) {
			memcpy(&addr, ptr->ai_addr, ptr->ai_addrlen);
			addr.sin6_port = htons(port);
			addr.sin6_family = AF_INET6;
		}
	}
	freeaddrinfo(result);
}



int UDPSender::send(Report report, int size) {
	char sendbuffer[65000];

	packetnum++;

	if (size > 65000) {
		return 0;
	}
	/*
	RTHeader_t header;
	header.time = clock() / (double)CLOCKS_PER_SEC;
	header.packetnum = packetnum;

	int ret;
	memcpy(sendbuffer, &header, sizeof(header));
	*/
	int ret;
	memcpy(sendbuffer, &report, size);

	ret = sendto(sock, sendbuffer,  size, 0, (const struct sockaddr*)&addr, sizeof(addr));
	return ret;
}


int UDPSender::sendInputs(Inputs inputs, int size) {
	char sendbuffer[1000];
	packetnum++;
	if (size > 1000) {
		return 0;
	}

	int ret;
	memcpy(sendbuffer, &inputs, size);

	ret = sendto(sock, sendbuffer, size, 0, (const struct sockaddr*)&addr, sizeof(addr));
	return ret;

}

void UDPSender::closeSock() {
	closesocket(sock);
	sock = 0;
}



void UDPSender::start() {

	startWinsock();

	init("::1", 50001);



	/*for (int i = 0; i < size; i++) {
		std::cout << data[i];
	}*/



}




void UDPSender::close() {
	std::cout << "close" << std::endl;
	WSACleanup();
	closeSock();
}