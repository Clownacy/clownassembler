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

#include <cstdlib>
#include <iostream>
#include <format>

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

#endif // FRONTEND_COMMON_H
