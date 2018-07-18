#include "WriterReaderServer.h"
#include <iostream>

std::mutex mtx;
WriterReaderServer swmr;

void read()
{
	swmr.start_reading();
	mtx.lock();
	std::cout << "***" << std::endl;
	mtx.unlock();
	swmr.end_reading();
}

void write()
{
	swmr.start_writing();
	mtx.lock();
	std::cout << "***" << std::endl;
	mtx.unlock();
	swmr.end_writing();
}

int main()
{
	std::thread t3(read);
	std::thread t4(write);
	std::thread t5(write);
	std::thread t1(read);
	std::thread t7(write);
	std::thread t6(read);
	std::thread t2(read);



	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	t6.join();
	t7.join();
	return 0;
}