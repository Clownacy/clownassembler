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

template<typename EntryArgument>
class Dictionary
{
private:
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

public:
	using Entry = EntryArgument;

	std::unordered_map<std::string, Entry, StringHash, StringCompare> internal;

	Dictionary(cc_bool case_insensitive)
		: internal(0, StringHash{}, StringCompare{case_insensitive})
	{
	}

	cc_bool LookUpAndCreateIfNotExist(const StringView &identifier, Entry **entry_pointer)
	{
		// All of this crap to work-around C++ not having a decent `operator[]` before C++26... fuck me.
		auto it = internal.find(identifier);

		auto &entry = it != internal.end() ? it->second : internal.emplace(std::piecewise_construct, std::forward_as_tuple(identifier), std::tuple<>()).first->second; // Nightmare Difficulty C++

		if (entry_pointer != nullptr)
			*entry_pointer = &entry;

		return cc_true;
	}

	Entry* LookUp(const StringView &identifier)
	{
		auto it = internal.find(identifier);

		if (it == internal.end())
			return nullptr;

		return &it->second;
	}

	cc_bool Remove(const StringView &identifier)
	{
		auto it = internal.find(identifier);

		if (it == internal.end())
			return cc_false;

		internal.erase(it);

		return cc_true;
	}

	void Filter(cc_bool (*filter_function)(Entry *entry, const char *identifier, std::size_t identifier_length, void *user_data), const void *user_data)
	{
		std::erase_if(internal,
			[&](auto &entry)
			{
				return !filter_function(&entry.second, entry.first.data(), entry.first.length(), const_cast<void*>(user_data));
			}
		);
	}
};

#endif /* DICTIONARY_H */
