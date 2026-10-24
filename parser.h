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

#ifndef PARSER_H
#define PARSER_H

#include "error.h"
#include "parser/syntactic.h"
#define YY_NO_UNISTD_H
#include "parser/lexical.h"

class Parser
{
private:
	class Flex
	{
	private:
		yyscan_t internal;

	public:
		Flex()
		{
			if (m68kasm_lex_init(&internal) != 0)
				throw std::runtime_error("m68kasm_lex_init failed.");
		}

		~Flex()
		{
			m68kasm_lex_destroy(internal);
		}

		Flex(const Flex &other) = delete;
		Flex& operator=(const Flex &other) = delete;

		yyscan_t& operator*()
		{
			return internal;
		}
	};

	Flex flex_state;
	Statement statement;
	m68kasm::parser bison;

public:
	Parser(ErrorManager &error_manager)
		: bison(*flex_state, error_manager, statement)
	{}

	Statement* ParseStatement(const std::string_view &view)
	{
		/* Parse the source line with Flex and Bison (Lex and Yacc). */
		const YY_BUFFER_STATE buffer = m68kasm__scan_bytes(view.data(), view.length(), *flex_state);
		const int parse_result = bison.parse();
		m68kasm__delete_buffer(buffer, *flex_state);

		return parse_result != 0 ? nullptr : &statement;
	}
};

#endif /* PARSER_H */
