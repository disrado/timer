#pragma once

#include <atomic>
#include <chrono>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>

namespace timer
{

class Timer final
{
	//
	// Alias declaration.
	//
public:
	using Seconds = std::chrono::seconds;
	using TimerExpireCallback = std::function<void()>;

	//
	// Construction and destruction.
	//
public:
	//! Constructor.
	Timer(Seconds delay, TimerExpireCallback callback = nullptr);
	//! Destructor.
	~Timer();
	//! Copy constructor.
	Timer(const Timer&) = delete;
	//! Move constructor.
	Timer(Timer&&) = delete;
	//! Copy assignment operator.
	Timer& operator=(const Timer&) = delete;
	//! Move assignment operator.
	Timer& operator=(Timer&&) = delete;

	//
	// Public interface.
	//
public:
	//! Runs the timer. Reruns timer if called again.
	void Start();
	//! Stops the timer.
	void Stop();
	//! Sets callback function.
	void SetCallback(const TimerExpireCallback newCallback);
	//! Returns true if timer is stopped otherwise false.
	bool IsStopped() const;

	//
	// Private data members.
	//
private:
	//! The period of time by the end of which timer invokes callback function.
	Seconds delay_;
	//! Callback function for cases when time is expired.
	TimerExpireCallback callback_;
	//! Flag which indicates whether timer has been stopped.
	std::atomic_bool isStopped_;
	//! Thread for timeout callback.
	std::thread thread_;
	//! Mutex for working with thread.
	std::mutex mutex_;
	//! Mutex for locking members
	std::mutex membersMtx_;
	//! CV for terminating thread if Stop() called.
	std::condition_variable terminate_;
};

}	// namespace timer