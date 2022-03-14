#include "symbols.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "clowncommon.h"

static Symbol** GetBucket(SymbolState *state, const char *identifier)
{
	unsigned int hash, character;

	/* Hash the identifier, using djb2. */
	/* http://www.cse.yorku.ca/~oz/hash.html */
	hash = 5381;

	while ((character = (unsigned int)*identifier++) != '\0')
		hash = hash * 33 + character;

	return &state->symbol_table[hash % CC_COUNT_OF(state->symbol_table)];
}

static Symbol** FindSymbol(SymbolState *state, const char *identifier)
{
	Symbol **symbol;

	for (symbol = GetBucket(state, identifier); *symbol != NULL; symbol = &(*symbol)->next)
		if (strcmp((*symbol)->identifier, identifier) == 0)
			return symbol;

	return NULL;
}

static cc_bool AddSymbol(SymbolState *state, const char *identifier, SymbolType type, unsigned long value)
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
		Symbol **bucket = GetBucket(state, identifier);

		symbol->next = *bucket;
		symbol->identifier = (char*)(symbol + 1);
		symbol->type = type;
		symbol->value = value;

		*bucket = symbol;

		memcpy(symbol->identifier, identifier, identifier_length);
	}

	return success;
}

void ClearSymbols(SymbolState *state)
{
	Symbol **bucket;

	for (bucket = state->symbol_table; bucket < &state->symbol_table[CC_COUNT_OF(state->symbol_table)]; ++bucket)
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

cc_bool SetSymbol(SymbolState *state, const char *identifier, SymbolType type, unsigned long value)
{
	cc_bool success = cc_true;

	Symbol **existing_symbol = FindSymbol(state, identifier);

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
				if (!AddSymbol(state, identifier, type, value))
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
				if (!AddSymbol(state, identifier, type, value))
					success = cc_false;
			}

			break;
	}

	return success;
}

cc_bool UnsetSymbol(SymbolState *state, const char *identifier)
{
	cc_bool success = cc_true;

	Symbol **symbol = FindSymbol(state, identifier);

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

cc_bool GetSymbol(SymbolState *state, const char *identifier, unsigned long *value)
{
	cc_bool success = cc_true;

	Symbol **symbol = FindSymbol(state, identifier);

	if (symbol == NULL)
		success = cc_false;
	else
		*value = (*symbol)->value;

	return success;
}
