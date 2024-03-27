// UFO
#include <ufo/utility/timing.hpp>

// Catch2
#include <catch2/catch_test_macros.hpp>

// STL
#include <thread>

TEST_CASE("Timing")
{
	using namespace std::chrono_literals;

	ufo::Timing t("Test");

	t[1].start("What what");
	t[1][2].start("OMG");
	t[1][3].start("Wiee");
	t[1][3][1].start("Hum");
	std::this_thread::sleep_for(5ms);
	t[1][3][1].stop();
	t[1][3].stop();
	t[5][-3].start("Hi");
	t[5][-3].stop();
	t[1][2].stop();
	t[1].stop();

	t.printNanoseconds(true, true, true, 1, 10, 2);
}