/*
 * Copyright (C) 2024-2025 Clownacy
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

#ifndef IO_H
#define IO_H

#include <iostream>
#include <format>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>

#include "clowncommon/clowncommon.h"

typedef char* (*ClownAssembler_ReadLine)(std::istream &callbacks, char *buffer, size_t buffer_size);

char* TextInput_fgets(char *buffer, size_t buffer_size, std::istream &callbacks);

template<typename... Args>
void TextOutput_Format(std::ostream &stream, std::format_string<Args...> &&format_string, Args &&...args)
{
	// TODO: Replace this with `std::format_to`.
	stream << std::format(std::forward<std::format_string<Args...>>(format_string), std::forward<Args>(args)...);
}

#endif /* IO_H */
