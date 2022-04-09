#include "strcmpci.h"

#include <ctype.h>
#include <stddef.h>

int strcmpci(const char *lhs, const char *rhs)
{
	int delta;

	delta = 0;

	for (;;)
	{
		const int lhs_character = tolower((unsigned char)*lhs++);
		const int rhs_character = tolower((unsigned char)*rhs++);

		delta = lhs_character - rhs_character;

		if (delta != 0 || (lhs_character == '\0' /*&& rhs_character == '\0'*/))
			break;
	}

	return delta;
}

int strncmpci(const char *lhs, const char *rhs, size_t count)
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

