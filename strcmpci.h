/*
 * Copyright (C) 2022 Clownacy
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

#ifndef STRCMPCI_H
#define STRCMPCI_H

#include <stddef.h>

typedef int (*MemoryComparisionFunction)(const void *lhs, const void *rhs, size_t count);

#define strcmpci(lhs, rhs) strncmpci(lhs, rhs, (size_t)-1)
int strncmpci(const char *lhs, const char *rhs, size_t count);

int memcasecmp(const void *lhs, const void *rhs, size_t count);

#endif /* STRCMPCI_H */
