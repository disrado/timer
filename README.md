# timer
Simple thread-safe timer which provides the possibility of a deferred function call. 
It is also possible to interrupt this action.

Based on C++11.

# Modules structure
## libtimer

Contains timer logic. Static library.

## app

Simple program which use library.

# Usage

	#include <timer/Timer.hpp>

	timer::Timer timer{ std::chrono::seconds(2), [] {
		std::cout << "Some message" << std::endl;
	} };

	// Starts the countdown to the function call
	timer.Start();
	
	// Interrupts countdown
	timer.Stop();

	// Checks status
	if (timer.IsStopped()) {
		// Sets the new callback
		timer.SetCallback([] { std::cout << "Another message" << std::endl"; });
	}

	// Runs the countdown again
	timer.Start();