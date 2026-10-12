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

#ifndef ERROR_H
#define ERROR_H

#include <deque>
#include <iostream>
#include <string>
#include <string_view>

#include "io.h"

class ErrorManager
{
public:
	struct Location
	{
		std::string file_path; /* TODO: This should really be a `std::string_view`, but fix-ups need deep copies... */
		unsigned long line_number;
	};

private:
	std::ostream &error_stream;
	const std::deque<Location> &locations;
	const std::string_view &source_line;
	const bool &warnings_enabled, &pedantic_warnings_enabled;
	bool error_encountered = false;

	void ErrorMessageCommon()
	{
		for (const auto &location : locations)
			TextOutput_Format(error_stream, "\nOn line {} of '{}'...", location.line_number, location.file_path);

		TextOutput_Format(error_stream, "\n{}\n\n", source_line.empty() ? "[No source line]" : source_line);
	}

public:
	template<typename... Args>
	void SemanticWarning(std::format_string<Args...> &&format_string, Args &&...args)
	{
		if (warnings_enabled)
		{
			error_stream << "Warning: ";

			TextOutput_Format(error_stream, std::forward<std::format_string<Args...>>(format_string), std::forward<Args>(args)...);

			ErrorMessageCommon();
		}
	}

	template<typename... Args>
	void SemanticError(std::format_string<Args...> &&format_string, Args &&...args)
	{
		error_stream << "Error: ";

		TextOutput_Format(error_stream, std::forward<std::format_string<Args...>>(format_string), std::forward<Args>(args)...);

		ErrorMessageCommon();

		error_encountered = true;
	}

	void ParserWarning(const std::string_view &message)
	{
		if (warnings_enabled)
		{
			TextOutput_Format(error_stream, "Warning: {}", message);

			ErrorMessageCommon();
		}
	}

	void ParserWarningPedantic(const std::string_view &message)
	{
		if (pedantic_warnings_enabled)
			ParserWarning(message);
	}

	void ParserError(const std::string_view &message)
	{
		TextOutput_Format(error_stream, "Error: {}", message);

		ErrorMessageCommon();

		error_encountered = true;
	}

	bool GetErrorEncountered() const
	{
		return error_encountered;
	}

	ErrorManager(
		std::ostream &error_stream,
		const std::deque<Location> &locations,
		const std::string_view &source_line,
		const bool &warnings_enabled,
		const bool &pedantic_warnings_enabled
	)
		: error_stream(error_stream)
		, locations(locations)
		, source_line(source_line)
		, warnings_enabled(warnings_enabled)
		, pedantic_warnings_enabled(pedantic_warnings_enabled)
	{}
};

#endif /* ERROR_H */
