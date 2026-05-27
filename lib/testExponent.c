/*
 * Copyright 2026 University Corporation for Atmospheric Research
 *
 * This file is part of the UDUNITS-2 package.  See the file COPYRIGHT
 * in the top-level source-directory of the package for copying and
 * redistribution conditions.
 *
 * Tests for the exponent cap introduced for issue #146.
 *
 * Behavior under test:
 *   - The "^N" and "**N" rules accept integer exponents whose digit
 *     sequence matches (0|[1-9][0-9]?), i.e. |N| <= 99 with no
 *     leading zeros. Sign is optional.
 *   - The UTF-8 superscript form (m³, m⁻¹⁸, etc.) is held to the
 *     same constraint: at most two digits, no leading zero on a
 *     two-digit value.
 *   - Anything else is rejected with a detailed error message.
 *
 * Assertion strategy:
 *   Accept tests compare the parsed unit against ut_raise(base, N)
 *   to verify the exponent was applied correctly — a parse that
 *   succeeds with the wrong magnitude is still a failure.
 *   Reject tests check that ut_parse() returns NULL.
 */

#include "config.h"

#include "udunits2.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

static const char*      xmlPath;
static ut_system*       unitSystem;
static ut_unit*         meter;

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
    meter = ut_get_unit_by_name(unitSystem, "meter");
    if (meter == NULL) {
        fprintf(stderr, "setup: lookup of \"meter\" failed\n");
        return -1;
    }
    return 0;
}

static int teardown(void)
{
    ut_free(meter);
    ut_free_system(unitSystem);
    return 0;
}

/* ---------------------------------------------------------------------- */
/*                              test helpers                              */
/* ---------------------------------------------------------------------- */

/*
 * Parse `spec` and require it to equal ut_raise(meter, expectedExponent).
 */
static void assert_accept_meter_exp(const char* spec, int expectedExponent)
{
    ut_set_status(UT_SUCCESS);
    ut_unit* parsed = ut_parse(unitSystem, spec, UT_UTF8);
    if (parsed == NULL) {
        fprintf(stderr, "assert_accept_meter_exp(\"%s\"): parse failed\n",
                spec);
    }
    CU_ASSERT_PTR_NOT_NULL_FATAL(parsed);

    ut_unit* expected = ut_raise(meter, expectedExponent);
    CU_ASSERT_PTR_NOT_NULL_FATAL(expected);

    if (ut_compare(parsed, expected) != 0) {
        fprintf(stderr,
                "assert_accept_meter_exp(\"%s\"): parsed unit != m^%d\n",
                spec, expectedExponent);
    }
    CU_ASSERT_EQUAL(ut_compare(parsed, expected), 0);

    ut_free(expected);
    ut_free(parsed);
}

/*
 * Parse `spec` and require it to be rejected. The reject set must
 * leave ut_parse returning NULL; we don't pin the exact error
 * message (that's a separate concern).
 */
static void assert_reject(const char* spec)
{
    ut_set_status(UT_SUCCESS);
    ut_unit* parsed = ut_parse(unitSystem, spec, UT_UTF8);
    if (parsed != NULL) {
        fprintf(stderr, "assert_reject(\"%s\"): unexpectedly parsed\n",
                spec);
        ut_free(parsed);
    }
    CU_ASSERT_PTR_NULL(parsed);
}

/* ---------------------------------------------------------------------- */
/*                       ASCII "^N" form — accept                         */
/* ---------------------------------------------------------------------- */

static void test_caret_zero(void)            { assert_accept_meter_exp("m^0", 0); }
static void test_caret_one(void)             { assert_accept_meter_exp("m^1", 1); }
static void test_caret_single_digit(void)    { assert_accept_meter_exp("m^9", 9); }
static void test_caret_two_digit(void)       { assert_accept_meter_exp("m^99", 99); }
static void test_caret_negative(void)        { assert_accept_meter_exp("m^-1", -1); }
static void test_caret_neg_two_digit(void)   { assert_accept_meter_exp("m^-99", -99); }
static void test_caret_explicit_positive(void) { assert_accept_meter_exp("m^+99", 99); }
static void test_caret_signed_zero_pos(void) { assert_accept_meter_exp("m^+0", 0); }
static void test_caret_signed_zero_neg(void) { assert_accept_meter_exp("m^-0", 0); }

/* ---------------------------------------------------------------------- */
/*                       ASCII "**N" form — accept                        */
/* ---------------------------------------------------------------------- */

static void test_starstar_basic(void)        { assert_accept_meter_exp("m**3", 3); }
static void test_starstar_two_digit(void)    { assert_accept_meter_exp("m**99", 99); }
static void test_starstar_negative(void)     { assert_accept_meter_exp("m**-18", -18); }
static void test_starstar_neg_boundary(void) { assert_accept_meter_exp("m**-99", -99); }

