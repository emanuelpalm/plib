#include "trie.h"
#include <string.h>
#include <../unit/unit.h>

void assert_trie_integrity(T_t *T, const trie_t *t);
void assert_trie_suggest(T_t *T, trie_t *t, const char *in, const char *result);

void test_put(T_t *T, void *t);
void test_put_beyond_capacity(T_t *T, void *t);
void test_put_get(T_t *T, void *t);
void test_put_suggest(T_t *T, void *t);
void test_copy(T_t *T, void *t);

void provider_trie(T_t *T, unit_test_t test);

void suite_trie(T_t *T)
{
    unit_run_test(T, &test_put, &provider_trie);
    unit_run_test(T, &test_put_beyond_capacity, &provider_trie);
    unit_run_test(T, &test_put_get, &provider_trie);
    unit_run_test(T, &test_put_suggest, &provider_trie);
    unit_run_test(T, &test_copy, &provider_trie);
}

int main()
{
    unit_t u;
    unit_init(&u);
    unit_run_suite(&u, "trie", &suite_trie);
    unit_exit(&u);
}

void test_put(T_t *T, void *t)
{
    const char *value = "world";
    unit_assert(T, trie_put(t, "hello", value) == value);
}

void test_put_beyond_capacity(T_t *T, void *t)
{
    const char *values[] = {"v0", "v1", "v2"};
    unit_assert(T, trie_put(t, "k0", values[0]) == values[0]);
    unit_assert(T, trie_put(t, "k1", values[1]) == values[1]);
    unit_assert(T, trie_put(t, "k2", values[2]) == values[2]);
}

void test_put_get(T_t *T, void *t)
{
    const char *values[] = {"v0", "v1"};

    trie_put(t, "k0", values[0]);
    unit_assert(T, trie_get(t, "k0") == values[0]);

    trie_put(t, "k1", values[1]);
    unit_assert(T, trie_get(t, "k1") == values[1]);

    unit_assert(T, trie_get(t, "k0") == values[0]);
}

void test_put_suggest(T_t *T, void *t)
{
    const char *value = "VALUE";

    trie_put(t, "cat", value);
    trie_put(t, "dog", value);
    trie_put(t, "donkey", value);
    trie_put(t, "doodle", value);

    assert_trie_suggest(T, t, "caturday", "cat");
    assert_trie_suggest(T, t, "do", "dog");
    assert_trie_suggest(T, t, "doogle", "doodle");
}

void test_copy(T_t *T, void *t)
{
    const char *value = "VALUE";
    trie_put(t, "abcdefghijklmnopqrstuvwxyz", value);
    trie_put(t, "0123456789abcdef", value);
    trie_put(t, "abcdefgx", value);
    trie_put(t, "abc", value);

    trie_t *t0 = (trie_t *) t;
    trie_t *t1 = trie_create(2);
    if (t1 == NULL) {
        unit_fatal(T, "Failed to allocate memory for TRIE.");
    }

    if (trie_copy(t0, t1) == NULL) {
        unit_fail(T, "trie_copy(t0, t1) == NULL");
        trie_destroy(t1);
        return;
    }

    if (t0->amount != t1->amount) {
        unit_failf(T, "t0->amount %u != t1->amount %u", t0->amount, t1->amount);
    }
    for (unsigned i = t0->amount; i-- != 0;) {
        if (t0->nodes[i].character != t1->nodes[i].character) {
            unit_failf(T, "%4lu: %c != %c", i, t0->nodes[i].character, t1->nodes[i].character);
        }
        if (t0->nodes[i].index_next != t1->nodes[i].index_next) {
            unit_failf(T, "%4lu: %lu != %lu", i, t0->nodes[i].index_next, t1->nodes[i].index_next);
        }
        if (t0->nodes[i].index_alt != t1->nodes[i].index_alt) {
            unit_failf(T, "%4lu: %lu != %lu", i, t0->nodes[i].index_alt, t1->nodes[i].index_alt);
        }
        if (t0->nodes[i].value != t1->nodes[i].value) {
            unit_failf(T, "%4lu: %p != %p", i, t0->nodes[i].value, t1->nodes[i].value);
        }
    }
    trie_destroy(t1);
}

void provider_trie(T_t *T, unit_test_t test)
{
    trie_t *t = trie_create(2);
    if (t == NULL) {
        unit_fatal(T, "Failed to allocate memory for TRIE.");
    }

    assert_trie_integrity(T, t);
    test(T, t);
    assert_trie_integrity(T, t);

    trie_destroy(t);
}

void assert_trie_integrity(T_t *T, const trie_t *t)
{
    if (t->nodes == NULL) {
        unit_fatal(T, "t->nodes == NULL");
    }
    if (t->amount > t->limit) {
        unit_fatal(T, "t->amount > t->limit");
    }
}

void assert_trie_suggest(T_t *T, trie_t *t, const char *in, const char *result)
{
    char *actual_result = trie_suggest(t, in);
    unit_assert(T, actual_result != NULL);
    if (strcmp(result, actual_result) != 0) {
        unit_failf(T, "\"%s\" != \"%s\"", result, actual_result);
    }
    free(actual_result);
}
