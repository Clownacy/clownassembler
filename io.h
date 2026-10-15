/*
 * Copyright (C) 2024-2025 Clownacy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef IO_H
#define IO_H

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <format>
#include <iterator>

namespace IO
{
	template<typename... Args>
	void Print(std::ostream &stream, std::format_string<Args...> &&format_string, Args &&...args)
	{
		// TODO: Replace this with `std::print` when that is provided by all target platforms.
		// TODO: This is slowing-down the assembler quite a bit!
		std::format_to(std::ostream_iterator<char>(stream), std::forward<std::format_string<Args...>>(format_string), std::forward<Args>(args)...);
	}

	inline void CopyBytes(std::istream &input, std::ostream &output, const std::size_t total_bytes)
	{
		// We do some batching using a buffer to improve performance.
		std::array<char, 0x100> buffer;

		for (std::size_t i = 0; i < total_bytes; i += std::size(buffer))
		{
			const auto bytes_to_do = std::min(std::size(buffer), total_bytes - i);

			input.read(std::data(buffer), bytes_to_do);
			output.write(std::data(buffer), bytes_to_do);
		}
	}

	class IntegerWriter
	{
	private:
		void WriteIntegerLE(unsigned long value, const unsigned int total_bytes)
		{
			for (unsigned int i = 0; i < total_bytes; ++i)
			{
				WriteU8(value & 0xFF);
				value >>= 8;
			}
		}

		void WriteIntegerBE(unsigned long value, const unsigned int total_bytes)
		{
			for (unsigned int i = 0; i < total_bytes; ++i)
			{
				WriteU8((value >> (total_bytes - 1) * 8) & 0xFF);
				value <<= 8;
			}
		}

	public:
		virtual void WriteU8(unsigned int value) = 0;

		void WriteU16LE(const unsigned int value)
		{
			WriteIntegerLE(value, 2);
		}

		void WriteU32LE(const unsigned int value)
		{
			WriteIntegerLE(value, 4);
		}

		void WriteU16BE(const unsigned int value)
		{
			WriteIntegerBE(value, 2);
		}

		void WriteU32BE(const unsigned int value)
		{
			WriteIntegerBE(value, 4);
		}
	};

	class StreamIntegerWriter : public IntegerWriter
	{
	public:
		std::ostream &stream;

		StreamIntegerWriter(std::ostream &stream)
			: stream(stream)
		{}

		void WriteU8(const unsigned int value) override final
		{
			stream.put(value);
		}
	};

	inline unsigned int ReadU8(std::istream &stream)
	{
		return stream.get();
	}

	inline unsigned long ReadIntegerLE(std::istream &stream, const unsigned int total_bytes)
	{
		unsigned long value = 0;

		for (unsigned int i = 0; i < total_bytes; ++i)
			value |= static_cast<unsigned long>(ReadU8(stream)) << (i * 8);

		return value;
	}

	inline unsigned int ReadU16LE(std::istream &stream)
	{
		return ReadIntegerLE(stream, 2);
	}

	inline unsigned long ReadU32LE(std::istream &stream)
	{
		return ReadIntegerLE(stream, 4);
	}
}

#endif /* IO_H */
