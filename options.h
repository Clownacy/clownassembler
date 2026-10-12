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

#ifndef OPTIONS_H
#define OPTIONS_H

#include <stack>

#include "clowncommon/clowncommon.h"

struct Options
{
	char local_signifier;
	bool debug;
	bool case_insensitive;
	bool equ_set_descope_local_labels;
	bool output_local_labels_to_sym_file;
	bool warnings_enabled;
	bool pedantic_warnings_enabled;
	bool expand_all_macros;
	bool automatic_even;
	struct
	{
		bool zero_displacement;
	} optimisation;
};

class OptionsStack
{
private:
	std::stack<Options> stack;

public:
	OptionsStack(Options default_options)
	{
		stack.emplace(std::move(default_options));
	}

	void Push()
	{
		stack.push(stack.top());
	}

	void Pop()
	{
		stack.pop();
	}

	Options& operator*()
	{
		return stack.top();
	}

	const Options& operator*() const
	{
		return stack.top();
	}

	Options* operator->()
	{
		return &stack.top();
	}

	const Options* operator->() const
	{
		return &stack.top();
	}
};

#endif /* OPTIONS_H */
