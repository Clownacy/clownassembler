#include "symbols.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "clowncommon.h"

typedef struct Symbol
{
	struct Symbol *next;

	char *identifier;
	unsigned long value;
} Symbol;

static Symbol *symbol_table['z' - 'A']; /* Identifiers can only start with letters A-Z/a-z. */

static Symbol** FindSymbol(const char *identifier)
{
	Symbol **symbol;

	for (symbol = &symbol_table[identifier[0] - 'A']; *symbol != NULL; symbol = &(*symbol)->next)
		if (strcmp((*symbol)->identifier, identifier) == 0)
			return symbol;

	return NULL;
}

void ClearSymbols(void)
{
	Symbol **bucket;

	for (bucket = symbol_table; bucket < &symbol_table[CC_COUNT_OF(symbol_table)]; ++bucket)
	{
		Symbol *symbol = *bucket;

		while (symbol != NULL)
		{
			Symbol *symbol_next = symbol->next;
			free(symbol);
			symbol = symbol_next;
		}

		*bucket = NULL;
	}
}

cc_bool AddSymbol(const char *identifier, unsigned long value)
{
	cc_bool success = cc_true;

	if (FindSymbol(identifier) != NULL)
	{
		fprintf(stderr, "Error: Symbol already defined\n");
		success = cc_false;
	}
	else
	{
		const size_t identifier_length = strlen(identifier) + 1;

		Symbol *symbol = malloc(sizeof(Symbol) + identifier_length);

		if (symbol == NULL)
		{
			fprintf(stderr, "Error: Could not allocate memory for symbol\n");
			success = cc_false;
		}
		else
		{
			Symbol **bucket = &symbol_table[identifier[0] - 'A'];

			symbol->next = *bucket;
			symbol->identifier = (char*)(symbol + 1);
			symbol->value = value;

			*bucket = symbol;

			memcpy(symbol->identifier, identifier, identifier_length);
		}
	}

	return success;
}

cc_bool RemoveSymbol(const char *identifier)
{
	cc_bool success = cc_true;

	Symbol **symbol = FindSymbol(identifier);

	if (symbol == NULL)
	{
		success = cc_false;
	}
	else
	{
		Symbol *next_symbol = (*symbol)->next;

		free(*symbol);

		*symbol = next_symbol;
	}

	return success;
}

cc_bool ObtainSymbol(const char *identifier, unsigned long *value)
{
	cc_bool success = cc_true;

	Symbol **symbol = FindSymbol(identifier);

	if (symbol == NULL)
		success = cc_false;
	else
		*value = (*symbol)->value;

	return success;
}
