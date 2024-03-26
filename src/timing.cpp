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

void Timing::printSeconds(bool first_as_tag, bool random_colors, bool bold,
                          std::size_t start_numbering_level,
                          std::size_t stop_numbering_level,
                          std::size_t group_colors_level, int precision) const
{
	print<std::chrono::seconds::period>(first_as_tag, random_colors, bold,
	                                    start_numbering_level, stop_numbering_level,
	                                    group_colors_level, precision);
}

void Timing::printMilliseconds(bool first_as_tag, bool random_colors, bool bold,
                               std::size_t start_numbering_level,
                               std::size_t stop_numbering_level,
                               std::size_t group_colors_level, int precision) const
{
	print<std::chrono::milliseconds::period>(first_as_tag, random_colors, bold,
	                                         start_numbering_level, stop_numbering_level,
	                                         group_colors_level, precision);
}

void Timing::printMicroseconds(bool first_as_tag, bool random_colors, bool bold,
                               std::size_t start_numbering_level,
                               std::size_t stop_numbering_level,
                               std::size_t group_colors_level, int precision) const
{
	print<std::chrono::microseconds::period>(first_as_tag, random_colors, bold,
	                                         start_numbering_level, stop_numbering_level,
	                                         group_colors_level, precision);
}

void Timing::printNanoseconds(bool first_as_tag, bool random_colors, bool bold,
                              std::size_t start_numbering_level,
                              std::size_t stop_numbering_level,
                              std::size_t group_colors_level, int precision) const
{
	print<std::chrono::nanoseconds::period>(first_as_tag, random_colors, bold,
	                                        start_numbering_level, stop_numbering_level,
	                                        group_colors_level, precision);
}

//
// Private functions
//

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

int Timing::longestNumSamples(bool skip_this) const
{
	int l = 0;

	if (!skip_this) {
		l = 1;
		for (auto u = numSamples(); 0 < u; u /= 10) {
			++l;
		}
	}

	for (auto const& [_, t] : timer_) {
		l = std::max(l, t.longestNumSamples(false));
	}
	return l;
}
}  // namespace ufo