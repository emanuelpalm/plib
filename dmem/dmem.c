#include "dmem.h"

#if defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L
void *_alloc(size_t size)
{
    void *result = NULL;
    if (posix_memalign(&result, DMEM_ALIGNMENT, size) != 0) {
        return NULL;
    }
    return result;
}
#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
#define _alloc(size) aligned_alloc(DMEM_ALIGNMENT, (size))
#else
#error "Build with >= POSIX.1d, or C11 support."
#endif

size_t _align(const size_t n)
{
    return (n & (DMEM_ALIGNMENT - 1))
           ? (n & ~(DMEM_ALIGNMENT - 1)) + DMEM_ALIGNMENT
           : n;
}

dmem_block_t *_block_create(size_t block_size)
{
    dmem_block_t *b = _alloc(sizeof(dmem_block_t) + _align(block_size));
    if (b == NULL) {
        return NULL;
    }
    b->origin = (void *) &b[1];
    b->offset = b->origin;
    b->end = &b->origin[block_size];
    b->next = NULL;

    return b;
}

dmem_t *dmem_create(size_t block_size)
{
    block_size = _align(block_size);
    dmem_t *m = _alloc(sizeof(dmem_t));
    if (m == NULL) {
        return NULL;
    }
    m->blocks = _block_create(block_size);
    if (m->blocks == NULL) {
        free(m);
        return NULL;
    }
    m->block_size = block_size;
    return m;
}

void dmem_destroy(dmem_t *m)
{
    dmem_block_t *b = m->blocks;
    while (b != NULL) {
        dmem_block_t *next = b->next;
        free(b);
        b = next;
    }
    free(m);
}

void *_single_block_alloc(dmem_t *m, const size_t size);
void *_block_alloc(dmem_t *m, const size_t size);

void *dmem_alloc(dmem_t *m, size_t size)
{
    size = _align(size);
    if (size > m->block_size / 2) {
        return _single_block_alloc(m, size);
    }
    return _block_alloc(m, size);
}

void *_single_block_alloc(dmem_t *m, const size_t size) {
    dmem_block_t *b = _block_create(size);
    if (b == NULL) {
        return NULL;
    }
    dmem_block_t *b0 = m->blocks->next;
    b->next = b0;
    m->blocks->next = b;
    return b->origin;
}

size_t _block_space_left(dmem_block_t *b);

void *_block_alloc(dmem_t *m, const size_t size)
{
    if (_block_space_left(m->blocks) < size) {
        dmem_block_t *b = _block_create(m->block_size);
        if (b == NULL) {
            return NULL;
        }
        b->next = m->blocks;
        m->blocks = b;
    }

    dmem_block_t *b = m->blocks;
    void *a = b->offset;
    b->offset += size;
    return a;
}

size_t _block_space_left(dmem_block_t *b)
{
    return b->end - b->offset;
}

void dmem_reset(dmem_t *m)
{
    dmem_block_t *b = m->blocks->next;
    while (b != NULL) {
        dmem_block_t *next = b->next;
        free(b);
        b = next;
    }
    m->blocks->offset = m->blocks->origin;
    m->blocks->next = NULL;
}
