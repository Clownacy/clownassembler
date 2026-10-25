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

#ifndef UTILITIES_CASE_INSENSITIVE_H
#define UTILITIES_CASE_INSENSITIVE_H

#include <algorithm>
#include <cstddef>
#include <string_view>

namespace Utilities
{
	namespace CaseInsensitive
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

		class StringHasher
		{
		public:
			class Piecemeal
			{
			private:
				// Hash the identifier using djb2.
				// http://www.cse.yorku.ca/~oz/hash.html
				std::size_t hash = 5381;

			public:
				void HashString(const std::string_view &string)
				{
					// Hash the identifier in lower-case form, so that case-insensitive mode works.
					// We avoid using `to_lower` since it is slower, due to being precise.
					// In contrast, ORing 0x20 is more of a hack, but it is faster, and precision
					// is not essential.
					for (const auto &character : string)
						hash = hash * 33 + (character | 0x20);
				};

				auto GetHash() const
				{
					return hash;
				}
			};

			using is_transparent = void;

			std::size_t operator()(const std::string_view &string) const
			{
				Piecemeal piecemeal;
				piecemeal.HashString(string);
				return piecemeal.GetHash();
			}
		};

		class CharComparer
		{
		private:
			const ToLower to_lower;

		public:
			using is_transparent = void;

			constexpr CharComparer(const bool case_insensitive)
				: to_lower(case_insensitive)
			{}

			constexpr bool operator()(const auto &lhs, const auto &rhs) const
			{
				return to_lower(lhs) == to_lower(rhs);
			}
		};

		class StringComparer
		{
		private:
			const CharComparer char_comparer;

		public:
			using is_transparent = void;

			constexpr StringComparer(const bool case_insensitive)
				: char_comparer(case_insensitive)
			{}

			constexpr bool operator()(const std::string_view &lhs, const std::string_view &rhs) const
			{
				return std::equal(std::begin(lhs), std::end(lhs), std::begin(rhs), std::end(rhs), char_comparer);
			}
		};

		inline bool CompareStrings(const std::string_view &lhs, const std::string_view &rhs)
		{
			constexpr StringComparer string_comparer(true);

			return string_comparer(lhs, rhs);
		}

		inline std::size_t FindString(const std::string_view &view, const std::string_view &sub_view, const std::size_t position, const bool case_insensitive)
		{
			const auto it = std::search(std::begin(view) + position, std::end(view), std::begin(sub_view), std::end(sub_view), CaseInsensitive::CharComparer(case_insensitive));

			if (it == std::end(view))
				return view.npos;

			return std::distance(std::begin(view), it);
		}
	}
}

#endif // UTILITIES_CASE_INSENSITIVE_H
