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

#ifndef FRONTEND_COMMON_H
#define FRONTEND_COMMON_H

#include <charconv>
#include <cstdlib>
#include <iostream>
#include <format>
#include <optional>
#include <string_view>

#include "../utilities/string.h"

template<typename... Args>
void Warning(std::format_string<Args...> &&format_string, Args &&...args)
{
	std::cerr << "Warning: " << std::format(std::forward<std::format_string<Args...>>(format_string), std::forward<Args>(args)...) << '\n';
}

template<typename... Args>
void Error(std::format_string<Args...> &&format_string, Args &&...args)
{
	std::cerr << "Error: " << std::format(std::forward<std::format_string<Args...>>(format_string), std::forward<Args>(args)...) << '\n';
}

template<typename... Args>
[[noreturn]] void ErrorAndExit(std::format_string<Args...> &&format_string, Args &&...args)
{
	Error(std::forward<std::format_string<Args...>>(format_string), std::forward<Args>(args)...);
	std::exit(EXIT_FAILURE);
}

[[nodiscard]] std::optional<std::pair<std::string_view, unsigned long>> DecodeDefinition(const std::string_view &definition)
{
	const auto &GetValue = [](const std::string_view &value_string) -> std::optional<unsigned long>
	{
		const auto &ValueFromChars = [](const std::string_view &value_string, const int base) -> std::optional<unsigned long>
		{
			unsigned long value;
			const auto &[end, error_code] = std::from_chars(value_string.begin(), value_string.end(), value, base);

			if (error_code != std::errc{} || end != value_string.end())
				return std::nullopt;

			return value;
		};

		/* SN 68k (asm68k) supports XX, 0xXX, and $XX. */
		if (!value_string.empty() && value_string.front() == '$')
			return ValueFromChars(Utilities::String::Subview(value_string, 1), 0x10);
		else
			return ValueFromChars(value_string, 0);
	};

	const auto &separator_position = definition.find('=');
	const std::string_view identifier = Utilities::String::Subview(definition, 0, separator_position);

	if (separator_position == definition.npos)
		return {{identifier, 0}}; /* Exactly what asm68k does... */

	const auto &value = GetValue(Utilities::String::Subview(definition, separator_position + 1));

	if (!value.has_value())
		return std::nullopt;

	return {{identifier, *value}};
}

#endif // FRONTEND_COMMON_H
