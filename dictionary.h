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
#include <functional>
#include <string>
#include <string_view>
#include <unordered_map>

#include "utilities/case-insensitive.h"

template<typename Entry>
using DictionaryBase = std::unordered_map<std::string, Entry, Utilities::CaseInsensitive::StringHasher, Utilities::CaseInsensitive::StringComparer>;

template<typename EntryArgument>
class Dictionary : public DictionaryBase<EntryArgument>
{
public:
	using Entry = EntryArgument;

	Dictionary(const bool case_insensitive)
		: DictionaryBase<Entry>(0, Utilities::CaseInsensitive::StringHasher{}, Utilities::CaseInsensitive::StringComparer{case_insensitive})
	{}

	auto emplace(const std::string_view &identifier)
	{
		// Nightmare Difficulty C++
		return DictionaryBase<Entry>::emplace(std::piecewise_construct, std::forward_as_tuple(identifier), std::tuple<>());
	}

	Entry& operator[](const std::string_view &identifier)
	{
		// All of this crap to work-around C++ not having a decent `operator[]` before C++26... fuck me.
		auto it = this->find(identifier);

		return it != this->end() ? it->second : this->emplace(identifier).first->second;
	}

	void Filter(const std::function<bool(const Entry &entry, const std::string_view &identifier)> &filter_function)
	{
		std::erase_if(*this,
			[&](auto &entry)
			{
				return !filter_function(entry.second, entry.first);
			}
		);
	}
};

#endif /* DICTIONARY_H */
