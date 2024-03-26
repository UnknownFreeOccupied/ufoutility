// UFO
#include <ufo/utility/timing.hpp>

// STL
#include <array>
#include <cmath>

namespace ufo
{
//
// Public functions
//

Timing::Timing(std::string const& tag) : tag_(tag) {}

Timing::Timing(std::string const&                                          tag,
               std::initializer_list<std::pair<std::size_t const, Timing>> init)
    : tag_(tag), timer_(init)
{
}

void Timing::start(std::string const& tag)
{
	Timer::start();
	tag_ = tag;
}

void Timing::start(std::string const& tag, std::string const& color)
{
	start(tag);
	color_ = color;
}

Timing const& Timing::operator[](std::size_t num) const { return timer_.at(num); }

Timing& Timing::operator[](std::size_t num) { return timer_[num]; }

std::string const& Timing::tag() const { return tag_; }

void Timing::setTag(std::string const& tag) { tag_ = tag; }

std::string const& Timing::color() const { return color_; }

void Timing::setColor(std::string const& color) { color_ = color; }

constexpr char const* Timing::resetColor() { return "\033[0m"; }
constexpr char const* Timing::blackColor() { return "\033[30m"; }
constexpr char const* Timing::redColor() { return "\033[31m"; }
constexpr char const* Timing::greenColor() { return "\033[32m"; }
constexpr char const* Timing::yellowColor() { return "\033[33m"; }
constexpr char const* Timing::blueColor() { return "\033[34m"; }
constexpr char const* Timing::magentaColor() { return "\033[35m"; }
constexpr char const* Timing::cyanColor() { return "\033[36m"; }
constexpr char const* Timing::whiteColor() { return "\033[37m"; }
constexpr char const* Timing::boldBlackColor() { return "\033[1m\033[30m"; }
constexpr char const* Timing::boldRedColor() { return "\033[1m\033[31m"; }
constexpr char const* Timing::boldGreenColor() { return "\033[1m\033[32m"; }
constexpr char const* Timing::boldYellowColor() { return "\033[1m\033[33m"; }
constexpr char const* Timing::boldBlueColor() { return "\033[1m\033[34m"; }
constexpr char const* Timing::boldMagentaColor() { return "\033[1m\033[35m"; }
constexpr char const* Timing::boldCyanColor() { return "\033[1m\033[36m"; }
constexpr char const* Timing::boldWhiteColor() { return "\033[1m\033[37m"; }

void Timing::print(bool first_as_tag, bool random_colors, bool bold,
                   std::size_t start_numbering_level, std::size_t stop_numbering_level,
                   std::size_t group_colors_level, int precision) const
{
	printSeconds(first_as_tag, random_colors, bold, start_numbering_level,
	             stop_numbering_level, group_colors_level, precision);
}

void Timing::printSeconds(bool first_as_tag, bool random_colors, bool bold,
                          std::size_t start_numbering_level,
                          std::size_t stop_numbering_level,
                          std::size_t group_colors_level, int precision) const
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
	                             longestTotal(first_as_tag, precision)),
      std::max(static_cast<int>(label[2].length()), longestLast(first_as_tag, precision)),
      std::max(static_cast<int>(label[3].length()), longestMean(first_as_tag, precision)),
      std::max(static_cast<int>(label[4].length()), longestStd(first_as_tag, precision)),
      std::max(static_cast<int>(label[5].length()), longestMin(first_as_tag, precision)),
      std::max(static_cast<int>(label[6].length()), longestMax(first_as_tag, precision)),
      std::max(static_cast<int>(label[7].length()), longestSteps(first_as_tag))};

	std::array<int, width.size()> left_pad;
	std::array<int, width.size()> right_pad;
	for (std::size_t i{}; label.size() != i; ++i) {
		left_pad[i]  = std::ceil((width[i] - static_cast<int>(label[i].length())) / 2.0);
		right_pad[i] = std::ceil((width[i] - static_cast<int>(label[i].length())) / 2.0);
	}

	if (first_as_tag) {
		printf("%s timings in seconds (s)\n", tag().c_str());
	} else {
		printf("Timings in seconds (s)\n");
	}

	printf("%*s%s%*s", left_pad[0], "", label[0].c_str(), right_pad[0], "");
	for (std::size_t i{1}; label.size() != i; ++i) {
		printf("\t%*s%s%*s", left_pad[i], "", label[i].c_str(), right_pad[i], "");
	}
	printf("\n");

	if (first_as_tag) {
		std::size_t i{};
		printSecondsRecurs(0, i, width[0], random_colors, bold, group_colors_level, precision,
		                   start_numbering_level, stop_numbering_level);
	} else {
		printf("%s%s%-*s\t%.*f\t%.*f\t%.*f\t%.*f\t%.*f\t%.*f\t%lu%s\n", bold ? "\033[1m" : "",
		       random_colors ? RC[0] : color().c_str(), width[0], tag().c_str(), precision,
		       totalSeconds(), precision, lastSeconds(), precision, meanSeconds(), precision,
		       stdSeconds(), precision, minSeconds(), precision, maxSeconds(), numSamples(),
		       resetColor());

		std::size_t i{};
		printSecondsRecurs(1, i, width[0], random_colors, bold, group_colors_level, precision,
		                   start_numbering_level, stop_numbering_level);
	}
}

