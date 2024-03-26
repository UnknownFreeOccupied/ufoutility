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

// UFO
#include <ufo/util/compression.hpp>

// STL
#include <fstream>
#include <memory>
#include <sstream>
#include <stdexcept>

// LZ4 compression
#include <lz4.h>
#include <lz4hc.h>

namespace ufo
{
std::size_t maxCompressedSizeLz4(std::size_t uncompressed_size)
{
	auto a = uncompressed_size / LZ4_MAX_INPUT_SIZE;
	auto b = uncompressed_size % LZ4_MAX_INPUT_SIZE;
	return a * LZ4_compressBound(LZ4_MAX_INPUT_SIZE) + LZ4_compressBound(b) +
	       (a + 1) * sizeof(std::int64_t);
}

int compressSingleLz4(std::istream& in, std::ostream& out, std::size_t uncompressed_size,
                      int acceleration_level, int compression_level)
{
	if (LZ4_MAX_INPUT_SIZE < uncompressed_size) {
		return 0;
	}

	auto data            = std::make_unique<char[]>(uncompressed_size);
	auto compressed_data = std::make_unique<char[]>(uncompressed_size);
	in.read(data.get(), uncompressed_size);

	int cs;
	if (0 < compression_level) {
		cs = LZ4_compress_HC(data.get(), compressed_data.get(), uncompressed_size,
		                     uncompressed_size, compression_level);
	} else {
		cs = LZ4_compress_fast(data.get(), compressed_data.get(), uncompressed_size,
		                       uncompressed_size, acceleration_level);
	}

	if (0 < cs) {
		out.write(compressed_data.get(), cs);
	}

	return cs;
}

int compressLz4(ReadBuffer& in, WriteBuffer& out, std::size_t uncompressed_size,
                int acceleration_level, int compression_level)

{
	if (LZ4_MAX_INPUT_SIZE < uncompressed_size) {
		return 0;
	}

	char const* data            = reinterpret_cast<char const*>(in.data());
	auto        idx             = in.readIndex();
	auto        compressed_data = std::make_unique<char[]>(uncompressed_size);

	int cs;
	if (0 < compression_level) {
		cs = LZ4_compress_HC(data + idx, compressed_data.get(), uncompressed_size,
		                     uncompressed_size, compression_level);
	} else {
		cs = LZ4_compress_fast(data + idx, compressed_data.get(), uncompressed_size,
		                       uncompressed_size, acceleration_level);
	}

	if (0 < cs) {
		out.write(compressed_data.get(), cs);
	}

	in.skipRead(uncompressed_size);

	return cs;
}

bool compressMultiLz4(std::istream& in, std::ostream& out, std::size_t uncompressed_size,
                      int acceleration_level, int compression_level)
{
	auto data            = std::make_unique<char[]>(LZ4_MAX_INPUT_SIZE);
	auto compressed_data = std::make_unique<char[]>(LZ4_MAX_INPUT_SIZE);

	for (; 0 < uncompressed_size;) {
		auto src_size =
		    std::min(static_cast<std::size_t>(LZ4_MAX_INPUT_SIZE), uncompressed_size);
		uncompressed_size -= src_size;

		in.read(data.get(), src_size);

		std::int64_t cs;
		if (0 < compression_level) {
			cs = LZ4_compress_HC(data.get(), compressed_data.get(), src_size,
			                     LZ4_MAX_INPUT_SIZE, compression_level);
		} else {
			cs = LZ4_compress_fast(data.get(), compressed_data.get(), src_size,
			                       LZ4_MAX_INPUT_SIZE, acceleration_level);
		}

		if (0 >= cs) {
			return false;
		}

		out.write(reinterpret_cast<char const*>(&cs), sizeof(cs));
		out.write(compressed_data.get(), cs);
	}

	return true;
}

bool compressMultiLz4(ReadBuffer& in, WriteBuffer& out, std::size_t uncompressed_size,
                      int acceleration_level, int compression_level)
{
	auto max_size = maxCompressedSizeLz4(uncompressed_size);

	auto before_size = out.size();
	auto before_idx  = out.writeIndex();

	out.resize(std::max(before_size, before_idx + max_size));

	std::size_t compressed_size{};

	for (; 0 < uncompressed_size;) {
		auto src_size =
		    std::min(static_cast<std::size_t>(LZ4_MAX_INPUT_SIZE), uncompressed_size);
		uncompressed_size -= src_size;

		auto size_idx = out.writeIndex();

		std::int64_t cs;
		out.write(cs);

		auto dst_cap = std::min(static_cast<std::size_t>(LZ4_MAX_INPUT_SIZE),
		                        out.size() - out.writeIndex());

		if (0 < compression_level) {
			cs = LZ4_compress_HC(reinterpret_cast<char const*>(in.data()) + in.readIndex(),
			                     reinterpret_cast<char*>(out.data()) + out.writeIndex(),
			                     src_size, dst_cap, compression_level);
		} else {
			cs = LZ4_compress_fast(reinterpret_cast<char const*>(in.data()) + in.readIndex(),
			                       reinterpret_cast<char*>(out.data()) + out.writeIndex(),
			                       src_size, dst_cap, acceleration_level);
		}

		in.skipRead(src_size);

		compressed_size += sizeof(cs) + cs;

		auto cur_idx = out.writeIndex();
		out.setWriteIndex(size_idx);
		out.write(cs);
		out.setWriteIndex(cur_idx);

		if (0 >= cs) {
			return false;
		}
	}

	out.resize(std::max(before_size, before_idx + compressed_size));

	return true;
}

int decompressLz4(std::istream& in, std::ostream& out, std::size_t compressed_size,
                  std::size_t uncompressed_size)
{
	if (LZ4_MAX_INPUT_SIZE < uncompressed_size) {
		return 0;
	}

	auto compressed_data = std::make_unique<char[]>(compressed_size);
	auto data            = std::make_unique<char[]>(uncompressed_size);

	in.read(compressed_data.get(), compressed_size);

	int ds = LZ4_decompress_safe(compressed_data.get(), data.get(), compressed_size,
	                             uncompressed_size);

	if (0 < ds) {
		out.write(data.get(), ds);
	} else {
		return false;
	}

	return ds;
}

int decompressLz4(ReadBuffer& in, WriteBuffer& out, std::size_t compressed_size,
                  std::size_t uncompressed_size)
{
	if (LZ4_MAX_INPUT_SIZE < uncompressed_size) {
		return 0;
	}

	out.resize(std::max(out.size(), out.writeIndex() + uncompressed_size));

	auto wi = out.writeIndex();
	out.skipWrite(uncompressed_size);

	return LZ4_decompress_safe(reinterpret_cast<char const*>(in.data()) + in.readIndex(),
	                           reinterpret_cast<char*>(out.data()) + wi, compressed_size,
	                           uncompressed_size);
}

bool decompressMultiLz4(std::istream& in, std::ostream& out,
                        std::size_t uncompressed_size)
{
	auto a = uncompressed_size / LZ4_MAX_INPUT_SIZE;
	auto b = uncompressed_size % LZ4_MAX_INPUT_SIZE;

	a = 0 < b ? a + 1 : a;

	auto compressed_data = std::make_unique<char[]>(LZ4_MAX_INPUT_SIZE);
	auto data            = std::make_unique<char[]>(LZ4_MAX_INPUT_SIZE);
	for (std::size_t i{}; a != i; ++i) {
		std::int64_t cs;
		in.read(reinterpret_cast<char*>(&cs), sizeof(cs));

		in.read(compressed_data.get(), cs);
		int ds =
		    LZ4_decompress_safe(compressed_data.get(), data.get(), cs, LZ4_MAX_INPUT_SIZE);

		if (0 < ds) {
			out.write(data.get(), ds);
		} else {
			return false;
		}
	}

	return !in.fail();
}

bool decompressMultiLz4(ReadBuffer& in, WriteBuffer& out, std::size_t uncompressed_size)
{
	auto a = uncompressed_size / LZ4_MAX_INPUT_SIZE;
	auto b = uncompressed_size % LZ4_MAX_INPUT_SIZE;

	a = 0 < b ? a + 1 : a;

	out.resize(std::max(out.size(), out.writeIndex() + uncompressed_size));

	for (std::size_t i{}; a != i; ++i) {
		std::int64_t cs;
		in.read(cs);

		int ds = LZ4_decompress_safe(
		    reinterpret_cast<char const*>(in.data()) + in.readIndex(),
		    reinterpret_cast<char*>(out.data()) + out.writeIndex(), cs,
		    std::min(out.writeLeft(), static_cast<std::size_t>(LZ4_MAX_INPUT_SIZE)));

		in.skipRead(cs);
		out.skipWrite(ds);

		if (0 >= ds) {
			return false;
		}
	}

	return true;
}
}  // namespace ufo
