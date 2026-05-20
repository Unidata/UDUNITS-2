/*
 * Copyright 2026 University Corporation for Atmospheric Research
 *
 * This file is part of the UDUNITS-2 package.  See the file COPYRIGHT
 * in the top-level source-directory of the package for copying and
 * redistribution conditions.
 *
 * Tests for the datetime parsing overhaul (issue #124, GRAMMAR.md).
 *
 * Coverage targets:
 *   - Broken DATE (YYYY-MM-DD) and right-truncations
 *   - Packed DATE (YYYYMMDD) and right-truncations
 *   - Broken CLOCK (HH:MM:SS) and right-truncations + fractional seconds
 *   - Packed CLOCK (HHMMSS) and right-truncations
 *   - Broken / packed TZ (+HH:MM, +HHMM)
 *   - Z / GMT / UTC tokens
 *   - ISO 8601 'T' separator semantics
 *   - Range validation (month, day, hour, minute, second, TZ hour)
 *   - Leap-second rule (60 only at 23:59:60)
 *   - Day overflow rollover (Feb 30 -> Mar 1/2)
 *   - Year 0 normalization
 *   - Negative-zero TZ rejection
 *   - SHIFT-with-REAL / SHIFT-with-INT (non-timestamp) still works
 *
 * Assertion strategy:
 *   Accept tests are value-checked: each parsed "seconds since <input>"
 *   is converted against a hand-built reference unit derived from
 *   ut_encode_time(Y, M, D, h, m, s). A successful parse with the wrong
 *   numeric origin therefore still fails the test.
 *
 *   Reject tests check that ut_parse() returns NULL.
 */

#include "config.h"

#include "udunits2.h"
#include "converter.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

static const char*      xmlPath;
static ut_system*       unitSystem;
static ut_unit*         second_unit;

/* Tolerance (in seconds) when comparing parsed timestamps. ut_encode_time
   uses double-precision Julian-day arithmetic, so sub-millisecond drift is
   expected at large dates. 1e-3 is comfortable. */
#define TS_EPS  1e-3

/* ---------------------------------------------------------------------- */
/*                              setup / teardown                          */
/* ---------------------------------------------------------------------- */

static int setup(void)
{
    unitSystem = ut_read_xml(xmlPath);
    if (unitSystem == NULL) {
        fprintf(stderr, "setup: ut_read_xml(%s) failed, status=%d\n",
                xmlPath ? xmlPath : "(default)", (int)ut_get_status());
        return -1;
    }
    second_unit = ut_get_unit_by_name(unitSystem, "second");
    if (second_unit == NULL) {
        fprintf(stderr, "setup: lookup of \"second\" failed\n");
        return -1;
    }
    return 0;
}

static int teardown(void)
{
    ut_free(second_unit);
    ut_free_system(unitSystem);
    return 0;
}

/* ---------------------------------------------------------------------- */
/*                              test helpers                              */
/* ---------------------------------------------------------------------- */

/*
 * Build "seconds since <suffix>" and parse it.
 * Returns the parsed unit (caller frees) or NULL on parse failure.
 */
static ut_unit* parse_seconds_since(const char* suffix)
{
    char buf[256];
    snprintf(buf, sizeof(buf), "seconds since %s", suffix);
    ut_set_status(UT_SUCCESS);
    return ut_parse(unitSystem, buf, UT_UTF8);
}

/*
 * Assert that "seconds since <suffix>" parses, and that its origin
 * equals ut_encode_time(Y, M, D, h, m, s) within TS_EPS seconds.
 *
 * Implementation: build a reference unit "seconds @ <encoded>", get the
 * converter from parsed to reference, apply to 0.0. The result is the
 * difference (parsed_origin - reference_origin) in seconds; we want ~0.
 */
static void assert_timestamp_origin(
        const char* suffix,
        int Y, int M, int D, int h, int min, double s)
{
    ut_unit* parsed = parse_seconds_since(suffix);
    CU_ASSERT_PTR_NOT_NULL_FATAL(parsed);

    double encoded = ut_encode_time(Y, M, D, h, min, s);
    ut_unit* ref = ut_offset_by_time(second_unit, encoded);
    CU_ASSERT_PTR_NOT_NULL_FATAL(ref);

    cv_converter* cv = ut_get_converter(parsed, ref);
    CU_ASSERT_PTR_NOT_NULL_FATAL(cv);

    double delta = cv_convert_double(cv, 0.0);
    if (fabs(delta) > TS_EPS) {
        fprintf(stderr,
            "assert_timestamp_origin: suffix=%s expected=(%d-%d-%d %d:%d:%g) "
            "delta=%g s\n", suffix, Y, M, D, h, min, s, delta);
    }
    CU_ASSERT_DOUBLE_EQUAL(delta, 0.0, TS_EPS);

    cv_free(cv);
    ut_free(ref);
    ut_free(parsed);
}

