/*
 * Copyright (C) 2025 Clownacy
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

#ifndef SUBSTITUTE_H
#define SUBSTITUTE_H

#include <cstddef>
#include <forward_list>
#include <functional>
#include <optional>
#include <string>
#include <string_view>

#include "clowncommon/clowncommon.h"

#include "string.h"

class Substitute
{
public:
	using CustomSearch = std::function<std::optional<std::string_view>(const std::string_view &view_to_search, std::size_t starting_position, cc_bool case_insensitive, std::size_t* const found_position, std::size_t* const found_length)>;

private:
	struct ListEntry
	{
		std::string identifier;
		const std::string *value;
	};

	std::forward_list<ListEntry> list;

	static cc_bool FindSubstitute(const std::string_view &view_to_search, const std::size_t starting_position, const std::string_view &substitute, const cc_bool allow_implicit_matches, const cc_bool case_insensitive, std::size_t* const found_position, std::size_t* const found_length);
	std::optional<std::string_view> FindEarliestSubstitute(Substitute *other_state, const std::string_view &view_to_search, const std::size_t starting_position, const CustomSearch &custom_search_callback, const cc_bool allow_implicit_matches, const cc_bool case_insensitive, std::size_t* const earliest_found_position, std::size_t* const earliest_found_length);

public:
	cc_bool PushSubstitute(const std::string_view &identifier, const std::string &value);
	void PopSubstitute();

	void ProcessSubString(Substitute *other_state, std::string &string, std::string_view &view_to_search, const CustomSearch &custom_search_callback, cc_bool allow_implicit_matches, cc_bool case_insensitive);
	void ProcessString(Substitute *other_state, std::string &string, const CustomSearch &custom_search_callback, cc_bool allow_implicit_matches, cc_bool case_insensitive);

	static cc_bool IsSubstituteBlockingCharacter(char character);
	static cc_bool IsWhitespaceCharacter(char character);
};

#endif /* SUBSTITUTE_H */
