/**
 * This file defines the power/exponent of a unit dimension.
 *
 *  @file:  DimPow.c
 * @author: Steven R. Emmerson <emmerson@ucar.edu>
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

#include "DimPow.h"

#include <assert.h>
#include <stdio.h>

#define ABS(x) ((x) >= 0 ? (x) : -(x))

/**
 * Compute the Greatest Common Divisor using the Euclidean algorithm.
 */
static int gcd0(int a, int b)
{
    assert(b <= a);

    while(b > 1) {
        int tmp = b;
        b = a % b;
        a = tmp;
    }

    return (b == 0) ? a : 1;
}


/**
 * Driver for above
 */
static int gcd(int a, int b)
{
    if (a < b) {
        /* swap */
        int tmp = a;
        a = b;
        b = tmp;
    }
    return gcd0(a, b);
}

static int lcm(int a, int b)
{
    return ABS(a*b)/gcd(a,b);
}

static void reduce(DimPow* dimPow)
{
    int divisor = gcd(ABS(dimPow->numer), ABS(dimPow->denom));
    if (divisor != 1) {
        dimPow->numer /= divisor;
        dimPow->denom /= divisor;
    }
}

void dp_init(
        DimPow* dimPow,
        short   numer,
        short   denom)
{
    assert(denom != 0);

    if (denom < 0) {
        numer *= -1;
        denom *= -1;
    }

    dimPow->numer = numer;
    dimPow->denom = denom;
    dimPow->contributes = numer != 0;
    reduce(dimPow);
}

void dp_initDefault(DimPow* dimPow)
{
    dp_init(dimPow, 0, 1);
}

DimPow* dp_new(
    const short numer,
    const short denom)
{
    DimPow* dimPow = malloc(sizeof(DimPow));
    if (dimPow)
        dp_init(dimPow, numer, denom);
    return dimPow;
}

DimPow* dp_newDefault()
{
    return dp_new(0, 1);
}

void dp_free(DimPow* dimPow)
{
    free(dimPow);
}

short dp_getNumerator(const DimPow* dimPow)
{
    return dimPow->numer;
}

short dp_getDenominator(const DimPow* dimPow)
{
    return dimPow->denom;
}

bool dp_contributes(const DimPow* dimPow)
{
    return dimPow->contributes;
}

bool dp_isUnity(const DimPow* dimPow)
{
    return dimPow->numer == dimPow->denom;
}

void dp_copy(
        DimPow*       lhs,
        const DimPow* rhs)
{
    *lhs = *rhs;
}

int dp_compare(
        const DimPow* lhs,
        const DimPow* rhs)
{
    if (lhs->contributes != rhs->contributes)
        return lhs->contributes
                ? 1
                : -1;
    const int left  = lhs->numer*rhs->denom;
    const int right = rhs->numer*lhs->denom;
    return left < right
            ? -1
            : left > right
              ? 1
              : 0;
}

bool dp_areEqual(
        const DimPow* lhs,
        const DimPow* rhs)
{
    return dp_compare(lhs, rhs) == 0;
}

bool dp_areInverse(
        const DimPow* lhs,
        const DimPow* rhs)
{
    return lhs->contributes == rhs->contributes &&
            lhs->numer*rhs->denom == -rhs->numer*lhs->denom;
}

void dp_add(
        DimPow*       dimPow,
        const DimPow* term)
{
    const int multiple = lcm(dimPow->denom, term->denom);
    const int newNumer = dimPow->numer*(multiple/dimPow->denom) +
            term->numer*(multiple/term->denom);
    if (newNumer == 0) {
        dimPow->contributes = false; // No longer contributes but memoizes previous state
    }
    else {
        dimPow->numer = newNumer;
        dimPow->denom = multiple;
        dimPow->contributes = true;
    }
    reduce(dimPow);
}

void dp_subtract(
        DimPow*       dimPow,
        const DimPow* term)
{
    const int multiple = lcm(dimPow->denom, term->denom);
    const int newNumer = dimPow->numer*(multiple/dimPow->denom) -
            term->numer*(multiple/term->denom);
    if (newNumer == 0) {
        dimPow->contributes = false; // No longer contributes but memoizes previous state
    }
    else {
        dimPow->numer = newNumer;
        dimPow->denom = multiple;
        dimPow->contributes = true;
    }
    reduce(dimPow);
}

void dp_multiply(
        DimPow* dimPow,
        short   numer,
        short   denom)
{
    assert(denom != 0);

    if (denom < 0) {
        denom *= -1;
        numer *= -1;
    }

    dimPow->numer *= numer;
    dimPow->denom *= denom;
    dimPow->contributes = dimPow->contributes && numer != 0;
    reduce(dimPow);
}

int dp_toString(
        const DimPow* const dimPow,
        char*               buf,
        const size_t        size)
{
    char* fmt;

    if (dimPow->contributes) {
        assert(dimPow->numer != 0);
        fmt = (dimPow->denom == 1) ? "%d" : "%d/%d";
    }
    else {
        fmt = (dimPow->numer == 0)
                ? "%d"
                : (dimPow->denom == 1) ? "%d/%d" : "(%d/%d)/(%d/%d)";
    }

    return snprintf(buf, size, fmt, dimPow->numer, dimPow->denom, dimPow->numer, dimPow->denom);
}
