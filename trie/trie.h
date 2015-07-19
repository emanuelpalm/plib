/*!
Associative string/void pointer tree map.

Keeps track of a tree of nodes which together facilitate efficient lookup of
void pointer values related to given key strings.

@file trie.h
*/
#ifndef TRIE_H
#define TRIE_H

/*!
Represents a character and a void pointer value within a TRIE.
*/
typedef struct {
    int character;
    union {
        struct {
            unsigned index_next, index_alt;
        };
        const void *value; // Is only available if character is '\0'.
    };
} trie_node_t;

/*!
Represents all nodes of a TRIE, and the memory which house them.
*/
typedef struct {
    trie_node_t *nodes;
    unsigned amount, limit;
} trie_t;

/*!
Creates and returns new TRIE object with given initial node capacity. NULL is
returned in case of memory allocation failure.
*/
trie_t *trie_create(const unsigned initial_node_capacity);

/*!
Frees all memory kept by TRIE.
*/
void trie_destroy(trie_t *t);

/*!
Inserts given key/value pair to TRIE, replacing any previous value associated
with the same key.

Returns inserted value. NULL is returned in case of memory allocation failure,
or if attempting to put a value with an empty key.
*/
void *trie_put(trie_t *t, const char *key, const void *value);

/*!
Finds value associated with given key in TRIE.

Returns found value, or NULL in case no associated value could be found.
*/
void *trie_get(const trie_t *t, const char *key);

/*!
Suggests an existing key using the given key. NULL is returned in case of memory
allocation failure.

The string returned has to be destroyed using free() once no longer needed.
*/
char *trie_suggest(const trie_t *t, const char *key);

/*!
Copies all TRIE data from t into target, and returns target. NULL is returned in
case of memory allocation failure.
 */
trie_t *trie_copy(const trie_t *t, trie_t *target);

#endif
