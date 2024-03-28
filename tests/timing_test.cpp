// UFO
#include <ufo/utility/timing.hpp>

// Catch2
#include <catch2/catch_test_macros.hpp>

// STL
#include <thread>

TEST_CASE("Timing")
{
	using namespace std::chrono_literals;

	// ufo::Timing t2("Viz");
	// ufo::Timing t3("Integration");

	// auto& a = t3.start();

	// std::thread t1([&t = a]() {
	// 	std::this_thread::sleep_for(1ms);
	// 	t.start("Ray casting");
	// 	std::this_thread::sleep_for(10ms);
	// 	t.stop();
	// });

	// std::this_thread::sleep_for(4ms);
	// t3.start("Marching cubes");
	// std::this_thread::sleep_for(10ms);
	// t3.stop();

	// t1.join();

	// t3.stopAll();

	// ufo::Timing t("Tag", t2, t3);
	// t.printMilliseconds(true, true, true, 3, 10, 1);

	// ufo::Timing t("Test");

	// std::thread t1([&t]() {
	// 	std::this_thread::sleep_for(10ms);
	// 	t.start("Hej");
	// 	std::this_thread::sleep_for(10ms);
	// 	t.stop();
	// });

	// std::this_thread::sleep_for(10ms);
	// t.start("Hej");
	// std::this_thread::sleep_for(10ms);
	// t.stop();

	// t1.join();

	// ufo::Timing t("Test");

	// std::thread t1{[&t]() {
	// 	ufo::Timing& a = t.start("Thread 1");
	// 	std::this_thread::sleep_for(5ms);
	// 	std::thread t2([&t = a]() {
	// 		std::this_thread::sleep_for(5ms);
	// 		t.start("Thread 1.2");
	// 		std::this_thread::sleep_for(5ms);
	// 		t.stop();
	// 	});
	// 	t.start("Wow");
	// 	std::this_thread::sleep_for(10ms);
	// 	t.stopAll();

	// 	t2.join();
	// }};

	// std::thread t2{[&t]() {
	// 	t.start("Thread 2");
	// 	std::this_thread::sleep_for(5ms);
	// 	t.stop();
	// }};

	// t1.join();
	// t2.join();

	// t.printNanoseconds(true, true, true, 1, 10, 2);

	ufo::Timing t("Test");

	auto& a = t.start("First").start("A");

	a.start("B").start("C").start("A");

	a.stopAll();

	a.start().start("B").start("C");

	t.stopAll();

	t.printMilliseconds(true, true, true, 1, 10, 2);

	for (std::size_t i{}; 1000 > i; ++i) {
		t.start("First").start("A");

		a.start("B").start("C").start("A");

		a.stopAll();

		a.start();
		a.start("B").start("C");

		t.stopAll();

		t.printMilliseconds(true, true, true, 1, 10, 2);
	}

	// ufo::Timing t("Test");

	// // t.start();
	// t.start("First");

	// t.start("A");

	// t.stop();
	// t.stop();
	// t.start("Second");
	// t.stop();
	// t.stop();

	// // std::thread t1([&t]() {
	// // 	t.stopAll();
	// // 	// std::this_thread::sleep_for(10ms);
	// // 	// t.start("Hej");
	// // 	// std::this_thread::sleep_for(10ms);
	// // 	// t.stop();
	// // });

	// // t.stopAll();

	// // // std::this_thread::sleep_for(10ms);
	// // // t.start("Hej");
	// // // std::this_thread::sleep_for(10ms);
	// // // t.stop();

	// // t1.join();

	// t.printMilliseconds(true, true, true, 2, 10, 2);
}