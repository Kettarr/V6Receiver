
#pragma once

#ifndef RECEIVETHREAD_H
#define RECEIVETHREAD_H

#include <queue>
#include "UDPReceive6.h"
#include "UDPSender.h"
#include <mutex>
#include "ThreadQueue.h"

extern "C" {
#include <stdio.h>
#include <time.h>
#include <libavcodec/avcodec.h>
#include <libavutil/opt.h>
#include <libavutil/imgutils.h>

}


class ReceiveThread
{

public:
	const AVCodec* codec;
	AVCodecParserContext* parser;
	AVCodecContext* c = NULL;
	AVFrame* frame;

	int ret;

	AVPacket* pkt;


	UDPReceive6 receiver;

	UDPSender sender;

	ThreadQueue* tQ;
	
	ReceiveThread(ThreadQueue* tQ) {
		this->tQ = tQ;
		
		sender.init("::1", 50001);

	};
	~ReceiveThread() {};

	

	void setupStart();
	void operator()() {
		setupStart();
	};


	

	void sendInformation(double recBytes, double recPackets, double framerate);

	void decode(AVCodecContext* dec_ctx, AVFrame* frame, AVPacket* pkt, const char* filename);

};

#endif