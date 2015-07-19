#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "unit.h"

#ifndef UNIT_NO_COLOR
# define _CLR_BLU "\x1B[34m"
# define _CLR_GRN "\x1B[32m"
# define _CLR_RED "\x1B[31m"
# define _CLR_YEL "\x1B[33m"
# define _CLR_MAG "\x1B[35m"
# define _CLR_STP "\x1B[0m"
#else
# define _CLR_BLU
# define _CLR_GRN
# define _CLR_RED
# define _CLR_YEL
# define _CLR_MAG
# define _CLR_STP
#endif

#define _USE_CLR(_CLR, string) _CLR string _CLR_STP
#define _FMT_ISSUE(_CLR, str_issue)                                            \
    "\t" _USE_CLR(_CLR, str_issue " %s") " @ %s:%d\n\t\t"

void unit_init(unit_t *u)
{
    u->suites.failed = 0;
    u->suites.total = 0;
}

void unit_exit(unit_t *u)
{
    if (u->suites.failed == 0) {
        exit(EXIT_SUCCESS);
    }
    exit(EXIT_FAILURE);
}

void _suite_report_success(T_t *T);
void _suite_report_skipped(T_t *T);
void _suite_report_failure(T_t *T);

void unit_run_suite(unit_t *u, const char *name, unit_suite_t suite)
{
    u->suites.total++;
    printf(_USE_CLR(_CLR_BLU, ">>") " %s\n", name);

    T_t T = {{0}};
    if (setjmp(T.jmp_fatal) == 0) {
        suite(&T);
    }

    if (T.tests.failed == 0 && T.tests.skipped == 0) {
        _suite_report_success(&T);
    } else if (T.tests.failed == 0 ) {
        _suite_report_skipped(&T);
    } else {
        u->suites.failed++;
        _suite_report_failure(&T);
    }

    putchar('\n');
}

void _suite_report_success(T_t *T)
{
    printf(
        _USE_CLR(_CLR_GRN, "<<")
        " Passed all " _USE_CLR(_CLR_GRN, "%zu") " tests.\n",
        T->tests.total
    );
}

void _suite_report_skipped(T_t *T)
{
    printf(
        _USE_CLR(_CLR_YEL, "<<")
        " Passed " _USE_CLR(_CLR_GRN, "%zu")
        " and skipped " _USE_CLR(_CLR_YEL, "%zu")
        " out of " _USE_CLR(_CLR_BLU, "%zu") " tests.\n",
        (T->tests.total - T->tests.skipped),
        T->tests.skipped,
        T->tests.total
    );
}

void _suite_report_failure(T_t *T)
{
    printf(
        _USE_CLR(_CLR_RED, "<<")
        " Failed " _USE_CLR(_CLR_RED, "%zu")
        " and skipped " _USE_CLR(_CLR_YEL, "%zu")
        " out of " _USE_CLR(_CLR_BLU, "%zu") " tests.\n",
        T->tests.failed,
        T->tests.skipped,
        T->tests.total
    );
}

void unit_run_test(T_t *T, unit_test_t test, unit_provider_t provider)
{
    T->tests.total++;
    if (setjmp(T->jmp_skip) == 0) {
        if (provider != NULL) {
            provider(T, test);

        } else {
            test(T, NULL);
        }
        if (T->test.failures != 0) {
            T->tests.failed++;
            putchar('\n');
        }
    }
    T->test.failures = 0;
}

void _unit_failf(T_t *T, const CTX_t *X, const char *fmt, ...)
{
    printf(_FMT_ISSUE(_CLR_RED, "-- FAIL:"), X->func, X->file, X->line);

    T->test.failures++;

    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);

    putchar('\n');
}

void _unit_skipf(T_t *T, const CTX_t *X, const char *fmt, ...)
{
    printf(_FMT_ISSUE(_CLR_YEL, "<- SKIP:"), X->func, X->file, X->line);

    T->tests.skipped++;

    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);

    puts("\n");

    longjmp(T->jmp_skip, 1);
}

void _unit_fatalf(T_t *T, const CTX_t *X, const char *fmt, ...)
{
    printf(_FMT_ISSUE(_CLR_MAG, "<= FATAL:"), X->func, X->file, X->line);

    T->tests.failed++;

    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);

    puts("\n");

    longjmp(T->jmp_fatal, 1);
}