/*
 * Assert that "seconds since <suffix>" fails to parse.
 */
static void assert_timestamp_reject(const char* suffix)
{
    ut_unit* parsed = parse_seconds_since(suffix);
    if (parsed != NULL) {
        fprintf(stderr, "assert_timestamp_reject: suffix=%s unexpectedly parsed\n",
                suffix);
        ut_free(parsed);
    }
    CU_ASSERT_PTR_NULL(parsed);
}

/*
 * Assert that two suffix strings produce equivalent timestamps.
 * Useful for TZ application: "12:00+05:30" must equal "06:30 UTC".
 */
static void assert_timestamps_equivalent(const char* a, const char* b)
{
    ut_unit* ua = parse_seconds_since(a);
    ut_unit* ub = parse_seconds_since(b);
    CU_ASSERT_PTR_NOT_NULL_FATAL(ua);
    CU_ASSERT_PTR_NOT_NULL_FATAL(ub);

    cv_converter* cv = ut_get_converter(ua, ub);
    CU_ASSERT_PTR_NOT_NULL_FATAL(cv);
    double delta = cv_convert_double(cv, 0.0);
    if (fabs(delta) > TS_EPS) {
        fprintf(stderr,
            "assert_timestamps_equivalent: a=%s b=%s delta=%g s\n",
            a, b, delta);
    }
    CU_ASSERT_DOUBLE_EQUAL(delta, 0.0, TS_EPS);

    cv_free(cv);
    ut_free(ua);
    ut_free(ub);
}

/* ---------------------------------------------------------------------- */
/*                          1. broken DATE                                */
/* ---------------------------------------------------------------------- */

static void test_broken_date_full(void)
{
    assert_timestamp_origin("2024-01-15",        2024,  1, 15, 0, 0, 0.0);
    assert_timestamp_origin("2024-12-31",        2024, 12, 31, 0, 0, 0.0);
    assert_timestamp_origin("1970-01-01",        1970,  1,  1, 0, 0, 0.0);
    assert_timestamp_origin("2000-02-29",        2000,  2, 29, 0, 0, 0.0);
}

static void test_broken_date_single_digit(void)
{
    /* GRAMMAR.md: <month> and <day> are 1-2 digits each. */
    assert_timestamp_origin("2024-1-5",          2024,  1,  5, 0, 0, 0.0);
    assert_timestamp_origin("2024-1-15",         2024,  1, 15, 0, 0, 0.0);
    assert_timestamp_origin("2024-11-5",         2024, 11,  5, 0, 0, 0.0);
}

static void test_broken_date_truncated(void)
{
    /* YYYY-MM with no day defaults to day=1 */
    assert_timestamp_origin("2024-01",           2024,  1,  1, 0, 0, 0.0);
    assert_timestamp_origin("2024-1",            2024,  1,  1, 0, 0, 0.0);
    assert_timestamp_origin("1999-12",           1999, 12,  1, 0, 0, 0.0);
}

static void test_broken_date_year_only(void)
{
    /* YYYY alone defaults to Jan 1 */
    assert_timestamp_origin("2024",              2024,  1,  1, 0, 0, 0.0);
    assert_timestamp_origin("1",                    1,  1,  1, 0, 0, 0.0);
    assert_timestamp_origin("9999",              9999,  1,  1, 0, 0, 0.0);
}

static void test_broken_date_year_zero(void)
{
    /* GRAMMAR.md: year 0 is silently normalized to year 1. */
    assert_timestamp_origin("0-01-01",              1,  1,  1, 0, 0, 0.0);
    assert_timestamp_origin("0",                    1,  1,  1, 0, 0, 0.0);
}

static void test_broken_date_negative_year(void)
{
    assert_timestamp_origin("-1-01-01",            -1,  1,  1, 0, 0, 0.0);
    assert_timestamp_origin("-100-06-15",        -100,  6, 15, 0, 0, 0.0);
}

