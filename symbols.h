#ifndef SYMBOLS_H
#define SYMBOLS_H

#include "clowncommon.h"

typedef enum SymbolType
{
	SYMBOL_CONSTANT,
	SYMBOL_VARIABLE
} SymbolType;

typedef struct Symbol
{
	struct Symbol *next;

	char *identifier;
	SymbolType type;
	unsigned long value;
} Symbol;

typedef struct SymbolState
{
	Symbol *symbol_table[0x100];
} SymbolState;

void ClearSymbols(SymbolState *state);
cc_bool SetSymbol(SymbolState *state, const char *identifier, SymbolType type, unsigned long value);
cc_bool UnsetSymbol(SymbolState *state, const char *identifier);
cc_bool GetSymbol(SymbolState *state, const char *identifier, unsigned long *value);

#endif  /* SYMBOLS_H */
