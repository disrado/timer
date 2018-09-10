#include <chrono>
#include <iostream>

#include <timer/Timer.hpp>

#include <chrono>
#include <vector>

int main()
{
	using namespace std::chrono_literals;

	const size_t delay{ 3 };
	std::atomic_int16_t counter{ 0 };

	std::vector<std::shared_ptr<timer::Timer>> inc_timers{
		std::make_shared<timer::Timer>( std::chrono::seconds(delay), [&counter] { counter++; } ),
		std::make_shared<timer::Timer>( std::chrono::seconds(delay), [&counter] { counter++; } ),
		std::make_shared<timer::Timer>( std::chrono::seconds(delay), [&counter] { counter++; } ),
		std::make_shared<timer::Timer>( std::chrono::seconds(delay), [&counter] { counter++; } ),
		std::make_shared<timer::Timer>( std::chrono::seconds(delay), [&counter] { counter++; } ),
		std::make_shared<timer::Timer>( std::chrono::seconds(delay), [&counter] { counter++; } )
	};

	std::vector<std::shared_ptr<timer::Timer>> dec_timers{
		std::make_shared<timer::Timer>( std::chrono::seconds(delay), [&counter] { counter--; } ),
		std::make_shared<timer::Timer>( std::chrono::seconds(delay), [&counter] { counter--; } ),
		std::make_shared<timer::Timer>( std::chrono::seconds(delay), [&counter] { counter--; } ),
		std::make_shared<timer::Timer>( std::chrono::seconds(delay), [&counter] { counter--; } ),
		std::make_shared<timer::Timer>( std::chrono::seconds(delay), [&counter] { counter--; } ),
		std::make_shared<timer::Timer>( std::chrono::seconds(delay), [&counter] { counter--; } )
	};

	try
	{
		size_t loopCounter = 0;

		while (loopCounter++ < 100)
		{
			std::thread doNothing{ [&delay, &inc_timers] {
				for (const auto timer : inc_timers) {
					timer->Start();
				}

				std::this_thread::sleep_for(1s);

				for (const auto timer : inc_timers) {
					timer->Stop();
				}

				std::this_thread::sleep_for(std::chrono::seconds(delay + 1));
			} };

			std::thread increaseCounter{ [&delay, &inc_timers] {
				for (const auto timer : inc_timers) {
					timer->Start();
				}

				std::this_thread::sleep_for(1s);

				for (const auto timer : inc_timers) {
					timer->Stop();
				}

				for (const auto& timer : inc_timers) {
					timer->Start();
				}

				std::this_thread::sleep_for(std::chrono::seconds(delay + 1));

			} };

			std::thread decreaseCounter{ [&delay, &dec_timers] {
				for (const auto timer : dec_timers) {
					timer->Start();
				}

				std::this_thread::sleep_for(std::chrono::seconds(delay + 1));
			} };

			doNothing.join();
			increaseCounter.join();
			decreaseCounter.join();

			if (counter != 0) {
				throw std::runtime_error("Something gone wrong. Counter isn't equal to 0");
			}

			std::cout << '\r' << loopCounter << '%';
		}
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}

	return 0;
}