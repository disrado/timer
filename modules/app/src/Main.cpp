#include <chrono>
#include <iostream>

#include <timer/Timer.hpp>

int main()
{
	while(true) {
	timer::Timer timer{ std::chrono::seconds(3), [] { 
		std::cout << "Trigger" << std::endl;
	} };

	timer.Run();

	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::cout << "wait 1" << std::endl;
	
	timer.Stop();

	std::this_thread::sleep_for(std::chrono::seconds(5));
	std::cout << "wait 3" << std::endl;

	timer.Run();
	
	std::this_thread::sleep_for(std::chrono::seconds(10));
	std::cout << "wait 5" << std::endl;

	timer.Run();

	std::this_thread::sleep_for(std::chrono::seconds(15));
	std::cout << "wait 6" << std::endl;

	// std::this_thread::sleep_for(std::chrono::seconds(10));
	
	
	std::cout << std::endl << std::endl;


	}
	return 0;
}

// Output:
// wait 1
// wait 3
// wait 4
// Trigger
// wait 10
// Trigger