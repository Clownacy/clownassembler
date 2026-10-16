/*
 * Copyright (C) 2026 Clownacy
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

#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>

#include "io.h"

namespace Object
{
	class Writer
	{
	private:
		IO::StreamBinaryWriter output;
		std::ostream::pos_type previous_segment_length_output_position;
		std::size_t segment_position = 0, segment_length = 0;

		void TerminatePreviousRecordSegment();
		void WriteRecordSegment();

	public:
		struct Position
		{
			friend Writer;

		private:
			Position(const std::ostream::pos_type position, const std::size_t segment_length)
				: position(position)
				, segment_length(segment_length)
			{}

			std::ostream::pos_type position;
			std::size_t segment_length;
		};

		Writer(std::ostream &stream)
			: output(stream)
		{}

		void WriteHeader();
		void WriteFooter();
		void WriteByte(unsigned int value, bool doing_fixup_pass);
		void BeginNewSegment(std::size_t position);

		Position Tell() const
		{
			return {output.stream.tellp(), segment_length};
		}

		void Seek(Position &position)
		{
			output.stream.seekp(position.position);
			segment_length = position.segment_length;
		}
	};
}

#endif /* OBJECT_H */
