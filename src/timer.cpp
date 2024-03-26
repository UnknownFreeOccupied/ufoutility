// UFO
#include <ufo/utility/timer.hpp>

namespace ufo
{
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
	mean_ =
	    std::chrono::duration<double, std::chrono::high_resolution_clock::period>::zero();
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
	variance_ += toDouble<std::chrono::seconds::period>(delta) *
	             toDouble<std::chrono::seconds::period>(delta_2);

	total_ += last_;
	min_ = std::min(min_, last_);
	max_ = std::max(max_, last_);
}

bool Timer::active() const
{
	return std::chrono::time_point<std::chrono::high_resolution_clock>{} != start_;
}

double Timer::currentSeconds() const { return current<std::chrono::seconds::period>(); }

double Timer::currentMilliseconds() const
{
	return current<std::chrono::milliseconds::period>();
}

double Timer::currentMicroseconds() const
{
	return current<std::chrono::microseconds::period>();
}

double Timer::currentNanoseconds() const
{
	return current<std::chrono::nanoseconds::period>();
}

double Timer::lastSeconds() const { return last<std::chrono::seconds::period>(); }

double Timer::lastMilliseconds() const
{
	return last<std::chrono::milliseconds::period>();
}

double Timer::lastMicroseconds() const
{
	return last<std::chrono::microseconds::period>();
}

double Timer::lastNanoseconds() const { return last<std::chrono::nanoseconds::period>(); }

double Timer::totalSeconds() const { return total<std::chrono::seconds::period>(); }

double Timer::totalMilliseconds() const
{
	return total<std::chrono::milliseconds::period>();
}

double Timer::totalMicroseconds() const
{
	return total<std::chrono::microseconds::period>();
}

double Timer::totalNanoseconds() const
{
	return total<std::chrono::nanoseconds::period>();
}

double Timer::minSeconds() const { return min<std::chrono::seconds::period>(); }

double Timer::minMilliseconds() const { return min<std::chrono::milliseconds::period>(); }

double Timer::minMicroseconds() const { return min<std::chrono::microseconds::period>(); }

double Timer::minNanoseconds() const { return min<std::chrono::nanoseconds::period>(); }

double Timer::maxSeconds() const { return max<std::chrono::seconds::period>(); }

double Timer::maxMilliseconds() const { return max<std::chrono::milliseconds::period>(); }

double Timer::maxMicroseconds() const { return max<std::chrono::microseconds::period>(); }

double Timer::maxNanoseconds() const { return max<std::chrono::nanoseconds::period>(); }

double Timer::meanSeconds() const { return mean<std::chrono::seconds::period>(); }

double Timer::meanMilliseconds() const
{
	return mean<std::chrono::milliseconds::period>();
}

double Timer::meanMicroseconds() const
{
	return mean<std::chrono::microseconds::period>();
}

double Timer::meanNanoseconds() const { return mean<std::chrono::nanoseconds::period>(); }

double Timer::varianceSeconds() const { return variance<std::chrono::seconds::period>(); }

double Timer::varianceMilliseconds() const
{
	return variance<std::chrono::milliseconds::period>();
}

double Timer::varianceMicroseconds() const
{
	return variance<std::chrono::microseconds::period>();
}

double Timer::varianceNanoseconds() const
{
	return variance<std::chrono::nanoseconds::period>();
}

double Timer::stdSeconds() const { return std<std::chrono::seconds::period>(); }

double Timer::stdMilliseconds() const { return std<std::chrono::milliseconds::period>(); }

double Timer::stdMicroseconds() const { return std<std::chrono::microseconds::period>(); }

double Timer::stdNanoseconds() const { return std<std::chrono::nanoseconds::period>(); }

double Timer::sampleVarianceSeconds() const
{
	return sampleVariance<std::chrono::seconds::period>();
}

double Timer::sampleVarianceMilliseconds() const
{
	return sampleVariance<std::chrono::milliseconds::period>();
}

double Timer::sampleVarianceMicroseconds() const
{
	return sampleVariance<std::chrono::microseconds::period>();
}

double Timer::sampleVarianceNanoseconds() const
{
	return sampleVariance<std::chrono::nanoseconds::period>();
}

double Timer::populationVarianceSeconds() const
{
	return populationVariance<std::chrono::seconds::period>();
}

double Timer::populationVarianceMilliseconds() const
{
	return populationVariance<std::chrono::milliseconds::period>();
}

double Timer::populationVarianceMicroseconds() const
{
	return populationVariance<std::chrono::microseconds::period>();
}

double Timer::populationVarianceNanoseconds() const
{
	return populationVariance<std::chrono::nanoseconds::period>();
}

std::size_t Timer::numSamples() const { return samples_; }
}  // namespace ufo