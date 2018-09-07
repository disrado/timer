#include "timer/Timer.hpp"	

#include <iostream>


namespace timer
{

Timer::Timer(Timer::Seconds delay, Timer::Callback callback)
	: delay_{ delay }
	, callback_{ callback }
	, stop_{ true }
{
}

Timer::~Timer()
{
	Stop();
}

void Timer::Start()
{
	Stop();

	stop_ = false;

	std::lock_guard<std::mutex> lg{ threadMtx_ };

	thread_ = std::thread{ [this] {
		auto locked{ std::unique_lock<std::mutex>(mutex_) };

		while (!stop_)
		{
			auto result{ terminate_.wait_for(locked, delay_) };

			if (result == std::cv_status::timeout)
			{
				std::lock_guard<std::mutex> lg{ callbackMutex_ };

				if (callback_) {
					callback_();
				}

				stop_ = true;
			}
		}
	} };
}

void Timer::Stop()
{
	stop_ = true;

	terminate_.notify_one();

	{ std::lock_guard<std::mutex> lg{ threadMtx_ };
		if (thread_.joinable()) {
			thread_.join();
		}
	}
}

void Timer::SetCallback(Timer::Callback newCallback)
{
	{ std::lock_guard<std::mutex> lg{ callbackMutex_ };
		callback_ = newCallback;
	}
}

bool Timer::GetStatus()
{
	return static_cast<bool>(stop_);
}

}	// namespace timer