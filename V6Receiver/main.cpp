#pragma once

#include "ReceiveThread.h"
#include <time.h>
#include <thread>
#include "RenderThread.h"
#include "ThreadQueue.h"







int main() {

	
	
	auto tQ = ThreadQueue();
	
	ReceiveThread recT(&tQ);
	RenderThread renT(&tQ);

	std::thread A(std::ref(recT));

	std::thread B(std::ref(renT));

	A.join();
	
	B.join();
	

	


	return 0;
}