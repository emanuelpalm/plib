/*!
Unit testing framework.

Simplistic testing framework which divides tests into suites.

@file unit.h
*/
#ifndef LIBP_UNIT_UNIT_H
#define LIBP_UNIT_UNIT_H

#include <setjmp.h>
#include <stdlib.h>

/*!
Global unit test context.

An instance of this type represents the results of all performed unit tests.
*/
typedef struct {
    struct {
        size_t total, failed;
    } suites;
} unit_t;

/*!
Unit test suite context.

An instance of this type is passed around to all tests within the same suite
in order for them to report on their results. The convention is to name
instances of this type T when passing them around.
*/
typedef struct {
    struct {
        size_t total, failed, skipped;
    } tests;
    struct {
        size_t failures;
    } test;
    jmp_buf jmp_skip, jmp_fatal;
} T_t;

/*!
Unit test suite function pointer type.
*/
typedef void (*unit_suite_t)(T_t *);

/*!
Unit test function pointer type.

The void pointer argument is only given if a provider function is used, and
it passes on anything of relevance.
*/
typedef void (*unit_test_t)(T_t *, void *);

/*!
Unit test provider function pointer type.
*/
typedef void (*unit_provider_t)(T_t *, unit_test_t);

/*!
Initializes unit test suite data structure.
*/
void unit_init(unit_t *u);

/*!
Reports on unit test suite results and terminates application.

The application is terminated with the EXIT_FAILURE exit code if any test
failed. In case all tests passed then EXIT_SUCCESS is used.
*/
void unit_exit(unit_t *u);

/*!
Prints name of given suite, runs suite, and reports on the suite results.
*/
void unit_run_suite(unit_t *u, const char *name, unit_suite_t suite);

/*!
Runs given test and stores any results into T.

If a provider function is given, it is run instead. It is called with a
reference to given test.
*/
void unit_run_test(T_t *T, unit_test_t test, unit_provider_t provider);

/*!
Fails current test, reporting given message.

Do note that execution of the current function continues after the call.
*/
#define unit_fail(T, msg) _unit_failf(T, CTX, msg)
#define unit_failf(T, fmt, ...) _unit_failf(T, CTX, fmt, __VA_ARGS__)

/*!
Skips current test, reporting given message.

The execution of the current test is stopped. Any clean-up required ought to
be performed prior to issuing this call.
*/
#define unit_skip(T, msg) _unit_skipf(T, CTX, msg)
#define unit_skipf(T, fmt, ...) _unit_skipf(T, CTX, fmt, __VA_ARGS__)

/*!
Skips current test suite, failing it, reporting given message.

The execution of the current test suite is stopped. Any clean-up required
ought to be performed prior to issuing this call.
*/
#define unit_fatal(T, msg) _unit_fatalf(T, CTX, msg)
#define unit_fatalf(T, fmt, ...) _unit_fatalf(T, CTX, fmt, __VA_ARGS__)

/*!
Asserts that given test is true.

In case of test is evaluated to false, the test is printed as a string and a
failure is registered.
*/
#define unit_assert(T, test) if (!(test)) { _unit_failf(T, CTX, "! " #test); }

/*!
Unit call context.

Represents the function, file and line at which a fail, skip or fatal call is
made. The type is typically instantiated using the CTX macro.
*/
typedef struct {
    const char *func;
    const char *file;
    const int line;
} CTX_t;

/*!
Unit call context macro.

This macro expands into a call context which describes the function, file and
line at which the macro is placed.

The macro is intended to be use together with the _unit_failf(),
_unit_skipf(), and _unit_fatalf() functions, in order for these to be able to
properly print the contexts in which they are called.
*/
#define CTX &(CTX_t){__func__, __FILE__, __LINE__}

void _unit_failf(T_t *T, const CTX_t *X, const char *fmt, ...);
void _unit_skipf(T_t *T, const CTX_t *X, const char *fmt, ...);
void _unit_fatalf(T_t *T, const CTX_t *X, const char *fmt, ...);

#endif