static void test_broken_date_day_rollover(void)
{
    /* GRAMMAR.md: out-of-calendar dates roll silently to the next month. */
    /* 2024 is a leap year, 1999 is not. */
    assert_timestamp_origin("2024-02-29",        2024,  2, 29, 0, 0, 0.0); /* valid */
    assert_timestamp_origin("1999-02-29",        1999,  3,  1, 0, 0, 0.0); /* rolls */
    assert_timestamp_origin("2024-02-30",        2024,  3,  1, 0, 0, 0.0); /* rolls */
    assert_timestamp_origin("2024-04-31",        2024,  5,  1, 0, 0, 0.0); /* rolls */
}

static void test_broken_date_reject_bad_month(void)
{
    assert_timestamp_reject("2024-13-01");
    assert_timestamp_reject("2024-00-01");
    assert_timestamp_reject("2024-99-01");
}

static void test_broken_date_reject_bad_day(void)
{
    assert_timestamp_reject("2024-01-00");
    assert_timestamp_reject("2024-01-32");
    assert_timestamp_reject("2024-01-99");
}

static void test_broken_date_reject_year_too_long(void)
{
    /* <year> is 1-4 digits (per grammar). */
    assert_timestamp_reject("99999-01-01");
    assert_timestamp_reject("-99999-01-01");
}

static void test_broken_date_reject_bad_separator(void)
{
    assert_timestamp_reject("2024.01.01");
    assert_timestamp_reject("2024/01/01");
}

/* ---------------------------------------------------------------------- */
/*                          2. packed DATE                                */
/* ---------------------------------------------------------------------- */

static void test_packed_date_full(void)
{
    /* YYYYMMDD */
    assert_timestamp_origin("20240115",          2024,  1, 15, 0, 0, 0.0);
    assert_timestamp_origin("20241231",          2024, 12, 31, 0, 0, 0.0);
    assert_timestamp_origin("19700101",          1970,  1,  1, 0, 0, 0.0);
}

static void test_packed_date_truncated(void)
{
    /* GRAMMAR.md length-based interpretation:
         len 1-4  -> YYYY            (-> YYYY-01-01)
         len 5    -> YYYYM           (-> YYYY-0M-01)
         len 6    -> YYYYMM          (-> YYYY-MM-01)
         len 7    -> YYYYMMD         (-> YYYY-MM-0D)
         len 8    -> YYYYMMDD
    */
    assert_timestamp_origin("2024",              2024,  1,  1, 0, 0, 0.0); /* 4 */
    assert_timestamp_origin("202401",            2024,  1,  1, 0, 0, 0.0); /* 6 */
    assert_timestamp_origin("2024011",           2024,  1,  1, 0, 0, 0.0); /* 7 */
    assert_timestamp_origin("2024012",           2024,  1,  2, 0, 0, 0.0); /* 7 */
    assert_timestamp_origin("20240107",          2024,  1,  7, 0, 0, 0.0); /* 8 */
}

static void test_packed_date_reject_bad_length(void)
{
    /* len 5: YYYYM -> month digit; if month=0 -> reject */
    assert_timestamp_reject("20240");   /* YYYY-00-01 -> bad month */
}

/* ---------------------------------------------------------------------- */
/*                       3. broken CLOCK                                  */
/* ---------------------------------------------------------------------- */

static void test_broken_clock_full(void)
{
    assert_timestamp_origin("2024-01-15 12:30:45",
                            2024,  1, 15, 12, 30, 45.0);
    assert_timestamp_origin("2024-01-15 00:00:00",
                            2024,  1, 15,  0,  0,  0.0);
    assert_timestamp_origin("2024-01-15 23:59:59",
                            2024,  1, 15, 23, 59, 59.0);
}

static void test_broken_clock_no_seconds(void)
{
    assert_timestamp_origin("2024-01-15 12:30",
                            2024,  1, 15, 12, 30,  0.0);
    assert_timestamp_origin("2024-01-15 00:00",
                            2024,  1, 15,  0,  0,  0.0);
}

static void test_broken_clock_fractional_seconds(void)
{
    assert_timestamp_origin("2024-01-15 12:30:45.5",
                            2024,  1, 15, 12, 30, 45.5);
    assert_timestamp_origin("2024-01-15 12:30:45.125",
                            2024,  1, 15, 12, 30, 45.125);
    assert_timestamp_origin("2024-01-15 12:30:45.000001",
                            2024,  1, 15, 12, 30, 45.000001);
}

