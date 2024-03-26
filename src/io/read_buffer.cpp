// UFO
#include <ufo/utility/io/read_buffer.hpp>

// STL
#include <cstring>

namespace ufo
{
ReadBuffer::ReadBuffer(std::uint8_t const* data, std::size_t count)
    : data_(data), size_(count)
{
}

ReadBuffer& ReadBuffer::read(void* dest, std::size_t count)
{
	if (size() < index_ + count) {
		// TODO: Fill in exception message
		throw std::out_of_range("");
	}

	return readUnsafe(dest, count);
}

ReadBuffer& ReadBuffer::read(std::ostream& out, std::size_t count)
{
	if (size() < index_ + count) {
		// TODO: Fill in exception message
		throw std::out_of_range("");
	}

	return readUnsafe(out, count);
}

ReadBuffer& ReadBuffer::readUnsafe(void* dest, std::size_t count)
{
	std::memmove(dest, data_ + index_, count);
	index_ += count;
	return *this;
}

ReadBuffer& ReadBuffer::readUnsafe(std::ostream& out, std::size_t count)
{
	out.write(reinterpret_cast<char const*>(data_), static_cast<std::streamsize>(count));
	index_ += count;
	return *this;
}

std::uint8_t const* ReadBuffer::data() const { return data_; }

bool ReadBuffer::empty() const { return 0 == size(); }

std::size_t ReadBuffer::size() const { return size_; }

std::size_t ReadBuffer::readIndex() const noexcept { return index_; }

void ReadBuffer::skipRead(std::size_t count) noexcept { index_ += count; }

void ReadBuffer::setReadIndex(std::size_t index) noexcept { index_ = index; }

std::size_t ReadBuffer::readLeft() const noexcept
{
	return size_ < index_ ? 0 : index_ - size_;
}
}  // namespace ufo