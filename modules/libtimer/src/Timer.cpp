#include "timer/Timer.hpp"	

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

	{ auto locked{ std::unique_lock<std::mutex>(m_mutex) };
		m_stop = false;
	}

	m_thread = std::thread{ [this] {
		{ auto locked{ std::unique_lock<std::mutex>(m_mutex) };

			while (!m_stop)
			{
				auto result{ m_terminate.wait_for(locked, m_lifeTime) };

				if (result == std::cv_status::timeout) {
					m_callback();
					return;
				}
			}
		}
	} };
}

void Timer::Stop()
{
	{ auto locked{ std::unique_lock<std::mutex>(m_mutex) };
		m_stop = true;
	}

	m_terminate.notify_one();

	if (m_thread.joinable()) {
		m_thread.join();
	}
}

}	// namespace timer