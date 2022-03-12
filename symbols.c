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
	SymbolType type;
	unsigned long value;
} Symbol;

static Symbol *symbol_table['z' - 'A']; /* Identifiers usually only start with letters A-Z/a-z. */

static Symbol** GetBucket(const char *identifier)
{
	return &symbol_table[identifier[0] % CC_COUNT_OF(symbol_table)];
}

static Symbol** FindSymbol(const char *identifier)
{
	Symbol **symbol;

	for (symbol = GetBucket(identifier); *symbol != NULL; symbol = &(*symbol)->next)
		if (strcmp((*symbol)->identifier, identifier) == 0)
			return symbol;

	return NULL;
}

static cc_bool AddSymbol(const char *identifier, SymbolType type, unsigned long value)
{
	cc_bool success = cc_true;

	const size_t identifier_length = strlen(identifier) + 1;

	Symbol *symbol = malloc(sizeof(Symbol) + identifier_length);

	if (symbol == NULL)
	{
		fprintf(stderr, "Error: Could not allocate memory for symbol\n");
		success = cc_false;
	}
	else
	{
		Symbol **bucket = GetBucket(identifier);

		symbol->next = *bucket;
		symbol->identifier = (char*)(symbol + 1);
		symbol->type = type;
		symbol->value = value;

		*bucket = symbol;

		memcpy(symbol->identifier, identifier, identifier_length);
	}

	return success;
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

cc_bool SetSymbol(const char *identifier, SymbolType type, unsigned long value)
{
	cc_bool success = cc_true;

	Symbol **existing_symbol = FindSymbol(identifier);

	switch (type)
	{
		case SYMBOL_CONSTANT:
			if (existing_symbol != NULL)
			{
				fprintf(stderr, "Error: Symbol already defined\n");
				success = cc_false;
			}
			else
			{
				if (!AddSymbol(identifier, type, value))
					success = cc_false;
			}

			break;

		case SYMBOL_VARIABLE:
			if (existing_symbol != NULL)
			{
				if ((*existing_symbol)->type != SYMBOL_VARIABLE)
				{
					fprintf(stderr, "Error: Symbol redefined with different type\n");
					success = cc_false;
				}
				else
				{
					(*existing_symbol)->value = value;
				}
			}
			else
			{
				if (!AddSymbol(identifier, type, value))
					success = cc_false;
			}

			break;
	}

	return success;
}

cc_bool UnsetSymbol(const char *identifier)
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

cc_bool GetSymbol(const char *identifier, unsigned long *value)
{
	cc_bool success = cc_true;

	Symbol **symbol = FindSymbol(identifier);

	if (symbol == NULL)
		success = cc_false;
	else
		*value = (*symbol)->value;

	return success;
}
