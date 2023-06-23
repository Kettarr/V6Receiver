/*
 *  UDPReceive6.cpp
 *
 *  Created by Helmut Hlavacs (2022).
 *
 */
#pragma once

#pragma comment(lib, "ws2_32.lib")


#include "UDPReceive6.h"

#include <iostream>
#include <string.h>
#include <inttypes.h>

extern "C" {
#include <stdio.h>
#include <time.h>
#include <libavcodec/avcodec.h>
#include <libavutil/opt.h>
#include <libavutil/imgutils.h>
}
#define INBUF_SIZE 4096


typedef struct RTHeader {
	double		  time;
	unsigned long packetnum;
} RTHeader_t;



UDPReceive6::UDPReceive6() {
	recbuffer = new char[65000];
}


void UDPReceive6::init(int port) {
	sock = socket(AF_INET6, SOCK_DGRAM, 0);

	memset(&addr, 0, sizeof(addr));

	addr.sin6_family = AF_INET6;
	addr.sin6_port = htons(port);
	addr.sin6_addr = in6addr_any;
	auto ret = bind(sock, (struct sockaddr*)&addr, sizeof(addr));
	//printf("Binding port %d return %d errno %d\n", port, ret, WSAGetLastError());
}


int UDPReceive6::receive(char* buffer, int len, double* ptime) {
	struct sockaddr_in6 si_other;
	socklen_t slen = sizeof(si_other);

	RTHeader_t* pheader = (RTHeader_t*)recbuffer;


	while (true) {
		auto ret = recvfrom(sock, recbuffer, 65000, 0, (sockaddr*)&si_other, &slen);
		//std::cout << "Ret size: " << ret << std::endl;
		if (ret > sizeof RTHeader_t) {

			packetnum = pheader->packetnum;
			memcpy(buffer, recbuffer + sizeof RTHeader_t, ret - sizeof RTHeader_t);
			return ret - sizeof RTHeader_t;
		}
	}
	return 0;
}


void UDPReceive6::closeSock() {
	closesocket(sock);
	sock = 0;
}






/*


int main() {

	const AVCodec* codec;
	AVCodecParserContext* parser;
	AVCodecContext* c = NULL;
	AVFrame* frame;

	int ret;
	
	AVPacket* pkt;

	int cter = 0;



	auto filename = "outfile";

	startWinsock();

	UDPReceive6 receiver;

	receiver.init(50000);

	

	
	double ptime;


	av_register_all();
	avcodec_register_all();

	pkt = av_packet_alloc();
	if (!pkt)
		exit(1);

	//memset(inbuf + INBUF_SIZE, 0, AV_INPUT_BUFFER_PADDING_SIZE);

	codec = avcodec_find_decoder(AV_CODEC_ID_MPEG2VIDEO);
	if (!codec) {
		fprintf(stderr, "Codec not found\n");
		exit(1);
	}
	parser = av_parser_init(codec->id);
	if (!parser) {
		fprintf(stderr, "parser not found\n");
		exit(1);
	}


	c = avcodec_alloc_context3(codec);
	if (!c) {
		fprintf(stderr, "Could not allocate video codec context\n");
		exit(1);
	}




	ret = avcodec_open2(c, codec, NULL);
	if (ret < 0) {
		std::cout << "Could not open codec" << std::endl;
		//fprintf(stderr, "Could not open codec: %s\n", av_err2str(ret));
		exit(1);
	}


	frame = av_frame_alloc();
	if (!frame) {
		fprintf(stderr, "Could not allocate video frame\n");
		exit(1);
	}

	c->bit_rate = 650000;

	c->width = 704;
	c->height = 576;

	c->time_base = { 1, 30 };
	c->framerate = { 30, 1 };


	c->gop_size = 10;
	c->max_b_frames = 1;
	c->pix_fmt = AV_PIX_FMT_YUV420P;


	frame->format = c->pix_fmt;
	frame->width = c->width;
	frame->height = c->height;
	frame->pts = 0;


	while (true) {

		char buf[65000];
		int currentsize = 0;
		
		currentsize = receiver.receive(buf, sizeof buf, &ptime);


		std::cout << "currentsize: " << currentsize << std::endl;
		cter++;

		frame->pts++;

		while (true) {
			int usedBytes = av_parser_parse2(parser, c, &pkt->data, &pkt->size, (uint8_t*)buf, currentsize, ptime, ptime, 0);
			
			
			std::cout << "pkt->size= " << pkt->size << std::endl;


			if (pkt->size > 0) {


				decode(c, frame, pkt, filename, cter);



				

				break;
			}
			
			}
			

			
		}

		
	


	WSACleanup();
	return 0;
}

*/

