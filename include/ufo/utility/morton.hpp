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

#ifndef UFO_UTILITY_MORTON_HPP
#define UFO_UTILITY_MORTON_HPP

// STL
#include <array>
#include <cstdint>

#if defined(UFO_BMI2)
#if defined(__i386__) || defined(__x86_64__) && (defined(__BMI2__) || defined(__AVX2__))
#include <immintrin.h>
#elif defined(__ARM_FEATURE_SIMD32) || defined(__ARM_NEON)
#include <arm_neon.h>
#endif
#endif

namespace ufo
{
static constexpr std::uint_fast64_t MORTON2_X_MASK = 0x5555555555555555;
static constexpr std::uint_fast64_t MORTON2_Y_MASK = 0xAAAAAAAAAAAAAAAA;

static constexpr std::uint_fast64_t MORTON3_X_MASK = 0x1249249249249249;
static constexpr std::uint_fast64_t MORTON3_Y_MASK = 0x2492492492492492;
static constexpr std::uint_fast64_t MORTON3_Z_MASK = 0x4924924924924924;

static constexpr std::uint_fast64_t MORTON4_X_MASK = 0x1111111111111111;
static constexpr std::uint_fast64_t MORTON4_Y_MASK = 0x2222222222222222;
static constexpr std::uint_fast64_t MORTON4_Z_MASK = 0x4444444444444444;
static constexpr std::uint_fast64_t MORTON4_W_MASK = 0x8888888888888888;

[[nodiscard]] constexpr std::uint_fast64_t mortonSpread2(std::uint_fast32_t v)
{
#if defined(UFO_BMI2)
	return _pdep_u64(static_cast<std::uint_fast64_t>(x), MORTON2_X_MASK);
#else
	std::uint_fast64_t x(v);
	x &= 0xFFFFFFFF;
	x = (x | (x << 16)) & 0x0000FFFF0000FFFF;
	x = (x | (x << 8)) & 0x00FF00FF00FF00FF;
	x = (x | (x << 4)) & 0x0F0F0F0F0F0F0F0F;
	x = (x | (x << 2)) & 0x3333333333333333;
	x = (x | (x << 1)) & 0x5555555555555555;
	return x;
#endif
}

[[nodiscard]] constexpr std::uint_fast64_t mortonSpread3(std::uint_fast32_t v)
{
#if defined(UFO_BMI2)
	return _pdep_u64(static_cast<std::uint_fast64_t>(x), MORTON3_X_MASK);
#else
	std::uint_fast64_t x(v);
	x &= 0x1FFFFF;
	x = (x | x << 32) & 0x1F00000000FFFF;
	x = (x | x << 16) & 0x1F0000FF0000FF;
	x = (x | x << 8) & 0x100F00F00F00F00F;
	x = (x | x << 4) & 0x10C30C30C30C30C3;
	x = (x | x << 2) & 0x1249249249249249;
	return x;
#endif
}

[[nodiscard]] constexpr std::uint_fast64_t mortonSpread4(std::uint_fast16_t v)
{
#if defined(UFO_BMI2)
	return _pdep_u64(static_cast<std::uint_fast64_t>(x), MORTON4_X_MASK);
#else
	std::uint_fast64_t x(v);
	x &= 0xFFFF;
	x = (x | (x << 32)) & 0xF800000007FF;
	x = (x | (x << 16)) & 0xF80007C0003F;
	x = (x | (x << 8)) & 0xC0380700C03807;
	x = (x | (x << 4)) & 0x843084308430843;
	x = (x | (x << 2)) & 0x909090909090909;
	x = (x | (x << 1)) & 0x1111111111111111;
	return x;
#endif
}

[[nodiscard]] constexpr std::uint_fast32_t mortonCompact2(std::uint_fast64_t x)
{
#if defined(UFO_BMI2)
	return static_cast<std::uint_fast32_t>(_pext_u64(x, MORTON2_X_MASK));
#else
	x &= 0x5555555555555555;
	x = (x ^ (x >> 1)) & 0x3333333333333333;
	x = (x ^ (x >> 2)) & 0x0F0F0F0F0F0F0F0F;
	x = (x ^ (x >> 4)) & 0x00FF00FF00FF00FF;
	x = (x ^ (x >> 8)) & 0x0000FFFF0000FFFF;
	x = (x ^ (x >> 16)) & 0x00000000FFFFFFFF;
	return static_cast<std::uint_fast32_t>(x);
#endif
}

[[nodiscard]] constexpr std::uint_fast32_t mortonCompact3(std::uint_fast64_t x)
{
#if defined(UFO_BMI2)
	return static_cast<std::uint_fast32_t>(_pext_u64(x, MORTON3_X_MASK));
#else
	x &= 0x1249249249249249;
	x = (x ^ (x >> 2)) & 0x10C30C30C30C30C3;
	x = (x ^ (x >> 4)) & 0x100F00F00F00F00F;
	x = (x ^ (x >> 8)) & 0x1F0000FF0000FF;
	x = (x ^ (x >> 16)) & 0x1F00000000FFFF;
	x = (x ^ (x >> 32)) & 0x1FFFFF;
	return static_cast<std::uint_fast32_t>(x);
#endif
}

[[nodiscard]] constexpr std::uint_fast16_t mortonCompact4(std::uint_fast64_t x)
{
#if defined(UFO_BMI2)
	return static_cast<std::uint_fast32_t>(_pext_u64(x, MORTON4_X_MASK));
#else
	x &= 0x1111111111111111;
	x = (x ^ (x >> 1)) & 0x909090909090909;
	x = (x ^ (x >> 2)) & 0x843084308430843;
	x = (x ^ (x >> 4)) & 0xC0380700C03807;
	x = (x ^ (x >> 8)) & 0xF80007C0003F;
	x = (x ^ (x >> 16)) & 0xF800000007FF;
	x = (x ^ (x >> 32)) & 0xFFFF;
	return static_cast<std::uint_fast16_t>(x);
#endif
}

[[nodiscard]] constexpr std::uint_fast64_t mortonEncode2(std::uint_fast32_t x,
                                                         std::uint_fast32_t y)
{
#if defined(UFO_BMI2)
	return _pdep_u64(static_cast<std::uint_fast64_t>(x), MORTON2_X_MASK) |
	       _pdep_u64(static_cast<std::uint_fast64_t>(y), MORTON2_Y_MASK);
#else
	return mortonSpread2(x) | (mortonSpread2(y) << 1);
#endif
}

[[nodiscard]] constexpr std::uint_fast64_t mortonEncode3(std::uint_fast32_t x,
                                                         std::uint_fast32_t y,
                                                         std::uint_fast32_t z)
{
#if defined(UFO_BMI2)
	return _pdep_u64(static_cast<std::uint_fast64_t>(x), MORTON3_X_MASK) |
	       _pdep_u64(static_cast<std::uint_fast64_t>(y), MORTON3_Y_MASK) |
	       _pdep_u64(static_cast<std::uint_fast64_t>(z), MORTON3_Z_MASK);
#else
	return mortonSpread3(x) | (mortonSpread3(y) << 1) | (mortonSpread3(z) << 2);
#endif
}

[[nodiscard]] constexpr std::uint_fast64_t mortonEncode4(std::uint_fast16_t x,
                                                         std::uint_fast16_t y,
                                                         std::uint_fast16_t z,
                                                         std::uint_fast16_t w)
{
#if defined(UFO_BMI2)
	return _pdep_u64(static_cast<std::uint_fast64_t>(x), MORTON4_X_MASK) |
	       _pdep_u64(static_cast<std::uint_fast64_t>(y), MORTON4_Y_MASK) |
	       _pdep_u64(static_cast<std::uint_fast64_t>(z), MORTON4_Z_MASK) |
	       _pdep_u64(static_cast<std::uint_fast64_t>(w), MORTON4_W_MASK);
#else
	return mortonSpread4(x) | (mortonSpread4(y) << 1) | (mortonSpread4(z) << 2) |
	       (mortonSpread4(w) << 3);
#endif
}

[[nodiscard]] constexpr std::array<std::uint_fast32_t, 2> mortonDecode2(
    std::uint_fast64_t x)
{
#if defined(UFO_BMI2)
	return {static_cast<std::uint_fast32_t>(_pext_u64(x, MORTON2_X_MASK)),
	        static_cast<std::uint_fast32_t>(_pext_u64(x, MORTON2_Y_MASK))};
#else
	return {mortonCompact2(x), mortonCompact2(x >> 1)};
#endif
}

[[nodiscard]] constexpr std::array<std::uint_fast32_t, 3> mortonDecode3(
    std::uint_fast64_t x)
{
#if defined(UFO_BMI2)
	return {static_cast<std::uint_fast32_t>(_pext_u64(x, MORTON3_X_MASK)),
	        static_cast<std::uint_fast32_t>(_pext_u64(x, MORTON3_Y_MASK)),
	        static_cast<std::uint_fast32_t>(_pext_u64(x, MORTON3_Z_MASK))};
#else
	return {mortonCompact3(x), mortonCompact3(x >> 1), mortonCompact3(x >> 2)};
#endif
}

[[nodiscard]] constexpr std::array<std::uint_fast16_t, 4> mortonDecode4(
    std::uint_fast64_t x)
{
#if defined(UFO_BMI2)
	return {static_cast<std::uint_fast32_t>(_pext_u64(x, MORTON4_X_MASK)),
	        static_cast<std::uint_fast32_t>(_pext_u64(x, MORTON4_Y_MASK)),
	        static_cast<std::uint_fast32_t>(_pext_u64(x, MORTON4_Z_MASK)),
	        static_cast<std::uint_fast32_t>(_pext_u64(x, MORTON4_W_MASK))};
#else
	return {mortonCompact4(x), mortonCompact4(x >> 1), mortonCompact4(x >> 2),
	        mortonCompact4(x >> 3)};
#endif
}
}  // namespace ufo

#endif  // UFO_UTILITY_MORTON_HPP