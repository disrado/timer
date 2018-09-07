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
	using Callback = std::function<void()>;

	//
	// Construction and destruction.
	//
public:
	//! Constructor.
	Timer(Seconds delay, Callback callback);
	//! Destructor.
	~Timer();
	//! Copy constructor.
	Timer(const Timer&) = delete;
	//! Copy assignment operator.
	Timer& operator=(const Timer&) = delete;
	//! Move constructor.
	Timer(Timer&&) = default;
	//! Move assignment operator.
	Timer& operator=(Timer&&) = default;

	//
	// Public interface.
	//
public:
	//! Runs the timer. Reruns timer if called again.
	void Start();
	//! Stops the timer.
	void Stop();
	//! Sets callback function;
	void SetCallback(Callback newCallback);
	//! Returns the status of timer.
	bool GetStatus();

	//
	// Private data members.
	//
private:
	//! How long timer waits before colling callback.
	Seconds delay_;
	//! Callback for calling when time expired.
	Callback callback_;
	// Timer is stoped?
	std::atomic_bool stop_;
	//! Thread for running waiting.
	std::thread thread_;
	//! Mutex for working with thread.
	std::mutex mutex_;
	//! Mutex for locking thread.
	std::mutex threadMtx_;
	//! Mutex for locking callback.
	std::mutex callbackMutex_;
	//! CV for terminating thread if Stop() called.
	std::condition_variable terminate_;
};

}	// namespace timer