/* ---------------------------------------------------------------------- */
/*                       ASCII "^N" / "**N" — reject                      */
/* ---------------------------------------------------------------------- */

static void test_reject_three_digits(void)        { assert_reject("m^100"); }
static void test_reject_three_digits_neg(void)    { assert_reject("m^-100"); }
static void test_reject_three_digits_starstar(void){ assert_reject("m**100"); }
static void test_reject_huge(void)                { assert_reject("m^99999999999999999999"); }
static void test_reject_leading_zero_two_digit(void) { assert_reject("m^01"); }
static void test_reject_leading_zero_zero(void)   { assert_reject("m^00"); }
static void test_reject_leading_zero_three(void)  { assert_reject("m^099"); }
static void test_reject_negative_leading_zero(void){ assert_reject("m^-01"); }

/* ---------------------------------------------------------------------- */
/*                      UTF-8 superscript form — accept                   */
/* ---------------------------------------------------------------------- */

/* m^0  -- U+2070 */
static void test_utf8_zero(void)
        { assert_accept_meter_exp("m\xe2\x81\xb0", 0); }
/* m^1  -- U+00B9 */
static void test_utf8_one(void)
        { assert_accept_meter_exp("m\xc2\xb9", 1); }
/* m^3  -- U+00B3 */
static void test_utf8_three(void)
        { assert_accept_meter_exp("m\xc2\xb3", 3); }
/* m^9  -- U+2079 */
static void test_utf8_nine(void)
        { assert_accept_meter_exp("m\xe2\x81\xb9", 9); }
/* m^12 -- U+00B9 U+00B2 */
static void test_utf8_two_digit(void)
        { assert_accept_meter_exp("m\xc2\xb9\xc2\xb2", 12); }
/* m^99 -- U+2079 U+2079 */
static void test_utf8_max(void)
        { assert_accept_meter_exp("m\xe2\x81\xb9\xe2\x81\xb9", 99); }
/* m^-18 -- U+207B U+00B9 U+2078 (radar reflectivity exponent) */
static void test_utf8_radar_minus_eighteen(void)
        { assert_accept_meter_exp(
                "m\xe2\x81\xbb\xc2\xb9\xe2\x81\xb8", -18); }
/* m^-99 -- U+207B U+2079 U+2079 */
static void test_utf8_neg_max(void)
        { assert_accept_meter_exp(
                "m\xe2\x81\xbb\xe2\x81\xb9\xe2\x81\xb9", -99); }
/* m^+99 -- U+207A U+2079 U+2079 */
static void test_utf8_pos_max(void)
        { assert_accept_meter_exp(
                "m\xe2\x81\xba\xe2\x81\xb9\xe2\x81\xb9", 99); }

/* ---------------------------------------------------------------------- */
/*                      UTF-8 superscript form — reject                   */
/* ---------------------------------------------------------------------- */

/* m^100 -- 3 digits */
static void test_utf8_reject_three_digits(void)
        { assert_reject(
                "m\xc2\xb9\xe2\x81\xb0\xe2\x81\xb0"); }
/* m^999 */
static void test_utf8_reject_three_digits_max(void)
        { assert_reject(
                "m\xe2\x81\xb9\xe2\x81\xb9\xe2\x81\xb9"); }
/* m^-100 */
static void test_utf8_reject_neg_three_digits(void)
        { assert_reject(
                "m\xe2\x81\xbb\xc2\xb9\xe2\x81\xb0\xe2\x81\xb0"); }
/* m^01 -- leading zero (2 digits) */
static void test_utf8_reject_leading_zero(void)
        { assert_reject(
                "m\xe2\x81\xb0\xc2\xb9"); }
/* m^00 -- leading zero, both zeros */
static void test_utf8_reject_double_zero(void)
        { assert_reject(
                "m\xe2\x81\xb0\xe2\x81\xb0"); }
/* m^09 */
static void test_utf8_reject_leading_zero_nine(void)
        { assert_reject(
                "m\xe2\x81\xb0\xe2\x81\xb9"); }

/* ---------------------------------------------------------------------- */
/*                    Real-world rainfall-radar use case                  */
/* ---------------------------------------------------------------------- */

/*
 * The reflectivity factor Z is expressed as mm^6/m^3 in CF metadata,
 * which udunits canonicalizes to "1e-18 m^3". The "10^-18 m^3" form
 * (used in some hand-written sources) exercises the ^N exponent rule
 * with N = -18 and must remain accepted.
 */
