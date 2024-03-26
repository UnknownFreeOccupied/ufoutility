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

#ifndef UFO_UTILITY_WRITE_BUFFER_HPP
#define UFO_UTILITY_WRITE_BUFFER_HPP

// STL
#include <cstddef>
#include <cstdint>
#include <istream>

namespace ufo
{
class WriteBuffer
{
 public:
	virtual ~WriteBuffer();

	template <typename T>
	WriteBuffer& write(T const& t)
	{
		return write(&t, sizeof(t));
	}

	virtual WriteBuffer& write(void const* src, std::size_t count);

	virtual WriteBuffer& write(std::istream& in, std::size_t count);

	virtual void reserve(std::size_t new_cap);

	virtual void resize(std::size_t new_size);

	virtual void clear();

	[[nodiscard]] virtual std::uint8_t* data();

	[[nodiscard]] virtual std::uint8_t const* data() const;

	[[nodiscard]] virtual bool empty() const;

	[[nodiscard]] virtual std::size_t size() const;

	[[nodiscard]] std::size_t capacity() const noexcept;

	[[nodiscard]] std::size_t writeIndex() const noexcept;

	void skipWrite(std::size_t count) noexcept;

	void setWriteIndex(std::size_t index) noexcept;

	[[nodiscard]] std::size_t writeLeft() const noexcept;

 protected:
	std::uint8_t* data_  = nullptr;
	std::size_t   size_  = 0;
	std::size_t   cap_   = 0;
	std::size_t   index_ = 0;
};
}  // namespace ufo
#endif  // UFO_UTILITY_WRITE_BUFFER_HPP