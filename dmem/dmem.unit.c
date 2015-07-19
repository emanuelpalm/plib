#include <../unit/unit.h>
#include <string.h>

/*
As all memory management is aligned, the alignment specifier is fixed here to
make test results more predictable.
*/
#define DMEM_ALIGNMENT 8
#include "dmem.h"

void test_alloc(T_t *T, void *m);
void test_alloc_beyond_capacity(T_t *T, void *m);
void test_alloc_larger_than_block_size(T_t *T, void *m);
void test_reset(T_t *T, void *m);
void test_reset_multiblock(T_t *T, void *m);

void provider_dmem(T_t *T, unit_test_t test);

void suite_dmem(T_t *T)
{
    unit_run_test(T, &test_alloc, &provider_dmem);
    unit_run_test(T, &test_alloc_beyond_capacity, &provider_dmem);
    unit_run_test(T, &test_alloc_larger_than_block_size, &provider_dmem);
    unit_run_test(T, &test_reset, &provider_dmem);
    unit_run_test(T, &test_reset_multiblock, &provider_dmem);
}

int main()
{
    unit_t u;
    unit_init(&u);
    unit_run_suite(&u, "dmem", &suite_dmem);
    unit_exit(&u);
}

void test_alloc(T_t *T, void *m)
{
    int *v0 = dmem_alloc(m, sizeof(int));
    if (v0 == NULL) {
        unit_fail(T, "v0 == NULL");
        return;
    }
    
    int *v1 = dmem_alloc(m, sizeof(int));
    if (v1 == NULL) {
        unit_fail(T, "v1 == NULL");
        return;
    }

    *v0 = 1;
    *v1 = 2;

    unit_assert(T, *v0 != *v1);
    unit_assert(T, v0 != v1);
}

void test_alloc_beyond_capacity(T_t *T, void *m)
{
    const size_t sizes[] = {32, 24, 49, 0};
    for (const size_t *size = sizes; *size != 0; ++size) {

        for (size_t i = 0; i < 700; ++i) {
            void *v = dmem_alloc(m, *size);
            if (v == NULL) {
                unit_failf(T, "v == NULL (size: %zu, i: %zu)", *size, i);
                return;
            }
            // Try to cause segmentation fault.
            memset(v, '\0', *size);
        }

    }
}

void test_alloc_larger_than_block_size(T_t *T, void *m)
{
    int *v0 = dmem_alloc(m, 1024);
    if (v0 == NULL) {
        unit_fail(T, "v0 == NULL");
        return;
    }
    
    int *v1 = dmem_alloc(m, 777);
    if (v1 == NULL) {
        unit_fail(T, "v1 == NULL");
        return;
    }

    *v0 = 1;
    *v1 = 2;

    unit_assert(T, *v0 != *v1);
    unit_assert(T, v0 != v1);
}

void test_reset(T_t *T, void *m)
{
    int *v0 = dmem_alloc(m, sizeof(int));
    if (v0 == NULL) {
        unit_fail(T, "v0 == NULL");
        return;
    }
    
    dmem_reset(m);

    int *v1 = dmem_alloc(m, sizeof(int));
    if (v1 == NULL) {
        unit_fail(T, "v1 == NULL");
        return;
    }

    *v0 = 1;
    *v1 = 2;

    // As memory was reset before allocating v1, it should point to the same
    // memory location as v0, making them both 2 after the v1 assignment.
    unit_assert(T, *v0 == *v1);
    unit_assert(T, v0 == v1);
}

void test_reset_multiblock(T_t *T, void *m)
{
    for (size_t i = 0; i < 2000; ++i) {
        void *v = dmem_alloc(m, 32 + i);
        if (v == NULL) {
            unit_failf(T, "v == NULL (i: %zu)", i);
            return;
        }
        memset(v, '\0', 32 + i);
    }

    dmem_reset(m);

    // Make sure all blocks are properly reset.
    size_t i = 0;
    dmem_block_t *b = ((dmem_t *) m)->blocks;
    for (; b != NULL; b = b->next, ++i) {
        if (b->origin != b->offset) {
            unit_failf(T, "b->origin != b->offset (i: %zu)", i);
            return;
        }
    }
}

void assert_dmem_integrity(T_t *T, const dmem_t *m);

void provider_dmem(T_t *T, unit_test_t test)
{
    dmem_t *m = dmem_create(64);
    if (m == NULL) {
        unit_fatal(T, "m == NULL");
    }
    assert_dmem_integrity(T, m);
    test(T, m);
    assert_dmem_integrity(T, m);
    dmem_destroy(m);
}

void assert_dmem_integrity(T_t *T, const dmem_t *m)
{
    if (m->blocks == NULL) {
        unit_fail(T, "m->blocks == NULL");
        return;
    }
    if (m->block_size == 0) {
        unit_fail(T, "m->block_size == 0");
        return;
    }

    for (dmem_block_t *b = m->blocks; b != NULL; b = b->next) {
        if (b->origin > b->offset) {
            unit_fail(T, "b->origin > b->offset");
            return;
        }
        if (b->origin > b->end) {
            unit_fail(T, "b->origin > b->end");
            return;
        }
        if (b->offset > b->end) {
            unit_fail(T, "b->offset > b->end");
            return;
        }
    }
}
