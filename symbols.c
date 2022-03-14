#include "symbols.h"

#include <stddef.h>
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

static SymbolError AddSymbol(SymbolState *state, const char *identifier, SymbolType type, unsigned long value)
{
	SymbolError error = SYMBOL_ERROR_NONE;

	const size_t identifier_length = strlen(identifier) + 1;

	Symbol *symbol = malloc(sizeof(Symbol) + identifier_length);

	if (symbol == NULL)
	{
		error = SYMBOL_ERROR_OUT_OF_MEMORY;
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

	return error;
}

void InitSymbols(SymbolState *state)
{
	unsigned int i;

	for (i = 0; i < CC_COUNT_OF(state->symbol_table); ++i)
		state->symbol_table[i] = NULL;
}

void DeinitSymbols(SymbolState *state)
{
	ClearSymbols(state);
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

SymbolError SetSymbol(SymbolState *state, const char *identifier, SymbolType type, unsigned long value)
{
	SymbolError error = SYMBOL_ERROR_NONE;

	Symbol **existing_symbol = FindSymbol(state, identifier);

	switch (type)
	{
		case SYMBOL_CONSTANT:
			if (existing_symbol != NULL)
				error = SYMBOL_ERROR_CONSTANT_ALREADY_DEFINED;
			else
				error = AddSymbol(state, identifier, type, value);

			break;

		case SYMBOL_VARIABLE:
			if (existing_symbol != NULL)
			{
				if ((*existing_symbol)->type != SYMBOL_VARIABLE)
					error = SYMBOL_ERROR_VARIABLE_REDEFINED_WITH_DIFFERENT_TYPE;
				else
					(*existing_symbol)->value = value;
			}
			else
			{
				error = AddSymbol(state, identifier, type, value);
			}

			break;
	}

	return error;
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
