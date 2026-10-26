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

#include "options.h"
#include "utilities/io.h"

class ErrorManager
{
public:
	struct Location
	{
		std::string file_path; /* TODO: This should really be a `std::string_view`, but fix-ups need deep copies... */
		unsigned long line_number = 0;

		Location(std::string file_path, const unsigned long line_number = 0)
			: file_path(std::move(file_path))
			, line_number(line_number)
		{}

		Location(const std::string_view &file_path, const unsigned long line_number = 0)
			: file_path(file_path)
			, line_number(line_number)
		{}
	};

private:
	std::ostream &error_stream;
	const std::deque<Location> &locations;
	const std::string_view &source_line;
	const OptionsStack &options;
	bool error_encountered = false;

	void ErrorMessageCommon()
	{
		for (const auto &location : locations)
			IO::PrintLine(error_stream, "On line {} of '{}'...", location.line_number, location.file_path);

		IO::PrintLine(error_stream, "\n{}\n", source_line.empty() ? "[No source line]" : source_line);
	}

public:
	template<typename... Args>
	void Information(std::format_string<Args...> &&format_string, Args &&...args)
	{
		error_stream << "Information: ";
		IO::PrintLine(error_stream, std::forward<std::format_string<Args...>>(format_string), std::forward<Args>(args)...);
	}

	void Information(const std::string_view &message)
	{
		IO::PrintLine(error_stream, "Information: {}", message);
	}

	template<typename... Args>
	void Warning(std::format_string<Args...> &&format_string, Args &&...args)
	{
		if (options->warnings_enabled)
		{
			IO::PrintWarning(error_stream, std::forward<std::format_string<Args...>>(format_string), std::forward<Args>(args)...);

			ErrorMessageCommon();
		}
	}

	void Warning(const std::string_view &message)
	{
		if (options->warnings_enabled)
		{
			IO::PrintWarning(error_stream, message);

			ErrorMessageCommon();
		}
	}

	template<typename... Args>
	void WarningPedantic(std::format_string<Args...> &&format_string, Args &&...args)
	{
		if (options->pedantic_warnings_enabled)
			Warning(std::forward<std::format_string<Args...>>(format_string), std::forward<Args>(args)...);
	}

	void WarningPedantic(const std::string_view &message)
	{
		if (options->pedantic_warnings_enabled)
			Warning(message);
	}

	template<typename... Args>
	void Error(std::format_string<Args...> &&format_string, Args &&...args)
	{
		IO::PrintError(error_stream, std::forward<std::format_string<Args...>>(format_string), std::forward<Args>(args)...);

		ErrorMessageCommon();

		error_encountered = true;
	}

	void Error(const std::string_view &message)
	{
		IO::PrintError(error_stream, message);

		ErrorMessageCommon();

		error_encountered = true;
	}

	bool WasErrorEncountered() const
	{
		return error_encountered;
	}

	ErrorManager(
		std::ostream &error_stream,
		const std::deque<Location> &locations,
		const std::string_view &source_line,
		const OptionsStack &options
	)
		: error_stream(error_stream)
		, locations(locations)
		, source_line(source_line)
		, options(options)
	{}
};

#endif /* ERROR_H */
