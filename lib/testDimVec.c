/**
 * This file tests dimensional vectors.
 *
 *  @file:  testDimVec.c
 * @author: Steven R. Emmerson
 *
 *    Copyright 2022 University Corporation for Atmospheric Research
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "config.h"

#include "DimVec.h"

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

/*
 * Only called once.
 */
static int
setup(
    void)
{
    return 0;
}


/*
 * Only called once.
 */
static int
teardown(
    void)
{
    return 0;
}


static void
test_defaultDimVecs(void)
{
    DimVec* dimVec;
    char    buf[80];

    dimVec = dv_new(0);
    CU_ASSERT_PTR_NOT_NULL_FATAL(dimVec);
    CU_ASSERT_TRUE_FATAL(dv_toString(dimVec, buf, sizeof(buf)) >= 0);
    CU_ASSERT_STRING_EQUAL(buf, "");
    CU_ASSERT_TRUE(dv_isDimensionless(dimVec));
    dv_free(dimVec);

    dimVec = dv_new(1);
    CU_ASSERT_PTR_NOT_NULL_FATAL(dimVec);
    CU_ASSERT_TRUE_FATAL(dv_toString(dimVec, buf, sizeof(buf)) >= 0);
    CU_ASSERT_STRING_EQUAL(buf, "[0]");
    CU_ASSERT_TRUE(dv_isDimensionless(dimVec));
    dv_free(dimVec);

    dimVec = dv_new(2);
    CU_ASSERT_PTR_NOT_NULL_FATAL(dimVec);
    CU_ASSERT_TRUE_FATAL(dv_toString(dimVec, buf, sizeof(buf)) >= 0);
    CU_ASSERT_STRING_EQUAL(buf, "[0][0]");
    CU_ASSERT_TRUE(dv_isDimensionless(dimVec));
    dv_free(dimVec);
}

static void
test_setFromExp(void)
{
    char    buf[80];
    DimVec* dimVec = dv_new(1);

    dv_setFromExp(dimVec, 0, 2, 4);
    CU_ASSERT_FALSE(dv_isDimensionless(dimVec));
    CU_ASSERT_TRUE_FATAL(dv_toString(dimVec, buf, sizeof(buf)) >= 0);
    CU_ASSERT_STRING_EQUAL(buf, "[1/2]");
    dv_free(dimVec);
}

static void
test_equal(void)
{
    DimVec* dimVec1 = dv_new(1);
    DimVec* dimVec2 = dv_new(1);
    DimVec* dimVec3 = dv_new(1);

    dv_setFromExp(dimVec1, 0, 1, 2);
    dv_setFromExp(dimVec2, 0, 2, 4);
    dv_setFromExp(dimVec3, 0, -1, 2);
    CU_ASSERT_TRUE(dv_equal(dimVec1, dimVec2));
    CU_ASSERT_FALSE(dv_equal(dimVec1, dimVec3));
    CU_ASSERT_FALSE(dv_equal(dimVec2, dimVec3));

    dv_free(dimVec1);
    dv_free(dimVec2);
    dv_free(dimVec3);
}

static void
test_multiply(void)
{
    DimVec* dimVec1 = dv_new(1);
    DimVec* dimVec2 = dv_new(1);

    dv_setFromExp(dimVec1, 0, 1, 2);
    dv_setFromExp(dimVec2, 0, -3, 4);
    dv_multiply(dimVec1, 3, -2);
    CU_ASSERT_TRUE(dv_equal(dimVec1, dimVec2));
    dv_free(dimVec1);
    dv_free(dimVec2);
}

static void
test_add(void)
{
    DimVec* dimVec1 = dv_new(1);
    DimVec* dimVec2 = dv_new(1);
    DimVec* dimVec3 = dv_new(1);

    dv_setFromExp(dimVec1, 0, 1, 2);
    dv_setFromExp(dimVec2, 0, 3, 4);
    dv_setFromExp(dimVec3, 0, 5, 4);

    dv_add(dimVec1, dimVec2);
    CU_ASSERT_FALSE(dv_equal(dimVec1, dimVec2));
    CU_ASSERT_TRUE(dv_equal(dimVec1, dimVec3));

    dv_free(dimVec1);
    dv_free(dimVec2);
    dv_free(dimVec3);
}

static void
test_subtract(void)
{
    char    buf[80];
    DimVec* dimVec1 = dv_new(1);
    DimVec* dimVec2 = dv_new(1);
    DimVec* dimVec3 = dv_new(1);

    dv_setFromExp(dimVec1, 0, 3, 2);
    dv_setFromExp(dimVec2, 0, 4, 4);
    dv_setFromExp(dimVec3, 0, 1, 2);

    dv_subtract(dimVec1, dimVec2);
    CU_ASSERT_FALSE(dv_equal(dimVec1, dimVec2));
    CU_ASSERT_TRUE(dv_equal(dimVec1, dimVec3));

    dv_subtract(dimVec1, dimVec3);
    CU_ASSERT_FALSE(dv_equal(dimVec1, dimVec3));
    CU_ASSERT_TRUE(dv_isDimensionless(dimVec1));
    CU_ASSERT_TRUE_FATAL(dv_toString(dimVec1, buf, sizeof(buf)) >= 0);
    CU_ASSERT_STRING_EQUAL(buf, "[(1/2)/(1/2)]");

    dv_free(dimVec1);
    dv_free(dimVec2);
    dv_free(dimVec3);
}

static void
test_radVsRad2(void)
{
    DimVec* rad = dv_new(1);
    dv_setFromExp(rad, 0, 1, 1);
    dv_subtract(rad, rad);
    CU_ASSERT_TRUE(dv_isDimensionless(rad));

    DimVec* rad2 = dv_clone(rad);
    CU_ASSERT_TRUE(dv_isDimensionless(rad2));
    CU_ASSERT_TRUE(dv_equal(rad, rad2)); // Tests `dv_clone()` as well
    dv_multiply(rad2, 2, 1);
    CU_ASSERT_TRUE(dv_isDimensionless(rad2));
    CU_ASSERT_FALSE(dv_equal(rad, rad2)); // Prevents converting values between radian and steradian

    dv_free(rad);
    dv_free(rad2);
}

int
main(
    const int           argc,
    const char* const*	argv)
{
    int	exitCode = EXIT_FAILURE;

    if (CU_initialize_registry() == CUE_SUCCESS) {
	CU_Suite*	testSuite = CU_add_suite(__FILE__, setup, teardown);

	if (testSuite != NULL) {
	    CU_ADD_TEST(testSuite, test_defaultDimVecs);
	    CU_ADD_TEST(testSuite, test_setFromExp);
	    CU_ADD_TEST(testSuite, test_equal);
	    CU_ADD_TEST(testSuite, test_multiply);
	    CU_ADD_TEST(testSuite, test_add);
	    CU_ADD_TEST(testSuite, test_subtract);
	    CU_ADD_TEST(testSuite, test_radVsRad2);

	    if (CU_basic_run_tests() == CUE_SUCCESS) {
		if (CU_get_number_of_tests_failed() == 0)
		    exitCode = EXIT_SUCCESS;
	    }
	}

	CU_cleanup_registry();
    }

    return exitCode;
}
