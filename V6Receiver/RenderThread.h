#pragma once
#include <queue>
#include <SDL.h>
#include "ThreadQueue.h"
#include "UDPSender.h"
extern "C" {
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/frame.h>
}



#include <SDL_thread.h>
#undef main

extern "C" {
#include <libavutil/frame.h>
}
class RenderThread
{
public:
	ThreadQueue* tQ;
	UDPSender sender;
	Inputs inputs;

	RenderThread(ThreadQueue* tQ) {
		this->tQ = tQ;
		sender.init("::1", 50001);
	}
	~RenderThread() {};


	void operator()() {
		checkLoop();
	};

	void sendInformation(double recBytes, double recPackets, double framerate);

	void sendInformation(Inputs inputs);

	void renderFrame(SDL_Renderer* renderer, SDL_Texture* texture);

	void renderFrame();

	void renderFrame(SDL_Window* window);

	void checkLoop();
	
};

