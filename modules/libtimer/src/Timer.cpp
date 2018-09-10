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

		const bool waitingResult{ terminate_.wait_for(locked, delay_, [this] { 
			return static_cast<bool>(this->stop_);
		}) };

		// wait_for returns false only if predicate returns false after waiting.
		// It is our case.
		if (!waitingResult)
		{
			{ std::lock_guard<std::mutex> lg{ callbackMutex_ };
				if (callback_) {
					callback_();
				}
			}
			stop_ = true;
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

bool Timer::IsStoped()
{
	return static_cast<bool>(stop_);
}

}	// namespace timer