static void test_broken_clock_single_digit_components(void)
{
    /* <tod-hour>, <minute>, <second> are each 1-2 digits per grammar. */
    assert_timestamp_origin("2024-01-15 9:5",
                            2024,  1, 15,  9,  5,  0.0);
    assert_timestamp_origin("2024-01-15 9:5:3",
                            2024,  1, 15,  9,  5,  3.0);
}

static void test_broken_clock_reject_bad_hour(void)
{
    assert_timestamp_reject("2024-01-15 24:00:00");
    assert_timestamp_reject("2024-01-15 25:00");
    assert_timestamp_reject("2024-01-15 99:00");
}

static void test_broken_clock_reject_bad_minute(void)
{
    assert_timestamp_reject("2024-01-15 12:60:00");
    assert_timestamp_reject("2024-01-15 12:99");
}

static void test_broken_clock_leap_second(void)
{
    /* Leap second 60 only allowed at 23:59:60. */
    assert_timestamp_origin("2024-01-15 23:59:60",
                            2024,  1, 16,  0,  0,  0.0);   /* rolls to next day */
    assert_timestamp_origin("2024-01-15 23:59:60.5",
                            2024,  1, 16,  0,  0,  0.5);
}

static void test_broken_clock_reject_leap_second_elsewhere(void)
{
    assert_timestamp_reject("2024-01-15 22:59:60");
    assert_timestamp_reject("2024-01-15 23:58:60");
    assert_timestamp_reject("2024-01-15 12:30:60");
    assert_timestamp_reject("2024-01-15 23:59:61");
}

/* ---------------------------------------------------------------------- */
/*                       4. packed CLOCK                                  */
/* ---------------------------------------------------------------------- */

static void test_packed_clock_full(void)
{
    /* HHMMSS */
    assert_timestamp_origin("2024-01-15 123045",
                            2024,  1, 15, 12, 30, 45.0);
    assert_timestamp_origin("2024-01-15 000000",
                            2024,  1, 15,  0,  0,  0.0);
}

static void test_packed_clock_truncated(void)
{
    /* GRAMMAR.md packed-clock length interpretation:
         len 1  -> H      (-> 0H:00:00)
         len 2  -> HH     (-> HH:00:00)
         len 3  -> HHM    (-> HH:0M:00)
         len 4  -> HHMM   (-> HH:MM:00)
         len 5  -> HHMMS  (-> HH:MM:0S)
         len 6  -> HHMMSS
    */
    assert_timestamp_origin("2024-01-15 1",
                            2024,  1, 15,  1,  0,  0.0); /* len 1 */
    assert_timestamp_origin("2024-01-15 12",
                            2024,  1, 15, 12,  0,  0.0); /* len 2 */
    assert_timestamp_origin("2024-01-15 123",
                            2024,  1, 15, 12,  3,  0.0); /* len 3 */
    assert_timestamp_origin("2024-01-15 1234",
                            2024,  1, 15, 12, 34,  0.0); /* len 4 */
    assert_timestamp_origin("2024-01-15 12345",
                            2024,  1, 15, 12, 34,  5.0); /* len 5 */
}

static void test_packed_clock_fractional_seconds(void)
{
    /* Per GRAMMAR.md: decimals only accepted for len 5+. */
    assert_timestamp_origin("2024-01-15 123045.5",
                            2024,  1, 15, 12, 30, 45.5);
    assert_timestamp_origin("2024-01-15 12345.5",
                            2024,  1, 15, 12, 34,  5.5);
}

static void test_packed_clock_reject_fraction_too_short(void)
{
    /* len 1-4 with a fraction should be rejected (grammar comment). */
    assert_timestamp_reject("2024-01-15 1.5");
    assert_timestamp_reject("2024-01-15 12.5");
    assert_timestamp_reject("2024-01-15 123.5");
    assert_timestamp_reject("2024-01-15 1234.5");
}

/* ---------------------------------------------------------------------- */
/*                  5. ISO 'T' separator semantics                        */
/* ---------------------------------------------------------------------- */

static void test_iso_t_separator(void)
{
    assert_timestamp_origin("2024-01-15T12:30:45",
                            2024,  1, 15, 12, 30, 45.0);
    assert_timestamp_origin("2024-01-15T00:00:00",
                            2024,  1, 15,  0,  0,  0.0);
    /* T equivalent to space */
    assert_timestamps_equivalent("2024-01-15T12:30:45",
                                 "2024-01-15 12:30:45");
}

