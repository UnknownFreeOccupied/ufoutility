// UFO
#include <ufo/utility/io/buffer.hpp>

namespace ufo
{
Buffer::~Buffer() {}

Buffer& Buffer::write(void const* src, std::size_t count)
{
	WriteBuffer::write(src, count);
	ReadBuffer::size_ = WriteBuffer::size_;
	return *this;
}

Buffer& Buffer::write(std::istream& in, std::size_t count)
{
	WriteBuffer::write(in, count);
	ReadBuffer::size_ = WriteBuffer::size_;
	return *this;
}

void Buffer::clear()
{
	WriteBuffer::clear();
	ReadBuffer::size_  = 0;
	ReadBuffer::index_ = 0;
}

std::uint8_t* Buffer::data() { return WriteBuffer::data(); }

std::uint8_t const* Buffer::data() const { return WriteBuffer::data(); }

bool Buffer::empty() const { return WriteBuffer::empty(); }

std::size_t Buffer::size() const { return WriteBuffer::size(); }

void Buffer::reserve(std::size_t new_cap)
{
	WriteBuffer::reserve(new_cap);
	ReadBuffer::data_ = WriteBuffer::data_;
}

void Buffer::resize(std::size_t new_size)
{
	WriteBuffer::resize(new_size);
	ReadBuffer::data_ = WriteBuffer::data_;
}
}  // namespace ufo