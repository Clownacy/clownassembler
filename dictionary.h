/*
 * Copyright (C) 2022-2025 Clownacy
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

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <cstddef>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>

#include "clowncommon/clowncommon.h"

#include "string.h"

class Dictionary
{
public:
	struct ToLower
	{
		const int delta;

		constexpr ToLower(const cc_bool enabled)
			: delta(enabled ? 'a' - 'A' : 0)
		{}

		constexpr auto operator()(const auto &value) const
		{
			return value + (value >= 'A' && value <= 'Z' ? delta : 0);
		}
	};

	struct StringHash
	{
		using is_transparent = void;

		std::size_t operator()(const std::string_view &string) const
		{
			constexpr ToLower to_lower(cc_true);

			// Hash the identifier using djb2.
			// http://www.cse.yorku.ca/~oz/hash.html
			std::size_t hash = 5381;

			for (const auto &character : string)
				hash = hash * 33 + to_lower(character); // Hash the identifier in lower-case form, so that case-insensitive mode works.

			return hash;
		}
	};

	class StringCompare
	{
	private:
		const ToLower to_lower;

	public:
		using is_transparent = void;

		StringCompare(const cc_bool enabled)
			: to_lower(enabled)
		{}

		constexpr bool operator()(const auto &lhs, const auto &rhs) const
		{
			if (std::size(lhs) != std::size(rhs))
				return false;

			for (std::size_t i = 0; i < std::size(lhs); ++i)
			{
				if (to_lower(lhs[i]) != to_lower(rhs[i]))
					return false;
			}

			return true;
		}
	};

	typedef struct Entry
	{
		int type = -1;
		union
		{
			unsigned long unsigned_long;
			long signed_long;
			void *pointer;
			String string;
		} shared; 
	} Entry;

	std::optional<std::unordered_map<std::string, Entry, StringHash, StringCompare>> internal;

	cc_bool Init(cc_bool case_insensitive);
	void Deinit();

	cc_bool LookUpAndCreateIfNotExist(const StringView &identifier, Entry **entry_pointer);
	Entry* LookUp(const StringView &identifier);
	cc_bool Remove(const StringView &identifier);
	void Filter(cc_bool (*filter_function)(Entry *entry, const char *identifier, std::size_t identifier_length, void *user_data), const void *user_data);
};

#endif /* DICTIONARY_H */
