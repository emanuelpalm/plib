#include "trie.h"

#include <stdlib.h>
#include <string.h>

static const unsigned EMPTY = 0;

int _resize(trie_t *t, const unsigned size)
{
    trie_node_t *mem = realloc(t->nodes, size * sizeof(trie_node_t));
    if (mem == NULL) {
        return -1;
    }
    t->nodes = mem;
    t->limit = size;
    return 0;
}

unsigned _new_node(trie_t *t, const int c)
{
    if (t->amount == t->limit) {
        if (_resize(t, t->limit * 2) == -1) {
            return EMPTY;
        }
    }

    trie_node_t *node = &t->nodes[t->amount];
    node->character = c;
    node->index_next = EMPTY;
    node->index_alt = EMPTY;

    return t->amount++;
}

trie_t *trie_create(const unsigned initial_node_capacity)
{
    trie_t *t = malloc(sizeof(trie_t));
    if (t == NULL) {
        return NULL;
    }
    t->nodes = NULL;
    t->amount = 0;
    if (_resize(t, initial_node_capacity) == -1) {
        free(t);
        return NULL;
    }
    _new_node(t, 'a'); // Any common starting character will do.
    return t;
}

void trie_destroy(trie_t *t)
{
    free(t->nodes);
    free(t);
}

void *_put(trie_t *t, const unsigned index, const char *k, const void *v)
{
    trie_node_t *n = &t->nodes[index];

    if (*k == '\0') {
        n->value = v;
        return (void *) v;
    }
    if (n->character == *k) {
        if (n->index_next == EMPTY) {
            unsigned index_next = _new_node(t, k[1]);
            if (index_next == EMPTY) return NULL;
            t->nodes[index].index_next = index_next;
        }
        return _put(t, t->nodes[index].index_next, &k[1], v);

    } else {
        if (n->index_alt == EMPTY) {
            unsigned index_alt = _new_node(t, *k);
            if (index_alt == EMPTY) return NULL;
            t->nodes[index].index_alt = index_alt;
        }
        return _put(t, t->nodes[index].index_alt, k, v);
    }
}

void *trie_put(trie_t *t, const char *key, const void *value)
{
    if (key[0] == '\0') {
        return NULL;
    }
    return _put(t, 0, key, value);
}

void *_get(const trie_t *t, unsigned index, const char *k)
{
    trie_node_t *n = &t->nodes[index];

    if (*k == '\0') {
        return (void *) n->value;
    }
    if (n->character == *k) {
        if (n->index_next == EMPTY) return NULL;
        return _get(t, n->index_next, &k[1]);

    } else {
        if (n->index_alt == EMPTY) return NULL;
        return _get(t, n->index_alt, k);
    }
}

void *trie_get(const trie_t *t, const char *key)
{
    if (key[0] == '\0') {
        return NULL;
    }
    return _get(t, 0, key);
}

/*
Internal string representation.
*/
typedef struct {
    char *origin, *offset;
    const char *end;
} _string_t;

int _string_resize(_string_t *str, unsigned capacity)
{
    void *mem = realloc(str->origin, capacity);
    if (mem == NULL) {
        return -1;
    }

    unsigned size = str->offset - str->origin;
    str->origin = mem;
    str->offset = &str->origin[size];
    str->end = &str->origin[capacity];
    return 0;
}

int _string_alloc(_string_t *str, unsigned initial_capacity)
{
    str->origin = NULL;
    str->offset = NULL;
    return _string_resize(str, initial_capacity);
}

int _string_append(_string_t *str, char c)
{
    if (str->offset == str->end) {
        unsigned capacity = (str->end - str->origin) * 2;
        if (_string_resize(str, capacity) == -1) {
            return -1;
        }
    }
    *(str->offset)++ = c;
    return 0;
}

int _suggest(const trie_t *t, unsigned index, const char *k, _string_t *r)
{
    const trie_node_t *n = &t->nodes[index];

    if (n->character == '\0') {
        return _string_append(r, '\0');
    }
    if (*k == '\0') {
        if (_string_append(r, n->character) == -1) return -1;
        return _suggest(t, n->index_next, k, r);
    }
    if (n->character == *k) {
        if (_string_append(r, n->character) == -1) return -1;
        return _suggest(t, n->index_next, &k[1], r);
    }
    if (n->index_alt == EMPTY) {
        if (_string_append(r, n->character) == -1) return -1;
        return _suggest(t, n->index_next, k, r);
    }
    return _suggest(t, n->index_alt, k, r);
}

char *trie_suggest(const trie_t *t, const char *key)
{
    _string_t result;
    if (_string_alloc(&result, 16) == -1) {
        return NULL;
    }
    if (_suggest(t, 0, key, &result) == -1) {
        free(result.origin);
        return NULL;
    }
    return result.origin;
}

trie_t *trie_copy(const trie_t *t, trie_t *target)
{
    if (target->limit < t->amount) {
        if (_resize(target, t->amount) == -1) {
            return NULL;
        }
    }
    memcpy(target->nodes, t->nodes, t->amount * sizeof(trie_node_t));
    target->amount = t->amount;
    return target;
}
