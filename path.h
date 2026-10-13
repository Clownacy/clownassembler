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

#ifndef PATH_H
#define PATH_H

#include <filesystem>
#include <string>
#include <string_view>

inline std::filesystem::path ToStandardPath(const std::string_view &input_path)
{
	std::u8string output_path;
	output_path.reserve(input_path.length());

	// Convert DOS path separators to standard ones.
	// Asm68k-based projects often use these, so they need to be
	// converted for compatibility with platforms such as Linux.
	for (const auto &character : input_path)
		output_path.push_back(character == '\\' ? '/' : character);

	return output_path;
}

#endif // PATH_H
