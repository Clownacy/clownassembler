#ifndef P2BIN_H
#define P2BIN_H

#include <stdio.h>

#include "clowncommon/clowncommon.h"

#include "io.h"

cc_bool ConvertObjectFileToFlatBinary(FILE *input_file, const ClownAssembler_BinaryOutput *output_file);

#endif /* P2BIN_H */