// void Timing::printMilliseconds(bool first_as_tag, bool random_colors, bool bold,
//                                std::size_t group_colors_level, int precision) const
// {
// 	// TODO: Implement
// }

// void Timing::printMicroseconds(bool first_as_tag, bool random_colors, bool bold,
//                                std::size_t group_colors_level, int precision) const
// {
// 	// TODO: Implement
// }

// void Timing::printNanoseconds(bool first_as_tag, bool random_colors, bool bold,
//                               std::size_t group_colors_level, int precision) const
// {
// 	// TODO: Implement
// }

//
// Private functions
//

void Timing::printSecondsRecurs(int level, std::size_t& i, int component_width,
                                bool random_colors, bool bold,
                                std::size_t group_colors_level, int precision,
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

		printf("%s%s%-*s\t%.*f\t%.*f\t%.*f\t%.*f\t%.*f\t%.*f\t%lu%s\n", bold ? "\033[1m" : "",
		       random_colors ? RC[i % RC.size()] : t.color().c_str(), component_width,
		       tag.c_str(), precision, t.totalSeconds(), precision, t.lastSeconds(),
		       precision, t.meanSeconds(), precision, t.stdSeconds(), precision,
		       t.minSeconds(), precision, t.maxSeconds(), t.numSamples(), resetColor());
		t.printSecondsRecurs(level + 1, i, component_width, random_colors, bold,
		                     group_colors_level, precision, start_numbering_level,
		                     stop_numbering_level);
	}
}

int Timing::longestTag(bool skip_this, int level, std::size_t start_numbering_level,
                       std::size_t stop_numbering_level) const
{
	int l = skip_this ? 0 : tag().length();

	for (auto const& [n, t] : timer_) {
		l = std::max(
		    l, t.longestTagHelper(n, level + 1, start_numbering_level, stop_numbering_level));
	}
	return l;
}

int Timing::longestTagHelper(std::size_t n, int level, std::size_t start_numbering_level,
                             std::size_t stop_numbering_level) const
{
	int l = 4 * level + tag().length();

	// FIXME: Should it be < or <=?
	if (start_numbering_level <= level && level <= stop_numbering_level) {
		l += 0 > n;
		for (; 0 < n; n /= 10) {
			++l;
		}
	}

	for (auto const& [n, t] : timer_) {
		l = std::max(
		    l, t.longestTagHelper(n, level + 1, start_numbering_level, stop_numbering_level));
	}
	return l;
}

int Timing::longestTotal(bool skip_this, int precision) const
{
	int l = 0;

	if (!skip_this) {
		auto t = totalSeconds();
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
		l = std::max(l, t.longestTotal(false, precision));
	}

	return l;
}

int Timing::longestLast(bool skip_this, int precision) const
{
	int l = 0;

	if (!skip_this) {
		auto t = lastSeconds();
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
		l = std::max(l, t.longestLast(false, precision));
	}

	return l;
}

int Timing::longestMean(bool skip_this, int precision) const
{
	int l = 0;

	if (!skip_this) {
		auto t = meanSeconds();
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
		l = std::max(l, t.longestMean(false, precision));
	}

	return l;
}

int Timing::longestStd(bool skip_this, int precision) const
{
	int l = 0;

	if (!skip_this) {
		auto t = stdSeconds();
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
		l = std::max(l, t.longestStd(false, precision));
	}

	return l;
}

int Timing::longestMin(bool skip_this, int precision) const
{
	int l = 0;

	if (!skip_this) {
		auto t = minSeconds();
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
		l = std::max(l, t.longestMin(false, precision));
	}

	return l;
}

int Timing::longestMax(bool skip_this, int precision) const
{
	int l = 0;

	if (!skip_this) {
		auto t = maxSeconds();
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
		l = std::max(l, t.longestMax(false, precision));
	}

	return l;
}

int Timing::longestSteps(bool skip_this) const
{
	int l = 0;

	if (!skip_this) {
		l = 1;
		for (auto u = numSamples(); 0 < u; u /= 10) {
			++l;
		}
	}

	for (auto const& [_, t] : timer_) {
		l = std::max(l, t.longestSteps(false));
	}
	return l;
}
}  // namespace ufo