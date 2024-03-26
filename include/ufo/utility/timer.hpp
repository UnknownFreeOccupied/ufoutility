/*!
 * UFOMap: An Efficient Probabilistic 3D Mapping Framework That Embraces the Unknown
 *
 * @author Daniel Duberg (dduberg@kth.se)
 * @see https://github.com/UnknownFreeOccupied/ufomap
 * @version 1.0
 * @date 2022-05-13
 *
 * @copyright Copyright (c) 2022, Daniel Duberg, KTH Royal Institute of Technology
 *
 * BSD 3-Clause License
 *
 * Copyright (c) 2022, Daniel Duberg, KTH Royal Institute of Technology
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *     list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *     contributors may be used to endorse or promote products derived from
 *     this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef UFO_UTILITY_TIMER_HPP
#define UFO_UTILITY_TIMER_HPP

// STL
#include <chrono>
#include <cstdlib>

namespace ufo
{
class Timer
{
 public:
	void start();

	void reset();

	void stop();

	[[nodiscard]] bool active() const;

	[[nodiscard]] double currentSeconds() const;

	[[nodiscard]] double currentMilliseconds() const;

	[[nodiscard]] double currentMicroseconds() const;

	[[nodiscard]] double currentNanoseconds() const;

	[[nodiscard]] double lastSeconds() const;

	[[nodiscard]] double lastMilliseconds() const;

	[[nodiscard]] double lastMicroseconds() const;

	[[nodiscard]] double lastNanoseconds() const;

	[[nodiscard]] double totalSeconds() const;

	[[nodiscard]] double totalMilliseconds() const;

	[[nodiscard]] double totalMicroseconds() const;

	[[nodiscard]] double totalNanoseconds() const;

	[[nodiscard]] double minSeconds() const;

	[[nodiscard]] double minMilliseconds() const;

	[[nodiscard]] double minMicroseconds() const;

	[[nodiscard]] double minNanoseconds() const;

	[[nodiscard]] double maxSeconds() const;

	[[nodiscard]] double maxMilliseconds() const;

	[[nodiscard]] double maxMicroseconds() const;

	[[nodiscard]] double maxNanoseconds() const;

	[[nodiscard]] double meanSeconds() const;

	[[nodiscard]] double meanMilliseconds() const;

	[[nodiscard]] double meanMicroseconds() const;

	[[nodiscard]] double meanNanoseconds() const;

	[[nodiscard]] double varianceSeconds() const;

	[[nodiscard]] double varianceMilliseconds() const;

	[[nodiscard]] double varianceMicroseconds() const;

	[[nodiscard]] double varianceNanoseconds() const;

	[[nodiscard]] double stdSeconds() const;

	[[nodiscard]] double stdMilliseconds() const;

	[[nodiscard]] double stdMicroseconds() const;

	[[nodiscard]] double stdNanoseconds() const;

	[[nodiscard]] double sampleVarianceSeconds() const;

	[[nodiscard]] double sampleVarianceMilliseconds() const;

	[[nodiscard]] double sampleVarianceMicroseconds() const;

	[[nodiscard]] double sampleVarianceNanoseconds() const;

	[[nodiscard]] double populationVarianceSeconds() const;

	[[nodiscard]] double populationVarianceMilliseconds() const;

	[[nodiscard]] double populationVarianceMicroseconds() const;

	[[nodiscard]] double populationVarianceNanoseconds() const;

	[[nodiscard]] std::size_t numSamples() const;

 private:
	[[nodiscard]] std::chrono::high_resolution_clock::duration current() const;

 private:
	std::chrono::time_point<std::chrono::high_resolution_clock> start_ = {};

	std::size_t                                  samples_ = 0;
	std::chrono::high_resolution_clock::duration last_ =
	    std::chrono::high_resolution_clock::duration::zero();
	std::chrono::high_resolution_clock::duration total_ =
	    std::chrono::high_resolution_clock::duration::zero();
	std::chrono::duration<double, std::chrono::high_resolution_clock::period> mean_ =
	    std::chrono::duration<double, std::chrono::high_resolution_clock::period>::zero();
	// std::chrono::duration<double> variance_ = std::chrono::duration<double>::zero();
	double                                       variance_ = 0.0;
	std::chrono::high_resolution_clock::duration min_ =
	    std::chrono::high_resolution_clock::duration::max();
	std::chrono::high_resolution_clock::duration max_ =
	    std::chrono::high_resolution_clock::duration::min();
};
}  // namespace ufo

#endif  // UFO_UTILITY_TIMER_HPP