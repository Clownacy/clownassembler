/*
 * Copyright (C) 2022 Clownacy
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

#include "strcmpci.h"

#include <ctype.h>
#include <stddef.h>

int strncmpci(const char *lhs, const char *rhs, const size_t count)
{
	int delta;
	size_t i;

	delta = 0;

	for (i = 0; i < count; ++i)
	{
		const int lhs_character = tolower((unsigned char)*lhs++);
		const int rhs_character = tolower((unsigned char)*rhs++);

		delta = lhs_character - rhs_character;

		if (delta != 0 || (lhs_character == '\0' /*&& rhs_character == '\0'*/))
			break;
	}

	return delta;
}

/* Like strcasecmp, but doesn't check for null characters. */
int memcasecmp(const void *lhs, const void *rhs, size_t count)
{
	const unsigned char *lhs_uc;
	const unsigned char *rhs_uc;
	int delta;
	size_t i;

	lhs_uc = (const unsigned char*)lhs;
	rhs_uc = (const unsigned char*)rhs;
	delta = 0;

	for (i = 0; i < count; ++i)
	{
		const int lhs_character = tolower(*lhs_uc++);
		const int rhs_character = tolower(*rhs_uc++);

		delta = lhs_character - rhs_character;

		if (delta != 0)
			break;
	}

	return delta;
}
