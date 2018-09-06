#pragma once

#include <atomic>
#include <chrono>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>


namespace timer
{

using Seconds = std::chrono::seconds;
using TimePoint = std::chrono::time_point<std::chrono::system_clock>;
using Callback = std::function<void()>;

class Timer
{
public:
	Timer(Seconds lifetime, Callback callback);
	~Timer();
	Timer(const Timer&) = delete;
	Timer& operator=(const Timer&) = delete;
	Timer(Timer&&) = default;
	Timer& operator=(Timer&&) = default;

	//! Runs the timer. Reruns timer if called again.
	void Start();
	//! Stops the timer.
	void Stop();
	//! Returns the status of timer.
	bool GetStatus();

private:
	//! How long timer waits before colling callback.
	Seconds m_lifeTime;
	Callback m_callback;
	// Timer is stoped?
	std::atomic_bool m_stop;
	std::thread m_thread;
	std::mutex m_mutex;
	std::mutex m_threadMtx;
	//! CV for terminating thread if Stop() called.
	std::condition_variable m_terminate;
};

using TimerShPtr = std::shared_ptr<Timer>;

}	// namespace timer