#ifndef SYMBOLS_H
#define SYMBOLS_H

#include "clowncommon.h"

void ClearSymbols(void);
cc_bool AddSymbol(const char *identifier, unsigned long value);
cc_bool RemoveSymbol(const char *identifier);
cc_bool ObtainSymbol(const char *identifier, unsigned long *value);

#endif  /* SYMBOLS_H */
