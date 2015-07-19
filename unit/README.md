# UNIT - Palm's C Unit Testing Framework

Simplistic testing framework that divides tests into suites.

## Building

Make sure that your testing binary is built with the files `unit.h` and
`unit.c`, which both reside in this folder.

Define the macro constant `UNIT_NO_COLOR` to disable colored output.

## Using

Please read [unit.h](unit.h) for function documentation.

### Examples

#### Basic Usage

Below a single suite, containing one test, is declared and executed.

````c
#include <string.h>
#include "unit.h"

void test_one_is_one(T_t *T, void *_)
{
    if (1 != 1) {
        unit_fail(T, "1 != 1");
    }
    if (strcmp("one", "one") != 0) {
        unit_failf(T, "one != %s", "one");
    }

    unit_skip(T, "Skipping before fatal.");
    unit_fatal(T, "Would skip suite and continue with next, if any.");
}

void suite_oneness(T_t *T)
{
    unit_run_test(T, &test_one_is_one, NULL);
}

int main()
{
    unit_t u;
    unit_init(&u);
    unit_run_suite(&u, "oneness", &suite_oneness);
    unit_exit(&u);
}
````

#### Using Providers

Provider functions may be used to perform setup and cleanup before and after
designated unit tests, as well as provide data to those tests.

````c
#include <string.h>
#include "unit.h"

// Provider function. Allocates memory block and passes it on to test.
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

void suite_cpy(T_t *T)
{
    // Reference to provider function given to tests.
    unit_run_test(T, &test_strcpy_hello, &provider_block);
    unit_run_test(T, &test_strcpy_world, &provider_block);
}

int main()
{
    unit_t u;
    unit_init(&u);
    unit_run_suite(&u, "cpy", &suite_cpy);
    unit_exit(&u);
}
````

## Conventions

### Function Names

__Suite__ function names should be prefixed with `suite_`, __test__ function
names with `test_` and __provider__ function names with `provider_`.

### Variable Names

The `T_t` reference passed to each suite, test and provider, ought to always be
named `T`.

If the `void *` value passed on to each test is left unused, it ought to be
named `_`.

### File Endings

In order to explicitly distinguish unit test files from regular C source and
header files, the convention is to suffix them with `.unit.c` or `.unit.h`.

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
before making a pull request. The idea is for UNIT to remain minimal. Features
which provide a significant boost in utility, but only require a small increase
in complexity, are most welcome.
