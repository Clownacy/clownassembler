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

#include "dictionary.h"

cc_bool Dictionary::Init(cc_bool case_insensitive)
{
	internal.emplace(0, decltype(internal)::value_type::hasher{}, decltype(internal)::value_type::key_equal{case_insensitive});

	return cc_true;
}

void Dictionary::Deinit()
{
	internal.reset();
}

cc_bool Dictionary::LookUpAndCreateIfNotExist(const StringView &identifier, Entry **entry_pointer)
{
	// All of this crap to work-around C++ not having a decent `operator[]` before C++26... fuck me.
	const std::string_view key{identifier.buffer, identifier.length};

	auto it = internal->find(key);

	auto &entry = it != internal->end() ? it->second : internal->emplace(std::piecewise_construct, std::forward_as_tuple(key), std::tuple<>()).first->second; // Nightmare Difficulty C++

	if (entry_pointer != nullptr)
		*entry_pointer = &entry;

	return cc_true;
}

Dictionary::Entry* Dictionary::LookUp(const StringView &identifier)
{
	const std::string_view key{identifier.buffer, identifier.length};

	auto it = internal->find(key);

	if (it == internal->end())
		return nullptr;

	return &it->second;
}

cc_bool Dictionary::Remove(const StringView &identifier)
{
	auto it = internal->find(std::string_view{identifier.buffer, identifier.length});

	if (it == internal->end())
		return cc_false;

	internal->erase(it);

	return cc_true;
}

void Dictionary::Filter(cc_bool (*filter_function)(Entry *entry, const char *identifier, size_t identifier_length, void *user_data), const void *user_data)
{
	std::erase_if(*internal,
		[&](auto &entry)
		{
			return !filter_function(&entry.second, entry.first.data(), entry.first.length(), const_cast<void*>(user_data));
		}
	);
}
