#include "SWMR.h"
#include <iostream>

void SWMR :: start_writing()
{
	std::unique_lock<std::mutex> unique_lock(mutex_);
	std::cout << "start writing by " << std::this_thread::get_id() << std::endl;
	count_all_writers_++;
	cv_writer_.wait(unique_lock, [this]() { return (count_active_readers_ == 0); });
	count_active_writers_++;
	count_all_writers_--;
	std::cout << "start write work by " << std::this_thread::get_id() << std::endl;
}

void SWMR :: end_writing()
{
	std::lock_guard<std::mutex> lock_g(mutex_);
	count_active_writers_--;

	if (count_all_writers_ != 0)
	{
		cv_writer_.notify_one();
	}
	else
	{
		cv_reader_.notify_all();
	}
	std::cout << "end writing by " << std::this_thread::get_id() << std::endl;
}

void SWMR :: start_reading()
{
	std::unique_lock<std::mutex> unique_lock(mutex_);
	std::cout << "start reading by " << std::this_thread::get_id() << std::endl;
	cv_reader_.wait(unique_lock, [this]() { return (count_all_writers_ == 0) && (count_active_writers_ == 0); });
	std::cout << "start read work by " << std::this_thread::get_id() << std::endl;

	++count_active_readers_;
}

void SWMR :: end_reading()
{
	{
		std::lock_guard<std::mutex> lock_g(mutex_);
		count_active_readers_--;
	}


	cv_writer_.notify_one();

	std::cout << "end reading by " << std::this_thread::get_id() << std::endl;
}