static void test_iso_t_no_trailing_space(void)
{
    /* GRAMMAR.md: "T" separator prohibits spaces before CLOCK. */
    assert_timestamp_reject("2024-01-15T 12:30");
    assert_timestamp_reject("2024-01-15T  12:30:45");
}

/* ---------------------------------------------------------------------- */
/*                          6. timezone offset                            */
/* ---------------------------------------------------------------------- */

static void test_tz_broken_positive(void)
{
    /* +05:30 (IST) means local clock is 5h30m ahead of UTC, so
       local 12:00+05:30 == 06:30 UTC. */
    assert_timestamps_equivalent("2024-01-15 12:00:00+05:30",
                                 "2024-01-15 06:30:00 UTC");
    assert_timestamps_equivalent("2024-01-15 12:00:00+09:00",
                                 "2024-01-15 03:00:00 UTC");
}

static void test_tz_broken_negative(void)
{
    /* -08:00 (PST): local 12:00-08:00 == 20:00 UTC same day. */
    assert_timestamps_equivalent("2024-01-15 12:00:00-08:00",
                                 "2024-01-15 20:00:00 UTC");
    assert_timestamps_equivalent("2024-01-15 12:00:00-05:00",
                                 "2024-01-15 17:00:00 UTC");
}

static void test_tz_packed(void)
{
    /* +HHMM */
    assert_timestamps_equivalent("2024-01-15 12:00:00+0530",
                                 "2024-01-15 06:30:00 UTC");
    assert_timestamps_equivalent("2024-01-15 12:00:00-0800",
                                 "2024-01-15 20:00:00 UTC");
    /* +HH */
    assert_timestamps_equivalent("2024-01-15 12:00:00+05",
                                 "2024-01-15 07:00:00 UTC");
}

static void test_tz_extremes_accepted(void)
{
    /* ±14:00 is the documented maximum. */
    assert_timestamps_equivalent("2024-01-15 12:00:00+14:00",
                                 "2024-01-14 22:00:00 UTC");
    assert_timestamps_equivalent("2024-01-15 12:00:00-14:00",
                                 "2024-01-16 02:00:00 UTC");
}

static void test_tz_reject_out_of_range(void)
{
    assert_timestamp_reject("2024-01-15 12:00:00+14:30");
    assert_timestamp_reject("2024-01-15 12:00:00+15:00");
    assert_timestamp_reject("2024-01-15 12:00:00-15:00");
    assert_timestamp_reject("2024-01-15 12:00:00+99:00");
}

static void test_tz_reject_negative_zero(void)
{
    /* GRAMMAR.md: -00:00 is explicitly disallowed. */
    assert_timestamp_reject("2024-01-15 12:00:00-00:00");
    assert_timestamp_reject("2024-01-15 12:00:00-0000");
    assert_timestamp_reject("2024-01-15 12:00:00-00");
}

static void test_tz_positive_zero_ok(void)
{
    /* +00:00 is fine (equivalent to UTC). */
    assert_timestamps_equivalent("2024-01-15 12:00:00+00:00",
                                 "2024-01-15 12:00:00 UTC");
    assert_timestamps_equivalent("2024-01-15 12:00:00+0000",
                                 "2024-01-15 12:00:00 UTC");
}

static void test_tz_crosses_date_boundary(void)
{
    /* +14:00 applied to 03:00 rolls to previous UTC day. */
    assert_timestamps_equivalent("2024-01-15 03:00:00+14:00",
                                 "2024-01-14 13:00:00 UTC");
    /* -12:00 applied to 18:00 rolls to next UTC day. */
    assert_timestamps_equivalent("2024-01-15 18:00:00-12:00",
                                 "2024-01-16 06:00:00 UTC");
}

/* ---------------------------------------------------------------------- */
/*                          7. Z / GMT / UTC tokens                       */
/* ---------------------------------------------------------------------- */

static void test_zulu_after_clock(void)
{
    assert_timestamp_origin("2024-01-15 12:30:00Z",
                            2024,  1, 15, 12, 30,  0.0);
    assert_timestamp_origin("2024-01-15 12:30Z",
                            2024,  1, 15, 12, 30,  0.0);
}

