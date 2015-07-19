#include <string.h>
#include "unit.h"

void suite_oneness(T_t *T);
void suite_cpy(T_t *T);

int main()
{
    unit_t u;
    unit_init(&u);
    unit_run_suite(&u, "oneness", &suite_oneness);
    unit_run_suite(&u, "cpy", &suite_cpy);
    unit_exit(&u);
}

void test_1_is_1(T_t *T, void *_);
void test_one_is_1(T_t *T, void *_);

void suite_oneness(T_t *T)
{
    unit_run_test(T, &test_1_is_1, NULL);
    unit_run_test(T, &test_one_is_1, NULL);
}

void test_1_is_1(T_t *T, void *_)
{
    if (1 != 1) {
        unit_failf(T, "%d != %d", 1, 1);
    }
}

void test_one_is_1(T_t *T, void *_)
{
    if (strcmp("one", "1") == 0) {
        unit_fail(T, "one == 1");
    }
}

void test_strcpy_hello(T_t *T, void *block);
void test_strcpy_world(T_t *T, void *block);
void provider_block(T_t *T, unit_test_t test);

void suite_cpy(T_t *T)
{
    unit_run_test(T, &test_strcpy_hello, &provider_block);
    unit_run_test(T, &test_strcpy_world, &provider_block);
}

void test_strcpy_hello(T_t *T, void *block)
{
    strcpy((char *) block, "hello");
    unit_assert(T,
                strcmp(block, "hello") == 0
               );
}

void test_strcpy_world(T_t *T, void *block)
{
    strcpy((char *) block, "world");
    unit_assert(T,
                strcmp(block, "world") == 0
               );
}

void provider_block(T_t *T, unit_test_t test)
{
    void *block = malloc(16);
    if (block == NULL) {
        unit_skip(T, "malloc(16) failed");
        return;
    }
    test(T, block);
    free(block);
}
