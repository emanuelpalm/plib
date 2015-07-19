# DMEM - Dynamic Memory Manager

A type with associated functions for managing dynamically sized blocks of
memory.

## Building

Make sure that your binary is built with the files `dmem.h` and `dmem.c`, which
both reside in this folder. The module relies on either C11 or POSIX.1d
compatibility, and has to be compiled with such enabled (e.g. using the
`-std=c11` or `-D_POSIX_C_SOURCE=200112L` flags).

## Using

Please read [dmem.h](dmem.h) for function documentation.

### Example usage:

````c
#include "dmem.h"
#include <stdlib.h>

struct car {
    enum {
        BLACK, RED, GREEN, BLUE, YELLOW, WHITE
    } color;
    int model;
};

int main()
{
    // Initializes car_mem with enough room to house a single car.
    dmem_t *car_mem = dmem_create(sizeof(struct car));
    
    // Note that dmem_alloc() returns a pointer to uninitialized memory.
    struct car *c0 = dmem_alloc(car_mem, sizeof(struct car));
    if (c0 == NULL) {
        exit(EXIT_FAILURE);
    }
    c0->color = BLACK;
    c0->model = 2014;

    // This call normally succeeds even though car_mem initially was too small.
    struct car *c1 = dmem_alloc(car_mem, sizeof(struct car));
    if (c1 == NULL) {
        exit(EXIT_FAILURE);
    }
    c1->color = WHITE;
    c1->model = 1969;

    /* Do something with cars. */

    // One call is enough to free all cars, as their data reside in a single
    // block of memory.
    dmem_destroy(car_mem);

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
before making a pull request. The idea is for DMEM to remain minimal. Features
which provide a significant boost in utility, but only require a small increase
in complexity, are most welcome.
