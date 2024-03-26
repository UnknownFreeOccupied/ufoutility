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
#include <array>
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

	static constexpr char const* resetColor() { return "\033[0m"; }
	static constexpr char const* blackColor() { return "\033[30m"; }
	static constexpr char const* redColor() { return "\033[31m"; }
	static constexpr char const* greenColor() { return "\033[32m"; }
	static constexpr char const* yellowColor() { return "\033[33m"; }
	static constexpr char const* blueColor() { return "\033[34m"; }
	static constexpr char const* magentaColor() { return "\033[35m"; }
	static constexpr char const* cyanColor() { return "\033[36m"; }
	static constexpr char const* whiteColor() { return "\033[37m"; }
	static constexpr char const* boldBlackColor() { return "\033[1m\033[30m"; }
	static constexpr char const* boldRedColor() { return "\033[1m\033[31m"; }
	static constexpr char const* boldGreenColor() { return "\033[1m\033[32m"; }
	static constexpr char const* boldYellowColor() { return "\033[1m\033[33m"; }
	static constexpr char const* boldBlueColor() { return "\033[1m\033[34m"; }
	static constexpr char const* boldMagentaColor() { return "\033[1m\033[35m"; }
	static constexpr char const* boldCyanColor() { return "\033[1m\033[36m"; }
	static constexpr char const* boldWhiteColor() { return "\033[1m\033[37m"; }

	template <class Period = std::chrono::seconds::period>
	void print(bool first_as_tag = false, bool random_colors = false, bool bold = false,
	           std::size_t start_numbering_level = 1,
	           std::size_t stop_numbering_level  = std::numeric_limits<std::size_t>::max(),
	           std::size_t group_colors_level    = std::numeric_limits<std::size_t>::max(),
	           int         precision             = 4) const
	{
		static constexpr std::array const RC{redColor(),  greenColor(),   yellowColor(),
		                                     blueColor(), magentaColor(), cyanColor(),
		                                     whiteColor()};

		std::array<std::string, 8> label{"Component", "Total", "Last", "Mean",
		                                 "StDev",     "Min",   "Max",  "Samples"};
		std::array                 width{
        std::max(static_cast<int>(label[0].length()),
		                             longestTag(first_as_tag, first_as_tag ? -1 : 0, start_numbering_level,
		                                        stop_numbering_level)),
        std::max(static_cast<int>(label[1].length()),
		                             longestTotal<Period>(first_as_tag, precision)),
        std::max(static_cast<int>(label[2].length()),
		                             longestLast<Period>(first_as_tag, precision)),
        std::max(static_cast<int>(label[3].length()),
		                             longestMean<Period>(first_as_tag, precision)),
        std::max(static_cast<int>(label[4].length()),
		                             longestStd<Period>(first_as_tag, precision)),
        std::max(static_cast<int>(label[5].length()),
		                             longestMin<Period>(first_as_tag, precision)),
        std::max(static_cast<int>(label[6].length()),
		                             longestMax<Period>(first_as_tag, precision)),
        std::max(static_cast<int>(label[7].length()), longestNumSamples(first_as_tag))};

		std::array<int, width.size()> left_pad;
		std::array<int, width.size()> right_pad;
		for (std::size_t i{}; label.size() != i; ++i) {
			left_pad[i]  = std::ceil((width[i] - static_cast<int>(label[i].length())) / 2.0);
			right_pad[i] = std::ceil((width[i] - static_cast<int>(label[i].length())) / 2.0);
		}

		if (first_as_tag) {
			printf("%s timings", tag().c_str());
		} else {
			printf("Timings");
		}

		if constexpr (std::is_same_v<Period, std::chrono::nanoseconds::period>) {
			printf(" in nanoseconds (ns)\n");
		} else if constexpr (std::is_same_v<Period, std::chrono::microseconds::period>) {
			printf(" in microseconds (µs)\n");
		} else if constexpr (std::is_same_v<Period, std::chrono::milliseconds::period>) {
			printf(" in milliseconds (ms)\n");
		} else if constexpr (std::is_same_v<Period, std::chrono::seconds::period>) {
			printf(" in seconds (s)\n");
		} else if constexpr (std::is_same_v<Period, std::chrono::minutes::period>) {
			printf(" in minutes (min)\n");
		} else if constexpr (std::is_same_v<Period, std::chrono::hours::period>) {
			printf(" in hours (h)\n");
		} else {
			printf(" [PERIOD NOT SUPPORTED]\n");
		}

		printf("%*s%s%*s", left_pad[0], "", label[0].c_str(), right_pad[0], "");
		for (std::size_t i{1}; label.size() != i; ++i) {
			printf("\t%*s%s%*s", left_pad[i], "", label[i].c_str(), right_pad[i], "");
		}
		printf("\n");

		if (first_as_tag) {
			std::size_t i{};
			printRecurs<Period>(0, i, width[0], random_colors, bold, group_colors_level,
			                    precision, start_numbering_level, stop_numbering_level);
		} else {
			printf("%s%s%-*s\t%.*f\t%.*f\t%.*f\t%.*f\t%.*f\t%.*f\t%lu%s\n",
			       bold ? "\033[1m" : "", random_colors ? RC[0] : color().c_str(), width[0],
			       tag().c_str(), precision, total<Period>(), precision, last<Period>(),
			       precision, mean<Period>(), precision, std<Period>(), precision,
			       min<Period>(), precision, max<Period>(), numSamples(), resetColor());

			std::size_t i{};
			printRecurs<Period>(1, i, width[0], random_colors, bold, group_colors_level,
			                    precision, start_numbering_level, stop_numbering_level);
		}
	}

	void printSeconds(
	    bool first_as_tag = false, bool random_colors = false, bool bold = false,
	    std::size_t start_numbering_level = 1,
	    std::size_t stop_numbering_level  = std::numeric_limits<std::size_t>::max(),
	    std::size_t group_colors_level    = std::numeric_limits<std::size_t>::max(),
	    int         precision             = 4) const;

	void printMilliseconds(
	    bool first_as_tag = false, bool random_colors = false, bool bold = false,
	    std::size_t start_numbering_level = 1,
	    std::size_t stop_numbering_level  = std::numeric_limits<std::size_t>::max(),
	    std::size_t group_colors_level    = std::numeric_limits<std::size_t>::max(),
	    int         precision             = 4) const;

	void printMicroseconds(
	    bool first_as_tag = false, bool random_colors = false, bool bold = false,
	    std::size_t start_numbering_level = 1,
	    std::size_t stop_numbering_level  = std::numeric_limits<std::size_t>::max(),
	    std::size_t group_colors_level    = std::numeric_limits<std::size_t>::max(),
	    int         precision             = 4) const;

	void printNanoseconds(
	    bool first_as_tag = false, bool random_colors = false, bool bold = false,
	    std::size_t start_numbering_level = 1,
	    std::size_t stop_numbering_level  = std::numeric_limits<std::size_t>::max(),
	    std::size_t group_colors_level    = std::numeric_limits<std::size_t>::max(),
	    int         precision             = 4) const;

 private:
	template <class Period = std::chrono::seconds::period>
	void printRecurs(int level, std::size_t& i, int component_width, bool random_colors,
	                 bool bold, std::size_t group_colors_level, int precision,
	                 std::size_t start_numbering_level,
	                 std::size_t stop_numbering_level) const
	{
		static constexpr std::array const RC{redColor(),  greenColor(),   yellowColor(),
		                                     blueColor(), magentaColor(), cyanColor(),
		                                     whiteColor()};

		for (auto const& [n, t] : timer_) {
			i += level <= group_colors_level;
			std::string tag;
			// FIXME: Should it be < or <=?
			if (start_numbering_level <= level && level <= stop_numbering_level) {
				tag = std::string(2 * level, ' ') + std::to_string(n) + ". " + t.tag();
			} else {
				tag = std::string(2 * level, ' ') + t.tag();
			}

			printf("%s%s%-*s\t%.*f\t%.*f\t%.*f\t%.*f\t%.*f\t%.*f\t%lu%s\n",
			       bold ? "\033[1m" : "", random_colors ? RC[i % RC.size()] : t.color().c_str(),
			       component_width, tag.c_str(), precision, t.template total<Period>(),
			       precision, t.template last<Period>(), precision, t.template mean<Period>(),
			       precision, t.template std<Period>(), precision, t.template min<Period>(),
			       precision, t.template max<Period>(), t.numSamples(), resetColor());
			t.template printRecurs<Period>(level + 1, i, component_width, random_colors, bold,
			                               group_colors_level, precision, start_numbering_level,
			                               stop_numbering_level);
		}
	}

	int longestTag(bool skip_this, int level, std::size_t start_numbering_level,
	               std::size_t stop_numbering_level) const;

	int longestTagHelper(std::size_t n, int level, std::size_t start_numbering_level,
	                     std::size_t stop_numbering_level) const;

	template <class Period = std::chrono::seconds::period>
	int longestTotal(bool skip_this, int precision) const
	{
		int l = 0;

		if (!skip_this) {
			auto t = total<Period>();
			if (std::isnan(t)) {
				l = 3;
			} else {
				l               = precision + 1 + static_cast<int>(0.0 > t);
				std::uint64_t u = std::abs(static_cast<std::int64_t>(t));
				for (; 0 < u; u /= 10) {
					++l;
				}
			}
		}

		for (auto const& [_, t] : timer_) {
			l = std::max(l, t.template longestTotal<Period>(false, precision));
		}

		return l;
	}

	template <class Period = std::chrono::seconds::period>
	int longestLast(bool skip_this, int precision) const
	{
		int l = 0;

		if (!skip_this) {
			auto t = last<Period>();
			if (std::isnan(t)) {
				l = 3;
			} else {
				l               = precision + 1 + static_cast<int>(0.0 > t);
				std::uint64_t u = std::abs(static_cast<std::int64_t>(t));
				for (; 0 < u; u /= 10) {
					++l;
				}
			}
		}

		for (auto const& [_, t] : timer_) {
			l = std::max(l, t.template longestLast<Period>(false, precision));
		}

		return l;
	}

	template <class Period = std::chrono::seconds::period>
	int longestMean(bool skip_this, int precision) const
	{
		int l = 0;

		if (!skip_this) {
			auto t = mean<Period>();
			if (std::isnan(t)) {
				l = 3;
			} else {
				l               = precision + 1 + static_cast<int>(0.0 > t);
				std::uint64_t u = std::abs(static_cast<std::int64_t>(t));
				for (; 0 < u; u /= 10) {
					++l;
				}
			}
		}

		for (auto const& [_, t] : timer_) {
			l = std::max(l, t.template longestMean<Period>(false, precision));
		}

		return l;
	}

	template <class Period = std::chrono::seconds::period>
	int longestStd(bool skip_this, int precision) const
	{
		int l = 0;

		if (!skip_this) {
			auto t = std<Period>();
			if (std::isnan(t)) {
				l = 3;
			} else {
				l               = precision + 1 + static_cast<int>(0.0 > t);
				std::uint64_t u = std::abs(static_cast<std::int64_t>(t));
				for (; 0 < u; u /= 10) {
					++l;
				}
			}
		}

		for (auto const& [_, t] : timer_) {
			l = std::max(l, t.template longestStd<Period>(false, precision));
		}

		return l;
	}

	template <class Period = std::chrono::seconds::period>
	int longestMin(bool skip_this, int precision) const
	{
		int l = 0;

		if (!skip_this) {
			auto t = min<Period>();
			if (std::isnan(t)) {
				l = 3;
			} else {
				l               = precision + 1 + static_cast<int>(0.0 > t);
				std::uint64_t u = std::abs(static_cast<std::int64_t>(t));
				for (; 0 < u; u /= 10) {
					++l;
				}
			}
		}

		for (auto const& [_, t] : timer_) {
			l = std::max(l, t.template longestMin<Period>(false, precision));
		}

		return l;
	}

	template <class Period = std::chrono::seconds::period>
	int longestMax(bool skip_this, int precision) const
	{
		int l = 0;

		if (!skip_this) {
			auto t = max<Period>();
			if (std::isnan(t)) {
				l = 3;
			} else {
				l               = precision + 1 + static_cast<int>(0.0 > t);
				std::uint64_t u = std::abs(static_cast<std::int64_t>(t));
				for (; 0 < u; u /= 10) {
					++l;
				}
			}
		}

		for (auto const& [_, t] : timer_) {
			l = std::max(l, t.template longestMax<Period>(false, precision));
		}

		return l;
	}

	int longestNumSamples(bool skip_this) const;

 private:
	std::map<std::size_t, Timing> timer_;
	std::string                   tag_;
	std::string                   color_;
};
}  // namespace ufo

#endif  // UFO_UTILITY_TIMING_HPP