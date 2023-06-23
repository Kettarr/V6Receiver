#pragma once
#include <queue>
#include <mutex>
extern "C" {
#include <libavformat/avformat.h>
}

class ThreadQueue
{
public: 
	std::queue<AVFrame*> frameQueue;
	std::mutex mt;
    std::condition_variable cv;


	ThreadQueue() {
       
    };
	/*ThreadQueue(std::queue<AVFrame*>* frameQueue) {
		this->frameQueue = frameQueue;
		this->mt = mt;
	}*/

    void push(AVFrame* frame) {
        frameQueue.push(frame);
      
    }

    bool empty() {
        return frameQueue.empty();
    }

    AVFrame* front() {
        return frameQueue.front();
    }

    void pop() {
        frameQueue.pop();
    }


	
};

