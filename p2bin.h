/*
 * Copyright (C) 2023-2025 Clownacy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef P2BIN_H
#define P2BIN_H

#include <sstream>

#include "clowncommon/clowncommon.h"

#include "io.h"

cc_bool ConvertObjectFileToFlatBinary(std::istream &input_stream, std::ostream &output_stream, std::ostream &error_stream);

#endif /* P2BIN_H */
