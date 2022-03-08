#ifndef SYMBOLS_H
#define SYMBOLS_H

#include "clowncommon.h"

typedef enum SymbolType
{
	SYMBOL_CONSTANT,
	SYMBOL_VARIABLE
} SymbolType;

void ClearSymbols(void);
cc_bool SetSymbol(const char *identifier, SymbolType type, unsigned long value);
cc_bool UnsetSymbol(const char *identifier);
cc_bool GetSymbol(const char *identifier, unsigned long *value);

#endif  /* SYMBOLS_H */
