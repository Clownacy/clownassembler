#ifndef SEMANTIC_H
#define SEMANTIC_H

#include <stdio.h>

#include "clowncommon.h"

#include "syntactic.h"

cc_bool ProcessParseTree(FILE *output_file, const StatementListNode *statement_list);

#endif /* SEMANTIC_H */
