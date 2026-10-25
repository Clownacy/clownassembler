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

#ifndef UTILITIES_STRING_H
#define UTILITIES_STRING_H

#include <cstddef>
#include <string>
#include <string_view>

#include "case-insensitive.h"

namespace Utilities
{
	namespace String
	{
		constexpr inline std::string_view Subview(const std::string_view &string, const std::string_view::size_type position = 0, const std::string_view::size_type count = std::string_view::npos)
		{
			return string.substr(position, count);
		}

		inline std::size_t GetLeadingLength(const std::string_view &view, const std::string_view &characters, const std::size_t position = 0)
		{
			const auto result = view.find_first_not_of(characters, position);

			if (result == view.npos)
				return view.length();

			return result - position;
		}

		inline std::size_t GetTrailingLength(const std::string_view &view, const std::string_view &characters, const std::size_t position = std::string_view::npos)
		{
			const auto result = view.find_last_not_of(characters, position);

			if (result == view.npos)
				return view.length();

			return std::min(view.length(), position) - result - 1;
		}

		inline void RemoveLeading(std::string_view &string_view, const std::string_view &characters)
		{
			string_view.remove_prefix(GetLeadingLength(string_view, characters));
		}

		inline void RemoveTrailing(std::string_view &string_view, const std::string_view &characters)
		{
			string_view.remove_suffix(GetTrailingLength(string_view, characters));
		}

		inline void RemoveLeadingAndTrailing(std::string_view &string_view, const std::string_view &characters)
		{
			RemoveLeading(string_view, characters);
			RemoveTrailing(string_view, characters);
		}

		inline std::string_view ExtractLeading(const std::string_view &string_view, const std::string_view &characters)
		{
			return Utilities::String::Subview(string_view, 0, GetLeadingLength(string_view, characters));
		}

		inline void ToLower(std::string &string)
		{
			constexpr CaseInsensitive::ToLower to_lower(true);

			for (auto &character : string)
				character = to_lower(character);
		}
	}
}

#endif // UTILITIES_STRING_H
