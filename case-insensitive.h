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

#ifndef CASE_H
#define CASE_H

#include <cstddef>
#include <string_view>

namespace Case
{
	class ToLower
	{
	private:
		const int delta;

	public:
		constexpr ToLower(const bool enabled)
			: delta(enabled ? 'a' - 'A' : 0)
		{}

		constexpr auto operator()(const auto &value) const
		{
			return value + (value >= 'A' && value <= 'Z' ? delta : 0);
		}
	};

	class StringHash
	{
	public:
		using is_transparent = void;

		std::size_t operator()(const std::string_view &string) const
		{
			constexpr ToLower to_lower(true);

			// Hash the identifier using djb2.
			// http://www.cse.yorku.ca/~oz/hash.html
			std::size_t hash = 5381;

			for (const auto &character : string)
				hash = hash * 33 + to_lower(character); // Hash the identifier in lower-case form, so that case-insensitive mode works.

			return hash;
		}
	};

	class CharCompare
	{
	private:
		const ToLower to_lower;

	public:
		using is_transparent = void;

		constexpr CharCompare(const bool case_insensitive)
			: to_lower(case_insensitive)
		{}

		constexpr bool operator()(const auto &lhs, const auto &rhs) const
		{
			return to_lower(lhs) == to_lower(rhs);
		}
	};

	class StringCompare
	{
	private:
		const CharCompare char_compare;

	public:
		using is_transparent = void;

		constexpr StringCompare(const bool case_insensitive)
			: char_compare(case_insensitive)
		{}

		constexpr bool operator()(const std::string_view &lhs, const std::string_view &rhs) const
		{
			return std::equal(std::begin(lhs), std::end(lhs), std::begin(rhs), std::end(rhs), char_compare);
		}
	};

	inline bool CompareStringsCaseInsensitive(const std::string_view &lhs, const std::string_view &rhs)
	{
		constexpr StringCompare string_compare(true);

		return string_compare(lhs, rhs);
	}
}

#endif // CASE_H
