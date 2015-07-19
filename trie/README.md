# TRIE - Ordered Tree Map

A type with associated functions for managing association between string keys
and void pointer values.

## Building

Make sure that your binary is built with the files `trie.h` and `trie.c`, which
both reside in this folder.

## Using

Please read [trie.h](trie.h) for function documentation.

### Example usage:

````c
#include "trie.h"
#include <stdlib.h>

int main() {
	trie_t *t = trie_create(16);

    int *values = { 1, 2, 3 };
    trie_put(t, "monkey", &values[0]);
    trie_put(t, "moon", &values[1]);
    trie_put(t, "donut", &values[2]);

    int *v = trie_get(t, "moon");
    // Do something with v.

    const char *suggestion = trie_suggest(t, "moan");
    // 'suggestion' now contains "moon".
    free(suggestion);

    trie_destroy(t);

	exit(EXIT_SUCCESS);
}
````

## Contributing

Contributions are made through [GitHub](http://www.github.com/emanuelpalm/plib).

### Style Guide

The K&R style is used, with soft tabs (using 4 spaces per tab). The AStyle
formatter using the `--style=kr` option is recommended for consistency.

Try to keep lines within a with of 80 characters. A width of more than 100
characters is forbidden.

Comments are as brief as possible and only contain text. Avoid using special
annotations. Comments above function and type declarations in header files use
the `/*! */` pattern, without `*` in the beginning of each new row. The
exclamation mark is omitted for source and test files.

### Bug Fixes

Pull requests for fixing bugs are always welcome. Please submit them with a
description of the fixed bug.

### New Features

Please create an issue with a discussion about the feature you which to add
before making a pull request. The idea is for TRIE to remain minimal. Features
which provide a significant boost in utility, but only require a small increase
in complexity, are most welcome.
