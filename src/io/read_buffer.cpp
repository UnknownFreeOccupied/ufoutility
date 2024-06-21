// UFO
#include <ufo/utility/io/read_buffer.hpp>

// STL
#include <cstring>

namespace ufo
{
ReadBuffer::ReadBuffer(std::byte const* data, size_type count) : data_(data), size_(count)
{
}

ReadBuffer& ReadBuffer::read(void* dest, size_type count)
{
	if (size() < pos_ + count) {
		// TODO: Fill in exception message
		throw std::out_of_range("");
	}

	return readUnsafe(dest, count);
}

ReadBuffer& ReadBuffer::read(std::ostream& out, size_type count)
{
	if (size() < pos_ + count) {
		// TODO: Fill in exception message
		throw std::out_of_range("");
	}

	return readUnsafe(out, count);
}

ReadBuffer& ReadBuffer::readUnsafe(void* dest, size_type count)
{
	std::memmove(dest, data_ + pos_, count);
	pos_ += count;
	return *this;
}

ReadBuffer& ReadBuffer::readUnsafe(std::ostream& out, size_type count)
{
	out.write(reinterpret_cast<char const*>(data_), static_cast<std::streamsize>(count));
	pos_ += count;
	return *this;
}

std::byte const* ReadBuffer::data() const { return data_; }

bool ReadBuffer::empty() const { return 0 == size(); }

ReadBuffer::size_type ReadBuffer::size() const { return size_; }

ReadBuffer::size_type ReadBuffer::readPos() const noexcept { return pos_; }

void ReadBuffer::skipRead(size_type count) noexcept { pos_ += count; }

void ReadBuffer::setReadPos(size_type pos) noexcept { pos_ = pos; }

ReadBuffer::size_type ReadBuffer::readLeft() const noexcept
{
	return size_ < pos_ ? 0 : pos_ - size_;
}
}  // namespace ufo