
#pragma once


#include "ReceiveThread.h"
#include <iostream>
#include <thread>
#include <chrono>


extern "C" {
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>

}




void ReceiveThread::sendInformation(double recBytes, double recPackets, double framerate) {

	std::cout << "sendinfo" << std::endl;
	Report report;
	report.framerate = framerate;
	report.recBytes = recBytes;
	report.recPackets = recPackets;

	std::cout << sizeof(report) << std::endl;
	
	auto size = 24;
	
	sender.send(report, size);
	



}

void ReceiveThread::decode(AVCodecContext* dec_ctx, AVFrame* frame, AVPacket* pkt,
	const char* filename)
{

	int ret = avcodec_send_packet(dec_ctx, pkt);
	if (ret < 0 || ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
		//std::cout << "avcodec_send_packet: " << ret << std::endl;
	}
	while (ret >= 0) {
		ret = avcodec_receive_frame(dec_ctx, frame);
		if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
			//std::cout << "avcodec_receive_frame: " << ret << std::endl;
			break;
		}
		//std::cout << "frame: " << dec_ctx->frame_number << std::endl;


		AVFrame* dstFrame = av_frame_alloc();
		dstFrame->format = AV_PIX_FMT_RGBA;
		dstFrame->width = frame->width;
		dstFrame->height = frame->height;

		av_frame_get_buffer(dstFrame, 32);

		SwsContext* swsContext = sws_getContext(
			frame->width, frame->height, AV_PIX_FMT_YUV420P, // Source dimensions and pixel format
			dstFrame->width, dstFrame->height, AV_PIX_FMT_RGBA, // Destination dimensions and pixel format
			SWS_BICUBIC, // Scaling algorithm
			nullptr, nullptr, nullptr
		);


		sws_scale(
			swsContext, // SwsContext
			frame->data, frame->linesize, // Source frame data and line sizes
			0, frame->height, // Source frame start and end height
			dstFrame->data, dstFrame->linesize // Destination frame data and line sizes
		);

		/*char buffer[1024];

		snprintf(buffer, sizeof(buffer), "%s-%d.png", filename, cter);*/

		tQ -> mt.lock();
		tQ->push(dstFrame);
		tQ->mt.unlock();
		tQ->cv.notify_one();
	
		// write_png(buffer, dstFrame);

		

	}
}



void ReceiveThread::setupStart() {

	sender.startWinsock();
	this->receiver.init(50000);

	double ptime;
	auto filename = "outfile";

	av_register_all();
	avcodec_register_all();

	frame = av_frame_alloc();
	if (!frame) {
		fprintf(stderr, "Could not allocate video frame\n");
		exit(1);
	}


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

	std::chrono::seconds interval(10);

	auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = startTime;

	auto receivedBytes = 0;

	int packets = 0;


	while (true) {

		

		char buf[65000];
		int currentsize = 0;

		currentsize = receiver.receive(buf, sizeof buf, &ptime);

		receivedBytes += currentsize;
		//std::cout << "currentsize: " << currentsize << std::endl;
	
		packets++;
		frame->pts++;

		while (true) {
			int usedBytes = av_parser_parse2(parser, c, &pkt->data, &pkt->size, (uint8_t*)buf, currentsize, ptime, ptime, 0);


			//std::cout << "pkt->size= " << pkt->size << std::endl;


			if (pkt->size > 0) {

				decode(c, frame, pkt, filename);
			
				break;
			}

		}

		currentTime = std::chrono::high_resolution_clock::now();
		//std::cout << "startTime " << startTime << std::endl;
		//std::cout << "currentTime " << currentTime << std::endl;
		auto duration = currentTime - startTime; 
		//std::cout << "duration " << duration << std::endl;
		

		if (duration >= interval) {
			double packn = (double) receiver.packetnum;

			std::cout <<"packetnumber:"<< packn << std::endl;

			double recpack = packn / double(packets);
			double recBytes = receivedBytes / 10.0;
			double framerate = packets / 10.0;

			std::cout << "recBytes: " << recBytes << "recpack: " << recpack << std::endl;
			
			//sendInformation(recBytes, recpack, framerate);
			startTime = currentTime;
			receivedBytes = 0;
		}
		
		
	}



	WSACleanup();

}
