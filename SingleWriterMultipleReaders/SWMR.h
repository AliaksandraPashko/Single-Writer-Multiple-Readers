#pragma once
#include <mutex>

class SWMR
{
private:
	int count_active_writers_ = 0;
	int count_active_readers_ = 0;
	
	int count_all_writers_ = 0;
	
	std::condition_variable cv_writer_;
	std::condition_variable cv_reader_;
	
	std::mutex mutex_;

public:
	void start_writing();
	void end_writing();

	void start_reading();
	void end_reading();
};