static void test_radar_reflectivity_via_caret_exponent(void)
{
    ut_set_status(UT_SUCCESS);
    ut_unit* u = ut_parse(unitSystem, "10^-18 m^3", UT_UTF8);
    CU_ASSERT_PTR_NOT_NULL_FATAL(u);

    /* Sanity-check it dimensionally equals mm^6/m^3. */
    ut_unit* mm6_per_m3 = ut_parse(unitSystem, "mm^6/m^3", UT_UTF8);
    CU_ASSERT_PTR_NOT_NULL_FATAL(mm6_per_m3);
    CU_ASSERT_EQUAL(ut_compare(u, mm6_per_m3), 0);

    ut_free(mm6_per_m3);
    ut_free(u);
}

static void test_radar_reflectivity_via_utf8_exponent(void)
{
    /* 10^-18 m^3 written with UTF-8 superscripts: 10⁻¹⁸ m³  */
    const char spec[] =
        "10\xe2\x81\xbb\xc2\xb9\xe2\x81\xb8 m\xc2\xb3";
    ut_set_status(UT_SUCCESS);
    ut_unit* u = ut_parse(unitSystem, spec, UT_UTF8);
    CU_ASSERT_PTR_NOT_NULL_FATAL(u);

    ut_unit* mm6_per_m3 = ut_parse(unitSystem, "mm^6/m^3", UT_UTF8);
    CU_ASSERT_PTR_NOT_NULL_FATAL(mm6_per_m3);
    CU_ASSERT_EQUAL(ut_compare(u, mm6_per_m3), 0);

    ut_free(mm6_per_m3);
    ut_free(u);
}

/* ---------------------------------------------------------------------- */
/*                          main / registration                           */
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

    /* ASCII "^N" accept */
    CU_ADD_TEST(s, test_caret_zero);
    CU_ADD_TEST(s, test_caret_one);
    CU_ADD_TEST(s, test_caret_single_digit);
    CU_ADD_TEST(s, test_caret_two_digit);
    CU_ADD_TEST(s, test_caret_negative);
    CU_ADD_TEST(s, test_caret_neg_two_digit);
    CU_ADD_TEST(s, test_caret_explicit_positive);
    CU_ADD_TEST(s, test_caret_signed_zero_pos);
    CU_ADD_TEST(s, test_caret_signed_zero_neg);

    /* ASCII "**N" accept */
    CU_ADD_TEST(s, test_starstar_basic);
    CU_ADD_TEST(s, test_starstar_two_digit);
    CU_ADD_TEST(s, test_starstar_negative);
    CU_ADD_TEST(s, test_starstar_neg_boundary);

    /* ASCII reject */
    CU_ADD_TEST(s, test_reject_three_digits);
    CU_ADD_TEST(s, test_reject_three_digits_neg);
    CU_ADD_TEST(s, test_reject_three_digits_starstar);
    CU_ADD_TEST(s, test_reject_huge);
    CU_ADD_TEST(s, test_reject_leading_zero_two_digit);
    CU_ADD_TEST(s, test_reject_leading_zero_zero);
    CU_ADD_TEST(s, test_reject_leading_zero_three);
    CU_ADD_TEST(s, test_reject_negative_leading_zero);

    /* UTF-8 accept */
    CU_ADD_TEST(s, test_utf8_zero);
    CU_ADD_TEST(s, test_utf8_one);
    CU_ADD_TEST(s, test_utf8_three);
    CU_ADD_TEST(s, test_utf8_nine);
    CU_ADD_TEST(s, test_utf8_two_digit);
    CU_ADD_TEST(s, test_utf8_max);
    CU_ADD_TEST(s, test_utf8_radar_minus_eighteen);
    CU_ADD_TEST(s, test_utf8_neg_max);
    CU_ADD_TEST(s, test_utf8_pos_max);

    /* UTF-8 reject */
    CU_ADD_TEST(s, test_utf8_reject_three_digits);
    CU_ADD_TEST(s, test_utf8_reject_three_digits_max);
    CU_ADD_TEST(s, test_utf8_reject_neg_three_digits);
    CU_ADD_TEST(s, test_utf8_reject_leading_zero);
    CU_ADD_TEST(s, test_utf8_reject_double_zero);
    CU_ADD_TEST(s, test_utf8_reject_leading_zero_nine);

    /* Real-world radar reflectivity */
    CU_ADD_TEST(s, test_radar_reflectivity_via_caret_exponent);
    CU_ADD_TEST(s, test_radar_reflectivity_via_utf8_exponent);

    /* Silence noisy error messages from reject tests. */
    ut_set_error_message_handler(ut_ignore);

    if (CU_basic_run_tests() == CUE_SUCCESS) {
        if (CU_get_number_of_tests_failed() == 0) exitCode = EXIT_SUCCESS;
    }

    CU_cleanup_registry();
    return exitCode;
}
