/*!
Dynamic memory block management.

These utilities provide efficient memory management in situations with the
following requirements:

1. The final amount of needed memory isn't known.
2. All memory data will be freed or reset in one go.
3. There may be pointers into the memory managed.

@file dmem.h
*/
#ifndef DMEM_H
#define DMEM_H

#include <stdlib.h>

#ifndef DMEM_ALIGNMENT
#define DMEM_ALIGNMENT sizeof(void *)
#endif

/*!
Represents a dynamically sized block of bytes.
*/
typedef struct dmem_block {
    char *origin, *offset;
    const char *end;

    struct dmem_block *next;
} dmem_block_t;

/*!
Represents a collection of related memory blocks.
*/
typedef struct {
	dmem_block_t *blocks;
	size_t block_size;
} dmem_t;

/*!
Creates a new DMEM object, keeping track of block of given size.

On most systems, keeping block size a multiple of the virtual memory page size
yields more optimal performance.

Returns NULL in case of memory allocation error.
*/
dmem_t *dmem_create(size_t block_size);

/*!
Destroys given DMEM object, freeing any memory owned by it.
*/
void dmem_destroy(dmem_t *m);

/*!
Allocates size bytes of memory and returns a pointer to it. If a new block was
required, but allocating such failed, NULL is returned.

The returned memory is guaranteed to be valid until the next call of either
dmem_reset() or dmem_free() on given DMEM object.
*/
void *dmem_alloc(dmem_t *m, size_t size);

/*!
Resets DMEM object, invalidating all memory, previously distributed using the
dmem_get() function.
*/
void dmem_reset(dmem_t *m);

#endif
