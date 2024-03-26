// UFO
#include <ufo/utility/timer.hpp>

// STL
#include <cmath>

namespace ufo
{
//
// Helper functions
//

namespace impl
{
template <class Period, class Duration>
[[nodiscard]] double duration(Duration dur)
{
	return std::chrono::duration<double, Period>(dur).count();
}
}  // namespace impl

//
// Public functions
//

void Timer::start() { start_ = std::chrono::high_resolution_clock::now(); }

void Timer::reset()
{
	start_   = {};
	samples_ = 0;
	last_    = std::chrono::high_resolution_clock::duration::zero();
	total_   = std::chrono::high_resolution_clock::duration::zero();
	mean_    = std::chrono::duration<double, std::chrono::high_resolution_clock::period>::zero();
	// variance_ = std::chrono::duration<double>::zero();
	variance_ = 0.0;
	min_      = std::chrono::high_resolution_clock::duration::max();
	max_      = std::chrono::high_resolution_clock::duration::min();
}

void Timer::stop()
{
	auto stop = std::chrono::high_resolution_clock::now();

	last_ = stop - start_;

	start_ = {};

	++samples_;

	auto delta = std::chrono::duration<double>(last_ - mean_);
	mean_ += delta / samples_;
	auto delta_2 = std::chrono::duration<double>(last_ - mean_);
	// variance_ += delta * delta_2.count();  // Correct?
	variance_ += impl::duration<std::chrono::nanoseconds::period>(delta) *
	             impl::duration<std::chrono::nanoseconds::period>(delta_2);

	total_ += last_;
	min_ = std::min(min_, last_);
	max_ = std::max(max_, last_);
}

bool Timer::active() const
{
	return std::chrono::time_point<std::chrono::high_resolution_clock>{} != start_;
}

double Timer::currentSeconds() const
{
	return impl::duration<std::chrono::seconds::period>(current());
}

double Timer::currentMilliseconds() const
{
	return impl::duration<std::chrono::milliseconds::period>(current());
}

double Timer::currentMicroseconds() const
{
	return impl::duration<std::chrono::microseconds::period>(current());
}

double Timer::currentNanoseconds() const
{
	return impl::duration<std::chrono::nanoseconds::period>(current());
}

double Timer::lastSeconds() const
{
	return impl::duration<std::chrono::seconds::period>(last_);
}

double Timer::lastMilliseconds() const
{
	return impl::duration<std::chrono::milliseconds::period>(last_);
}

double Timer::lastMicroseconds() const
{
	return impl::duration<std::chrono::microseconds::period>(last_);
}

double Timer::lastNanoseconds() const
{
	return impl::duration<std::chrono::nanoseconds::period>(last_);
}

double Timer::totalSeconds() const
{
	return impl::duration<std::chrono::seconds::period>(current() + total_);
}

double Timer::totalMilliseconds() const
{
	return impl::duration<std::chrono::milliseconds::period>(current() + total_);
}

double Timer::totalMicroseconds() const
{
	return impl::duration<std::chrono::microseconds::period>(current() + total_);
}

double Timer::totalNanoseconds() const
{
	return impl::duration<std::chrono::nanoseconds::period>(current() + total_);
}

double Timer::minSeconds() const
{
	return 0 < numSamples() ? impl::duration<std::chrono::seconds::period>(min_)
	                        : std::numeric_limits<double>::quiet_NaN();
}

double Timer::minMilliseconds() const
{
	return 0 < numSamples() ? impl::duration<std::chrono::milliseconds::period>(min_)
	                        : std::numeric_limits<double>::quiet_NaN();
}

double Timer::minMicroseconds() const
{
	return 0 < numSamples() ? impl::duration<std::chrono::microseconds::period>(min_)
	                        : std::numeric_limits<double>::quiet_NaN();
}

double Timer::minNanoseconds() const
{
	return 0 < numSamples() ? impl::duration<std::chrono::nanoseconds::period>(min_)
	                        : std::numeric_limits<double>::quiet_NaN();
}

double Timer::maxSeconds() const
{
	return 0 < numSamples() ? impl::duration<std::chrono::seconds::period>(max_)
	                        : std::numeric_limits<double>::quiet_NaN();
}

double Timer::maxMilliseconds() const
{
	return 0 < numSamples() ? impl::duration<std::chrono::milliseconds::period>(max_)
	                        : std::numeric_limits<double>::quiet_NaN();
}

double Timer::maxMicroseconds() const
{
	return 0 < numSamples() ? impl::duration<std::chrono::microseconds::period>(max_)
	                        : std::numeric_limits<double>::quiet_NaN();
}

double Timer::maxNanoseconds() const
{
	return 0 < numSamples() ? impl::duration<std::chrono::nanoseconds::period>(max_)
	                        : std::numeric_limits<double>::quiet_NaN();
}

double Timer::meanSeconds() const
{
	return impl::duration<std::chrono::seconds::period>(mean_);
}

double Timer::meanMilliseconds() const
{
	return impl::duration<std::chrono::milliseconds::period>(mean_);
}

double Timer::meanMicroseconds() const
{
	return impl::duration<std::chrono::microseconds::period>(mean_);
}

double Timer::meanNanoseconds() const
{
	return impl::duration<std::chrono::nanoseconds::period>(mean_);
}

double Timer::varianceSeconds() const { return sampleVarianceSeconds(); }

double Timer::varianceMilliseconds() const { return sampleVarianceMilliseconds(); }

double Timer::varianceMicroseconds() const { return sampleVarianceMicroseconds(); }

double Timer::varianceNanoseconds() const { return sampleVarianceNanoseconds(); }

double Timer::stdSeconds() const { return std::sqrt(varianceSeconds()); }

double Timer::stdMilliseconds() const { return std::sqrt(varianceMilliseconds()); }

double Timer::stdMicroseconds() const { return std::sqrt(varianceMicroseconds()); }

double Timer::stdNanoseconds() const { return std::sqrt(varianceNanoseconds()); }

double Timer::sampleVarianceSeconds() const
{
	// return 1 < numSamples() ? impl::duration<std::chrono::seconds::period>(
	//                               variance_ / (numSamples() - 1))
	//                         : std::numeric_limits<double>::quiet_NaN();
	return 1 < numSamples() ? sampleVarianceNanoseconds() / 1000000000
	                        : std::numeric_limits<double>::quiet_NaN();
}

double Timer::sampleVarianceMilliseconds() const
{
	// return 1 < numSamples() ? impl::duration<std::chrono::milliseconds::period>(
	//                               variance_ / (numSamples() - 1))
	//                         : std::numeric_limits<double>::quiet_NaN();
	return 1 < numSamples() ? sampleVarianceNanoseconds() / 1000000
	                        : std::numeric_limits<double>::quiet_NaN();
}

double Timer::sampleVarianceMicroseconds() const
{
	// return 1 < numSamples() ? impl::duration<std::chrono::microseconds::period>(
	//                               variance_ / (numSamples() - 1))
	//                         : std::numeric_limits<double>::quiet_NaN();
	return 1 < numSamples() ? sampleVarianceNanoseconds() / 1000
	                        : std::numeric_limits<double>::quiet_NaN();
}

double Timer::sampleVarianceNanoseconds() const
{
	// return 1 < numSamples() ? impl::duration<std::chrono::nanoseconds::period>(
	//                               variance_ / (numSamples() - 1))
	//                         : std::numeric_limits<double>::quiet_NaN();
	return 1 < numSamples() ? variance_ / (numSamples() - 1)
	                        : std::numeric_limits<double>::quiet_NaN();
}

double Timer::populationVarianceSeconds() const
{
	// return 1 < numSamples()
	//            ? impl::duration<std::chrono::seconds::period>(variance_ / numSamples())
	//            : std::numeric_limits<double>::quiet_NaN();
	return 1 < numSamples() ? populationVarianceNanoseconds() / 1000000000
	                        : std::numeric_limits<double>::quiet_NaN();
}

double Timer::populationVarianceMilliseconds() const
{
	// return 1 < numSamples()
	//            ? impl::duration<std::chrono::milliseconds::period>(variance_ /
	//            numSamples()) : std::numeric_limits<double>::quiet_NaN();
	return 1 < numSamples() ? populationVarianceNanoseconds() / 1000000
	                        : std::numeric_limits<double>::quiet_NaN();
}

double Timer::populationVarianceMicroseconds() const
{
	// return 1 < numSamples()
	//            ? impl::duration<std::chrono::microseconds::period>(variance_ /
	//            numSamples()) : std::numeric_limits<double>::quiet_NaN();
	return 1 < numSamples() ? populationVarianceNanoseconds() / 1000
	                        : std::numeric_limits<double>::quiet_NaN();
}

double Timer::populationVarianceNanoseconds() const
{
	// return 1 < numSamples()
	//            ? impl::duration<std::chrono::nanoseconds::period>(variance_ /
	//            numSamples()) : std::numeric_limits<double>::quiet_NaN();
	return 1 < numSamples() ? variance_ / numSamples()
	                        : std::numeric_limits<double>::quiet_NaN();
}

std::size_t Timer::numSamples() const { return samples_; }

//
// Private functions
//

std::chrono::high_resolution_clock::duration Timer::current() const
{
	return active() ? std::chrono::high_resolution_clock::now() - start_
	                : std::chrono::high_resolution_clock::duration::zero();
}
}  // namespace ufo