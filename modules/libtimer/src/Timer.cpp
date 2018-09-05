#include "timer/Timer.hpp"	

#include <iostream>

namespace timer
{

Timer::Timer(Seconds lifetime, Callback callback)
	: m_lifeTime{ lifetime }
	, m_callback{ callback }
{
}

Timer::~Timer()
{
	Stop();
}

void Timer::Run()
{
	Stop();

	// m_stop = false;

	std::lock_guard<std::mutex> lg{ m_threadMtx };

	m_thread = std::thread{ [this] {
		{ auto locked{ std::unique_lock<std::mutex>(m_mutex) };

			std::cout << "In while" << std::endl;

			if (!m_stop)
			{
				auto result{ m_terminate.wait_for(locked, m_lifeTime) };

				if (result == std::cv_status::timeout) {
					m_callback();
					// m_stop = true;
					std::cout << "In if" << std::endl;
				}
			}
		}
	} };
}

void Timer::Stop()
{
	// m_stop = true;

	m_terminate.notify_one();

	if (m_thread.joinable()) {
		m_thread.join();
	}
}

}	// namespace timer