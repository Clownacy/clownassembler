#ifndef P2BIN_H
#define P2BIN_H

#include <stdio.h>

#include "clowncommon/clowncommon.h"

cc_bool ConvertObjectFileToFlatBinary(FILE *input_file, FILE *output_file);

#endif /* P2BIN_H */
