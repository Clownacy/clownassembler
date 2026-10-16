/*
 * Copyright (C) 2023-2025 Clownacy
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

/* Documentation of AS's code file format can be found here:
   http://john.ccac.rwth-aachen.de:8000/as/as_EN.html#sect_5_1_ */

/* Terminology in this code reflects the above documentation. */

#include "p2bin.h"

#include <array>
#include <exception>
#include <format>
#include <optional>

#include "io.h"

bool ConvertObjectFileToFlatBinary(std::istream &input_stream, std::ostream &output_stream, std::ostream &error_stream)
{
	IO::StreamBinaryReader input(input_stream);

	std::optional<unsigned long> base_address;

	const auto &ProcessSegment = [&]()
	{
		const auto start_address = input.ReadU32LE();
		const auto length = input.ReadU16LE();

		if (!base_address.has_value())
			base_address = start_address;
		else if (start_address < *base_address)
			throw std::runtime_error("Error: Segment located before start of file.\n");
		else
			output_stream.seekp(start_address - *base_address);

		/* Copy segment data. */
		IO::CopyBytes(input.stream, output_stream, length);
	};

	const auto &ProcessRecords = [&]()
	{
		for (;;)
		{
			const auto record_header = input.ReadU8();

			switch (record_header)
			{
				case 0:
					/* Creator string. This marks the end of the file. */
					return;

				case 0x80:
					/* Entry point. We don't care about this. */
					input.ReadU32LE();
					break;

				case 0x81:
				{
					/* Arbitrary segment. */
					input.ReadU8(); /* Processor family. We don't care about this. */
					input.ReadU8(); /* Segment. We don't care about this. */
					const auto granularity = input.ReadU8();

					if (granularity != 1)
						throw std::runtime_error(std::format("Error: Unsupported granularity of {} (only 1 is supported).\n", granularity));

					ProcessSegment();

					break;
				}

				default:
					if (record_header >= 0x80)
						throw std::runtime_error(std::format("Error: Unrecognised record header value (0x{:02X}).\n", record_header));

					/* Legacy CODE segment. */
					ProcessSegment();

					break;
			}
		}
	};

	bool success = false;

	const auto previous_exceptions = input.stream.exceptions();
	input.stream.exceptions(input.stream.badbit | input.stream.eofbit | input.stream.failbit);

	try
	{
		/* Read and check the header's magic number. */
		const auto magic = input.ReadU16BE();

		/* TODO: Use error message manager for this. */
		if (magic != 0x8914)
			throw std::runtime_error(std::format("Error: Invalid header magic value - expected 0x8914 but got 0x{:04X}.\nInput file is either corrupt or not a valid AS code file.\n", magic));

		ProcessRecords();

		success = true;
	}
	catch (const std::runtime_error &error)
	{
		error_stream << error.what();
	}

	input.stream.exceptions(previous_exceptions);

	return success;
}
