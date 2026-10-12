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

#ifndef UTILITIES_H
#define UTILITIES_H

#include <algorithm>
#include <cstddef>
#include <string>
#include <string_view>

#include "case-insensitive.h"

namespace Utilities
{
	namespace String
	{
		inline std::size_t Span(const std::string_view &view, const char* const characters, const std::size_t position = 0)
		{
			return std::min(view.length(), view.find_first_not_of(characters, position));
		}

		inline void ToLower(std::string &string)
		{
			constexpr CaseInsensitive::ToLower to_lower(true);

			for (auto &character : string)
				character = to_lower(character);
		}
	}
}

#endif // UTILITIES_H
