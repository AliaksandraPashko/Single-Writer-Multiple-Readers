#include "SWMR.h"

void SWMR :: start_writing()
{
	std::unique_lock<std::mutex> unique_lock(mutex_writer_);
	count_all_writers_++;
	count_active_writers_++;
	count_all_writers_--;
	cv_writer_.wait(unique_lock, [this]() { return (count_active_readers_ == 0); });
}

void SWMR :: end_writing()
{
	std::lock_guard<std::mutex> lock_g(mutex_writer_);
	count_active_writers_--;
	cv_writer_.notify_all();
}

void SWMR :: start_reading()
{
	std::unique_lock<std::mutex> unique_lock(mutex_writer_);
	cv_writer_.wait(unique_lock, [this]() { return (count_all_writers_ == 0) && (count_active_writers_ == 0); });

	std::lock_guard<std::mutex> lock_g(mutex_reader_);
	count_all_readers_++;
	count_active_readers_++;
	count_all_readers_--;
	}

void SWMR :: end_reading()
{
	std::lock_guard<std::mutex> lock_g(mutex_reader_);
	count_active_readers_--;
	cv_reader_.notify_all();
}