static void test_zulu_after_date_alone(void)
{
    /* GRAMMAR.md allows DATE Z_TOK (without CLOCK). */
    assert_timestamp_origin("2024-01-15Z",
                            2024,  1, 15,  0,  0,  0.0);
}

static void test_gmt_utc_after_clock(void)
{
    assert_timestamp_origin("2024-01-15 12:30:00 GMT",
                            2024,  1, 15, 12, 30,  0.0);
    assert_timestamp_origin("2024-01-15 12:30:00 UTC",
                            2024,  1, 15, 12, 30,  0.0);
    /* GMT/UTC/Z are all equivalent to no-offset. */
    assert_timestamps_equivalent("2024-01-15 12:00:00 GMT",
                                 "2024-01-15 12:00:00 UTC");
    assert_timestamps_equivalent("2024-01-15 12:00:00Z",
                                 "2024-01-15 12:00:00 UTC");
}

static void test_gmt_utc_reject_without_clock(void)
{
    /* Per grammar: GMT_TOK / UTC_TOK only valid after DATE CLOCK,
       not after DATE alone. (Z is the asymmetric exception.) */
    assert_timestamp_reject("2024-01-15 GMT");
    assert_timestamp_reject("2024-01-15 UTC");
    assert_timestamp_reject("2024-01-15GMT");
    assert_timestamp_reject("2024-01-15UTC");
}

/* ---------------------------------------------------------------------- */
/*                          8. SHIFT with REAL / INT                      */
/* ---------------------------------------------------------------------- */

/*
 * The shift productions also accept REAL and INT (not just timestamps).
 * These are simple offset operations; make sure the datetime rewrite
 * didn't break them.
 */
static void test_shift_real(void)
{
    ut_set_status(UT_SUCCESS);
    ut_unit* u = ut_parse(unitSystem, "K @ 273.15", UT_UTF8);
    CU_ASSERT_PTR_NOT_NULL_FATAL(u);
    /* K @ 273.15 should be celsius. */
    ut_unit* celsius = ut_get_unit_by_name(unitSystem, "celsius");
    CU_ASSERT_PTR_NOT_NULL_FATAL(celsius);
    CU_ASSERT_EQUAL(ut_compare(u, celsius), 0);
    ut_free(celsius);
    ut_free(u);
}

static void test_shift_int(void)
{
    ut_set_status(UT_SUCCESS);
    ut_unit* u = ut_parse(unitSystem, "K @ 273", UT_UTF8);
    CU_ASSERT_PTR_NOT_NULL(u);
    ut_free(u);
}

static void test_shift_keywords(void)
{
    /* "after", "from", "since", "ref", "@" all valid SHIFT operators. */
    const char* prefixes[] = {
        "K @ 273", "K after 273", "K from 273", "K since 273", "K ref 273",
        "K AFTER 273", "K From 273", "K SiNcE 273", "K REF 273",
    };
    for (size_t i = 0; i < sizeof(prefixes)/sizeof(prefixes[0]); ++i) {
        ut_unit* u = ut_parse(unitSystem, prefixes[i], UT_UTF8);
        if (u == NULL) {
            fprintf(stderr, "test_shift_keywords: failed on '%s'\n", prefixes[i]);
        }
        CU_ASSERT_PTR_NOT_NULL(u);
        ut_free(u);
    }
}

/* ---------------------------------------------------------------------- */
/*                          9. miscellaneous edge cases                   */
/* ---------------------------------------------------------------------- */

static void test_date_clock_combinations_consistent(void)
{
    /* Several spellings of the same instant must agree. */
    assert_timestamps_equivalent("2024-01-15 12:30:45",
                                 "20240115 123045");
    assert_timestamps_equivalent("2024-01-15 12:30:45",
                                 "2024-01-15T12:30:45");
    assert_timestamps_equivalent("2024-01-15 12",
                                 "2024-01-15 12:00:00");
    assert_timestamps_equivalent("2024-01-15",
                                 "2024-01-15 00:00:00");
}

static void test_reject_garbage(void)
{
    assert_timestamp_reject("not-a-date");
    assert_timestamp_reject("2024-01-15 12:30:45 garbage");
    assert_timestamp_reject("2024-01-15 12:30:45+nonsense");
    assert_timestamp_reject("2024-01-15 12:30:45+5:30:00"); /* TZ has no seconds */
}

/* ---------------------------------------------------------------------- */
/*                          main / registration                            */
/* ---------------------------------------------------------------------- */

