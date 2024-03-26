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

#ifndef UFO_UTILITY_TIMING_HPP
#define UFO_UTILITY_TIMING_HPP

// UFO
#include <ufo/utility/timer.hpp>

// STL
#include <cstdlib>
#include <initializer_list>
#include <limits>
#include <map>
#include <string>

namespace ufo
{
class Timing : public Timer
{
 public:
	Timing() = default;

	Timing(std::string const& tag);

	Timing(std::string const&                                          tag,
	       std::initializer_list<std::pair<std::size_t const, Timing>> init);

	using Timer::start;

	void start(std::string const& tag);

	void start(std::string const& tag, std::string const& color);

	using Timer::reset;

	Timing const& operator[](std::size_t num) const;

	Timing& operator[](std::size_t num);

	std::string const& tag() const;

	void setTag(std::string const& tag);

	std::string const& color() const;

	void setColor(std::string const& color);

	static constexpr char const* resetColor();
	static constexpr char const* blackColor();
	static constexpr char const* redColor();
	static constexpr char const* greenColor();
	static constexpr char const* yellowColor();
	static constexpr char const* blueColor();
	static constexpr char const* magentaColor();
	static constexpr char const* cyanColor();
	static constexpr char const* whiteColor();
	static constexpr char const* boldBlackColor();
	static constexpr char const* boldRedColor();
	static constexpr char const* boldGreenColor();
	static constexpr char const* boldYellowColor();
	static constexpr char const* boldBlueColor();
	static constexpr char const* boldMagentaColor();
	static constexpr char const* boldCyanColor();
	static constexpr char const* boldWhiteColor();

	void print(bool first_as_tag = false, bool random_colors = false, bool bold = false,
	           std::size_t start_numbering_level = 1,
	           std::size_t stop_numbering_level  = std::numeric_limits<std::size_t>::max(),
	           std::size_t group_colors_level    = std::numeric_limits<std::size_t>::max(),
	           int         precision             = 4) const;

	void printSeconds(
	    bool first_as_tag = false, bool random_colors = false, bool bold = false,
	    std::size_t start_numbering_level = 1,
	    std::size_t stop_numbering_level  = std::numeric_limits<std::size_t>::max(),
	    std::size_t group_colors_level    = std::numeric_limits<std::size_t>::max(),
	    int         precision             = 4) const;

	// void printMilliseconds(
	//     bool first_as_tag = false, bool random_colors = false, bool bold = false,
	//     std::size_t group_colors_level = std::numeric_limits<std::size_t>::max(),
	//     int         precision          = 4) const;

	// void printMicroseconds(
	//     bool first_as_tag = false, bool random_colors = false, bool bold = false,
	//     std::size_t group_colors_level = std::numeric_limits<std::size_t>::max(),
	//     int         precision          = 4) const;

	// void printNanoseconds(
	//     bool first_as_tag = false, bool random_colors = false, bool bold = false,
	//     std::size_t group_colors_level = std::numeric_limits<std::size_t>::max(),
	//     int         precision          = 4) const;

 private:
	void printSecondsRecurs(int level, std::size_t& i, int component_width,
	                        bool random_colors, bool bold, std::size_t group_colors_level,
	                        int precision, std::size_t start_numbering_level,
	                        std::size_t stop_numbering_level) const;

	int longestTag(bool skip_this, int level, std::size_t start_numbering_level,
	               std::size_t stop_numbering_level) const;

	int longestTagHelper(std::size_t n, int level, std::size_t start_numbering_level,
	                     std::size_t stop_numbering_level) const;

	int longestTotal(bool skip_this, int precision) const;

	int longestLast(bool skip_this, int precision) const;

	int longestMean(bool skip_this, int precision) const;

	int longestStd(bool skip_this, int precision) const;

	int longestMin(bool skip_this, int precision) const;

	int longestMax(bool skip_this, int precision) const;

	int longestSteps(bool skip_this) const;

 private:
	std::map<std::size_t, Timing> timer_;
	std::string                   tag_;
	std::string                   color_;
};
}  // namespace ufo

#endif  // UFO_UTILITY_TIMING_HPP