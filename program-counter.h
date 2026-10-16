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

#ifndef PROGRAM_COUNTER_H
#define PROGRAM_COUNTER_H

#include <cassert>

class ProgramCounter
{
private:
	unsigned long value = 0;

public:
	// The 68000 has 32-bit addresses, so never let the program counter exceed this range!
	ProgramCounter& operator=(const unsigned long new_value)
	{
		assert(new_value <= 0xFFFFFFFF);

		value = new_value;

		return *this;
	}

	ProgramCounter& operator+=(const auto delta)
	{
		value += delta;
		value &= 0xFFFFFFFF;

		return *this;
	}

	ProgramCounter& operator-=(const auto delta)
	{
		value -= delta;
		value &= 0xFFFFFFFF;

		return *this;
	}

	operator unsigned long() const
	{
		return value;
	}
};

#endif /* PROGRAM_COUNTER_H */