int main(const int argc, const char* const* argv)
{
    int exitCode = EXIT_FAILURE;

    xmlPath = argv[1] ? argv[1] : getenv("UDUNITS2_XML_PATH");

    if (CU_initialize_registry() != CUE_SUCCESS) {
        fprintf(stderr, "CU_initialize_registry failed\n");
        return EXIT_FAILURE;
    }

    CU_Suite* s = CU_add_suite(__FILE__, setup, teardown);
    if (s == NULL) {
        CU_cleanup_registry();
        return EXIT_FAILURE;
    }

    /* 1. broken DATE */
    CU_ADD_TEST(s, test_broken_date_full);
    CU_ADD_TEST(s, test_broken_date_single_digit);
    CU_ADD_TEST(s, test_broken_date_truncated);
    CU_ADD_TEST(s, test_broken_date_year_only);
    CU_ADD_TEST(s, test_broken_date_year_zero);
    CU_ADD_TEST(s, test_broken_date_negative_year);
    CU_ADD_TEST(s, test_broken_date_day_rollover);
    CU_ADD_TEST(s, test_broken_date_reject_bad_month);
    CU_ADD_TEST(s, test_broken_date_reject_bad_day);
    CU_ADD_TEST(s, test_broken_date_reject_year_too_long);
    CU_ADD_TEST(s, test_broken_date_reject_bad_separator);

    /* 2. packed DATE */
    CU_ADD_TEST(s, test_packed_date_full);
    CU_ADD_TEST(s, test_packed_date_truncated);
    CU_ADD_TEST(s, test_packed_date_reject_bad_length);

    /* 3. broken CLOCK */
    CU_ADD_TEST(s, test_broken_clock_full);
    CU_ADD_TEST(s, test_broken_clock_no_seconds);
    CU_ADD_TEST(s, test_broken_clock_fractional_seconds);
    CU_ADD_TEST(s, test_broken_clock_single_digit_components);
    CU_ADD_TEST(s, test_broken_clock_reject_bad_hour);
    CU_ADD_TEST(s, test_broken_clock_reject_bad_minute);
    CU_ADD_TEST(s, test_broken_clock_leap_second);
    CU_ADD_TEST(s, test_broken_clock_reject_leap_second_elsewhere);

    /* 4. packed CLOCK */
    CU_ADD_TEST(s, test_packed_clock_full);
    CU_ADD_TEST(s, test_packed_clock_truncated);
    CU_ADD_TEST(s, test_packed_clock_fractional_seconds);
    CU_ADD_TEST(s, test_packed_clock_reject_fraction_too_short);

    /* 5. T separator */
    CU_ADD_TEST(s, test_iso_t_separator);
    CU_ADD_TEST(s, test_iso_t_no_trailing_space);

    /* 6. TZ offset */
    CU_ADD_TEST(s, test_tz_broken_positive);
    CU_ADD_TEST(s, test_tz_broken_negative);
    CU_ADD_TEST(s, test_tz_packed);
    CU_ADD_TEST(s, test_tz_extremes_accepted);
    CU_ADD_TEST(s, test_tz_reject_out_of_range);
    CU_ADD_TEST(s, test_tz_reject_negative_zero);
    CU_ADD_TEST(s, test_tz_positive_zero_ok);
    CU_ADD_TEST(s, test_tz_crosses_date_boundary);

    /* 7. Z / GMT / UTC */
    CU_ADD_TEST(s, test_zulu_after_clock);
    CU_ADD_TEST(s, test_zulu_after_date_alone);
    CU_ADD_TEST(s, test_gmt_utc_after_clock);
    CU_ADD_TEST(s, test_gmt_utc_reject_without_clock);

    /* 8. SHIFT with REAL/INT */
    CU_ADD_TEST(s, test_shift_real);
    CU_ADD_TEST(s, test_shift_int);
    CU_ADD_TEST(s, test_shift_keywords);

    /* 9. misc */
    CU_ADD_TEST(s, test_date_clock_combinations_consistent);
    CU_ADD_TEST(s, test_reject_garbage);

    /* Silence the (noisy, expected) error messages from reject tests. */
    ut_set_error_message_handler(ut_ignore);

    if (CU_basic_run_tests() == CUE_SUCCESS) {
        if (CU_get_number_of_tests_failed() == 0) exitCode = EXIT_SUCCESS;
    }

    CU_cleanup_registry();
    return exitCode;
}
