#include <winsock.h>


typedef struct RTHeader {
	double		  time;
	unsigned long packetnum;
} RTHeader_t;


int startWinsock(void)
{
	WSADATA wsa;
	return WSAStartup(MAKEWORD(2, 0), &wsa);
}