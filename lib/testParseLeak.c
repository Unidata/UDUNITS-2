/*
 * testParseLeak.c — regression harness for the timestamp-trailing-identifier
 * leak (see security-scan-report.md, finding F1) and a few related
 * parse-failure paths.
 *
 * This leak cannot be detected by an in-process assertion: the leaked block is
 * only observable at process exit under a leak detector. The harness is
 * therefore meant to be run under AddressSanitizer or Valgrind in CI:
 *
 *   gcc -fsanitize=address -g testParseLeak.c -ludunits2 -lm -o testParseLeak
 *   ASAN_OPTIONS=detect_leaks=1 ./testParseLeak udunits2.xml
 *
 *   valgrind --leak-check=full --errors-for-leak-kinds=definite \
 *            --error-exitcode=1 ./testParseLeak udunits2.xml
 *
 * The program itself always exits 0 on a successful run; the leak detector is
 * responsible for failing the build. Each input is parsed many times so that a
 * single-block leak accumulates into an obvious, attributable signature.
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "udunits2.h"

static int
silent_handler(const char* fmt, va_list args)
{
    (void)fmt;
    (void)args;
    return 0;
}

static ut_system* sys;

static void
parse_and_free(const char* spec)
{
    ut_unit* u;

    ut_set_status(UT_SUCCESS);
    u = ut_parse(sys, spec, UT_UTF8);
    if (u != NULL)
        ut_free(u);
}

int
main(int argc, char** argv)
{
    const char* xmlPath = (argc > 1) ? argv[1] : NULL;
    int         i;

    /*
     * Inputs that previously leaked a strdup'd ID token. Each pairs a
     * successful parse leaving an unreduced trailing identifier with a
     * follow-up parse that fails before the scanner re-enters its {id} rule —
     * the sequence that exposes the leak at exit. The destructor on <id>
     * must free the discarded token in every case.
     */
    static const char* const leak_inputs[] = {
        "seconds since 2024-01-01 12:00 garbage_trailing",
        "seconds since 2024-01-01 12:00 Z trailingZ",
        "seconds since 2024-01-01 12:00:00 trailingword",
        "seconds since 2024-01-01 1234 trailingpacked",
        /* follow-up parse that fails in the date validator */
        "seconds since 2024-02-31",
        /* assorted leftover-identifier and parse-failure paths */
        "m garbageword",
        "kg something extra",
        "(((m)))xyz",
        "m^100",
        NULL
    };

    ut_set_error_message_handler(silent_handler);

    sys = ut_read_xml(xmlPath);
    if (sys == NULL) {
        (void)fprintf(stderr, "testParseLeak: could not read unit database\n");
        return EXIT_FAILURE;
    }

    /* Repeat enough times that any per-iteration leak is unmistakable. */
    for (i = 0; i < 200; ++i) {
        const char* const* p;

        for (p = leak_inputs; *p != NULL; ++p)
            parse_and_free(*p);
    }

    ut_free_system(sys);

    (void)fprintf(stderr, "testParseLeak: completed (check leak detector)\n");
    return EXIT_SUCCESS;
}
