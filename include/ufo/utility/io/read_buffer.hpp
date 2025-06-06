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

#ifndef UFO_UTILITY_READ_BUFFER_HPP
#define UFO_UTILITY_READ_BUFFER_HPP

// STL
#include <cstddef>
#include <cstdint>
#include <ostream>

namespace ufo
{
class ReadBuffer
{
 public:
	using size_type = std::size_t;

	ReadBuffer()                  = default;
	ReadBuffer(ReadBuffer const&) = default;
	ReadBuffer(ReadBuffer&&)      = default;

	ReadBuffer(std::byte const* data, size_type count);

	virtual ~ReadBuffer() = default;

	ReadBuffer& operator=(ReadBuffer const&) = default;
	ReadBuffer& operator=(ReadBuffer&&)      = default;

	template <class T>
	ReadBuffer& read(T& t)
	{
		return read(&t, sizeof(t));
	}

	ReadBuffer& read(void* dest, size_type count);

	ReadBuffer& read(std::ostream& out, size_type count);

	bool readLine(std::string& line);

	template <class T>
	ReadBuffer& readUnsafe(T& t)
	{
		return readUnsafe(&t, sizeof(t));
	}

	ReadBuffer& readUnsafe(void* dest, size_type count);

	ReadBuffer& readUnsafe(std::ostream& out, size_type count);

	[[nodiscard]] virtual std::byte const* data() const;

	[[nodiscard]] virtual bool empty() const;

	[[nodiscard]] virtual size_type size() const;

	[[nodiscard]] size_type readPos() const noexcept;

	void readPos(size_type pos) noexcept;

	void readSkip(size_type count) noexcept;

	[[nodiscard]] size_type readLeft() const noexcept;

 protected:
	std::byte const* data_ = nullptr;
	size_type        size_{};
	size_type        pos_{};
};
}  // namespace ufo
#endif  // UFO_UTILITY_READ_BUFFER_HPP