// UFO
#include <ufo/utility/io/write_buffer.hpp>

// STL
#include <cstring>

namespace ufo
{
WriteBuffer::~WriteBuffer()
{
	if (data_) {
		free(data_);
	}
}

WriteBuffer& WriteBuffer::write(void const* src, std::size_t count)
{
	if (capacity() < index_ + count) {
		reserve(index_ + count);
	}

	std::memmove(data_ + index_, src, count);

	index_ += count;
	size_ = std::max(size_, index_);

	return *this;
}

WriteBuffer& WriteBuffer::write(std::istream& in, std::size_t count)
{
	if (capacity() < index_ + count) {
		reserve(index_ + count);
	}

	in.read(reinterpret_cast<char*>(data_), static_cast<std::streamsize>(count));

	index_ += count;
	size_ = std::max(size_, index_);

	return *this;
}

void WriteBuffer::reserve(std::size_t new_cap)
{
	if (cap_ >= new_cap) {
		return;
	}

	if (auto p_new =
	        static_cast<std::uint8_t*>(realloc(data_, new_cap * sizeof(std::uint8_t)))) {
		data_ = p_new;
		cap_  = new_cap;
	} else {
		throw std::bad_alloc();
	}
}

void WriteBuffer::resize(std::size_t new_size)
{
	reserve(new_size);
	size_ = new_size;
}

void WriteBuffer::clear()
{
	size_  = 0;
	index_ = 0;
}

std::uint8_t* WriteBuffer::data() { return data_; }

std::uint8_t const* WriteBuffer::data() const { return data_; }

bool WriteBuffer::empty() const { return 0 == size(); }

std::size_t WriteBuffer::size() const { return size_; }

std::size_t WriteBuffer::capacity() const noexcept { return cap_; }

std::size_t WriteBuffer::writeIndex() const noexcept { return index_; }

void WriteBuffer::skipWrite(std::size_t count) noexcept { index_ += count; }

void WriteBuffer::setWriteIndex(std::size_t index) noexcept { index_ = index; }

std::size_t WriteBuffer::writeLeft() const noexcept
{
	return size_ < index_ ? 0 : index_ - size_;
}
}  // namespace ufo