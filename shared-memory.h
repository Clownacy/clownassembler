#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <stddef.h>

#include "clowncommon/clowncommon.h"

void* SharedMemory_Allocate(size_t size);
void SharedMemory_Free(void *memory);
void* SharedMemory_Copy(void *memory);
cc_bool SharedMemory_WillBeDestroyed(void *memory);

#endif /* SHARED_MEMORY_H */
