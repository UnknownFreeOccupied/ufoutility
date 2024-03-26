// UFO
#include <ufo/utility/timer.hpp>

// Catch2
#include <catch2/catch_test_macros.hpp>

// STL
#include <iostream>
#include <thread>

TEST_CASE("Timer")
{
	using namespace std::chrono_literals;

	ufo::Timer t;

	for (std::size_t i{}; 25 > i; ++i) {
		t.start();
		// std::this_thread::sleep_for(1s);
		t.stop();
		std::cout << t.lastNanoseconds() << ", ";
	}
	std::cout << std::endl;

	std::cout << t.populationVarianceNanoseconds() << std::endl;
	std::cout << t.varianceNanoseconds() << std::endl;
	std::cout << t.stdNanoseconds() << std::endl;
	std::cout << t.meanNanoseconds() << std::endl;
	std::cout << t.totalNanoseconds() << std::endl;
}