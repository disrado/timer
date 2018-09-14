#include "timer/Timer.hpp"	

#include <iostream>

namespace timer
{

Timer::Timer(Timer::Seconds delay, Timer::TimerExpireCallback callback)
	: delay_{ delay }
	, callback_{ callback }
	, isStopped_{ true }
{
}

Timer::~Timer()
{
	Stop();
}

void Timer::Start()
{
	Stop();

	isStopped_ = false;

	std::lock_guard<std::mutex> lg{ membersMtx_ };

	thread_ = std::thread{ [this] {
		auto locked{ std::unique_lock<std::mutex>(mutex_) };

		const bool waitingResult{ terminate_.wait_for(locked, delay_, [this] { 
			return static_cast<bool>(this->isStopped_);
		}) };

		// wait_for returns false only if predicate returns false after waiting.
		// It is our case.
		if (!waitingResult)
		{
			{
				std::lock_guard<std::mutex> lg{ membersMtx_ };

				if (callback_) {
					callback_();
				}
			}
			isStopped_ = true;
		}
	} };
}

void Timer::Stop()
{
	isStopped_ = true;

	terminate_.notify_one();

	{
		std::lock_guard<std::mutex> lg{ membersMtx_ };
		
		if (thread_.joinable()) {
			thread_.join();
		}
	}
}

void Timer::SetCallback(Timer::TimerExpireCallback newCallback)
{
	{
		std::lock_guard<std::mutex> lg{ membersMtx_ };
		
		callback_ = newCallback;
	}
}

bool Timer::IsStopped() const
{
	return static_cast<bool>(isStopped_);
}

}	// namespace timer