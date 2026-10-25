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

#include <format>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>

#include "utilities/case-insensitive.h"
#include "utilities/string.h"

// To allow code to reference local labels globally ("jmp (Func@Ret).l"), the dictionary
// internally stores the global and local label as a single concatenated identifier.
// To avoid needing to allocate and generate combined identifiers for every lookup,
// the dictionary supports performing lookups with split identifiers. The following
// amalgamation of nightmare-code is responsible for making this possible.

struct DictionaryLookupKey
{
	std::string_view global, local;

	DictionaryLookupKey(std::string_view local, std::string_view global = {})
		: global(std::move(global))
		, local(std::move(local))
	{}
};

class DictionaryHasher
{
public:
	using is_transparent = void;

	std::size_t operator()(const std::string &key) const
	{
		Utilities::CaseInsensitive::StringHasher::Piecemeal hasher;
		hasher.HashString(key);
		return hasher.GetHash();
	}

	std::size_t operator()(const DictionaryLookupKey &key) const
	{
		Utilities::CaseInsensitive::StringHasher::Piecemeal hasher;
		hasher.HashString(key.global);
		hasher.HashString(key.local);
		return hasher.GetHash();
	}
};

class DictionaryComparer
{
private:
	const Utilities::CaseInsensitive::StringComparer string_comparer;

public:
	using is_transparent = void;

	constexpr DictionaryComparer(const bool case_insensitive)
		: string_comparer(case_insensitive)
	{}

	constexpr bool operator()(const std::string &lhs, const std::string &rhs) const
	{
		return string_comparer(lhs, rhs);
	}

	constexpr bool operator()(const DictionaryLookupKey &lhs, const std::string &rhs) const
	{
		const auto first_length = lhs.global.length();

		if (!string_comparer(Utilities::String::Subview(rhs, 0, first_length), lhs.global))
			return false;

		if (!string_comparer(Utilities::String::Subview(rhs, first_length), lhs.local))
			return false;

		return true;
	}

	constexpr bool operator()(const std::string &lhs, const DictionaryLookupKey &rhs) const
	{
		return operator()(rhs, lhs);
	}
};

template<typename Entry>
using DictionaryBase = std::unordered_map<std::string, Entry, DictionaryHasher, DictionaryComparer>;

template<typename Entry>
class Dictionary : public DictionaryBase<Entry>
{
private:
	using Base = DictionaryBase<Entry>;

public:
	Dictionary(const bool case_insensitive)
		: Base(0, DictionaryHasher{}, DictionaryComparer{case_insensitive})
	{}

	template<typename T>
	auto emplace(T &&identifier)
	{
		// Nightmare Difficulty C++
		return Base::emplace(std::piecewise_construct, std::forward_as_tuple(std::forward<T>(identifier)), std::tuple<>());
	}

	auto emplace(const DictionaryLookupKey &identifier)
	{
		return emplace(std::format("{}{}", identifier.global, identifier.local));
	}

	Entry& operator[](const auto &identifier)
	{
		// All of this crap to work-around C++ not having a decent `operator[]` before C++26... fuck me.
		auto it = Base::find(identifier);

		return it != Base::end() ? it->second : emplace(identifier).first->second;
	}
};

#endif /* DICTIONARY_H */
