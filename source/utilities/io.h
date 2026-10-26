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

#ifndef UTILITIES_IO_H
#define UTILITIES_IO_H

#include <algorithm>
#include <array>
#include <cstddef>
#include <iostream>
#include <format>
#include <iterator>
#include <string_view>

namespace IO
{
	inline void Print(std::ostream &stream, const std::string_view &message)
	{
		stream << message;
	}

	template<typename... Args>
	inline void Print(std::ostream &stream, std::format_string<Args...> &&format_string, Args &&...args)
	{
		// `std::print` should be considerably faster.
	#if defined(__cpp_lib_print) && __cpp_lib_print >= 202207L
		std::print(stream,
	#else
		std::format_to(std::ostream_iterator<char>(stream),
	#endif
			std::forward<std::format_string<Args...>>(format_string), std::forward<Args>(args)...);
	}

	inline void PrintLine(std::ostream &stream, const std::string_view &message)
	{
		Print(stream, "{}\n", message);
	}

	template<typename... Args>
	inline void PrintLine(std::ostream &stream, std::format_string<Args...> &&format_string, Args &&...args)
	{
		Print(stream, std::forward<std::format_string<Args...>>(format_string), std::forward<Args>(args)...);
		stream << '\n';
	}

	inline void PrintWarning(std::ostream &stream, const std::string_view &message)
	{
		PrintLine(stream, "Warning: {}", message);
	}

	template<typename... Args>
	inline void PrintWarning(std::ostream &stream, std::format_string<Args...> &&format_string, Args &&...args)
	{
		stream << "Warning: ";
		PrintLine(stream, std::forward<std::format_string<Args...>>(format_string), std::forward<Args>(args)...);
	}

	inline void PrintError(std::ostream &stream, const std::string_view &message)
	{
		PrintLine(stream, "Error: {}", message);
	}

	template<typename... Args>
	inline void PrintError(std::ostream &stream, std::format_string<Args...> &&format_string, Args &&...args)
	{
		stream << "Error: ";
		PrintLine(stream, std::forward<std::format_string<Args...>>(format_string), std::forward<Args>(args)...);
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

	class BinaryReader
	{
	public:
		virtual unsigned int ReadU8() = 0;

		unsigned long ReadIntegerLE(const unsigned int total_bytes)
		{
			unsigned long value = 0;

			for (unsigned int i = 0; i < total_bytes; ++i)
				value |= static_cast<unsigned long>(ReadU8()) << (i * 8);

			return value;
		}

		unsigned int ReadU16LE()
		{
			return ReadIntegerLE(2);
		}

		unsigned long ReadU32LE()
		{
			return ReadIntegerLE(4);
		}

		unsigned long ReadIntegerBE(const unsigned int total_bytes)
		{
			unsigned long value = 0;

			for (unsigned int i = 0; i < total_bytes; ++i)
			{
				value <<= 8;
				value |= ReadU8();
			}

			return value;
		}

		unsigned int ReadU16BE()
		{
			return ReadIntegerBE(2);
		}

		unsigned long ReadU32BE()
		{
			return ReadIntegerBE(4);
		}
	};

	class StreamBinaryReader : public BinaryReader
	{
	public:
		std::istream &stream;

		StreamBinaryReader(std::istream &stream)
			: stream(stream)
		{}

		unsigned int ReadU8() override final
		{
			return stream.get();
		}
	};

	class BinaryWriter
	{
	public:
		virtual void WriteU8(unsigned int value) = 0;

		void WriteIntegerLE(unsigned long value, const unsigned int total_bytes)
		{
			for (unsigned int i = 0; i < total_bytes; ++i)
			{
				WriteU8(value & 0xFF);
				value >>= 8;
			}
		}

		void WriteU16LE(const unsigned int value)
		{
			WriteIntegerLE(value, 2);
		}

		void WriteU32LE(const unsigned int value)
		{
			WriteIntegerLE(value, 4);
		}

		void WriteIntegerBE(unsigned long value, const unsigned int total_bytes)
		{
			for (unsigned int i = 0; i < total_bytes; ++i)
			{
				WriteU8((value >> (total_bytes - 1) * 8) & 0xFF);
				value <<= 8;
			}
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

	class StreamBinaryWriter : public BinaryWriter
	{
	public:
		std::ostream &stream;

		StreamBinaryWriter(std::ostream &stream)
			: stream(stream)
		{}

		void WriteU8(const unsigned int value) override final
		{
			stream.put(value);
		}
	};
}

#endif /* UTILITIES_IO_H */
