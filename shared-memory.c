#include "shared-memory.h"

#include <stdlib.h>

typedef struct Header
{
	union
	{
		unsigned int reference_count;
		/* The biggest types that I can think of. */
		long padding_long;
		long double padding_long_double;
		size_t padding_size;
		void *padding_pointer;
	} u;
} Header;

static Header* GetHeader(void* const memory)
{
	return (Header*)memory - 1;
}

void* SharedMemory_Allocate(const size_t size)
{
	Header* const header = (Header*)malloc(sizeof(Header) + size);

	if (header == NULL)
		return NULL;

	header->u.reference_count = 1;

	return header + 1; /* Point past header. */
}

void SharedMemory_Free(void *memory)
{
	if (memory != NULL)
	{
		Header* const header = GetHeader(memory);

		if (--header->u.reference_count == 0)
			free(header);
	}
}

void* SharedMemory_Copy(void *memory)
{
	if (memory != NULL)
		++GetHeader(memory)->u.reference_count;

	return memory;
}

cc_bool SharedMemory_WillBeDestroyed(void *memory)
{
	if (memory == NULL)
		return cc_false;

	return GetHeader(memory)->u.reference_count == 1;
}
