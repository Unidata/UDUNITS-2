/**
 * This file defines a unit's dimension vector.
 *
 *  @file:  DimVec.c
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

#include "DimVec.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct DimVec {
    int     numDim;
    DimPow* dimPows;
};

#define MIN(a,b) ((a) <= (b) ? (a) : (b))

DimVec* dv_new(const int numDim)
{
    assert(numDim >= 0);

    DimVec* dimVec = malloc(sizeof(DimVec));

    if (dimVec) {
        DimPow* dimPows = malloc(numDim*sizeof(DimPow));

        if (dimPows) {
            for (int i = 0; i < numDim; ++i)
                dp_init(dimPows+i, 0, 1);
            dimVec->dimPows = dimPows;
            dimVec->numDim = numDim;
        }
        else {
            free(dimVec);
            dimVec = NULL;
        }
    }

    return dimVec;
}

void dv_free(DimVec* dimVec)
{
    if (dimVec) {
        free(dimVec->dimPows);
        free(dimVec);
    }
}

void dv_setFromExp(
        DimVec*       dimVec,
        const int     i,
        const int     numer,
        const int     denom)
{
    assert(dimVec);
    assert(dimVec->dimPows);
    assert(i >= 0);
    assert(i < dimVec->numDim);

    dp_init(dimVec->dimPows+i, numer, denom);
}

void dv_set(
        DimVec*       dimVec,
        const int     i,
        const DimPow* dimPow)
{
    dv_setFromExp(dimVec, i, dp_getNumerator(dimPow), dp_getDenominator(dimPow));
}

DimVec* dv_clone(const DimVec* dimVec)
{
    DimVec* clone = dv_new(dimVec->numDim);
    if (clone) {
        clone->numDim = dimVec->numDim;
        memcpy(clone->dimPows, dimVec->dimPows, dimVec->numDim*sizeof(DimPow));
    }
    return clone;
}

bool dv_isDimensionless(const DimVec* dimVec)
{
    for (int i = 0; i < dimVec->numDim; ++i)
        if (dp_contributes(dimVec->dimPows+i))
            return false;
    return true;
}

bool dv_equal(
        const DimVec* lhs,
        const DimVec* rhs)
{
    if (lhs->numDim != rhs->numDim)
        return false;
    for (int i = 0; i < lhs->numDim; ++i)
        if (!dp_areEqual(lhs->dimPows+i, rhs->dimPows+i))
            return false;
    return true;
}

void dv_multiply(
        DimVec*   dimVec,
        const int numer,
        const int denom)
{
    for (int i = 0; i < dimVec->numDim; ++i)
        dp_multiply(dimVec->dimPows+i, numer, denom);
}

void dv_add(
        DimVec*       dimVec,
        const DimVec* term)
{
    assert(dimVec->numDim == term->numDim);
    for (int i = 0; i < dimVec->numDim; ++i)
        dp_add(dimVec->dimPows+i, term->dimPows+i);
}

void dv_subtract(
        DimVec*       dimVec,
        const DimVec* term)
{
    assert(dimVec->numDim == term->numDim);
    for (int i = 0; i < dimVec->numDim; ++i)
        dp_subtract(dimVec->dimPows+i, term->dimPows+i);
}

int dv_toString(
        const DimVec* dimVec,
        char*         buf,
        const size_t  size)
{
    const char* out = buf + size;
    int         nbytes = 0;
    for (int i = 0; i < dimVec->numDim; ++i) {
        if (buf < out)
            *buf++ = '[';
        ++nbytes;

        int n = dp_toString(dimVec->dimPows+i, buf, out-buf);
        if (n < 0)
            return n;
        buf += MIN(n, out-buf);
        nbytes += n;

        if (buf < out)
            *buf++ = ']';
        ++nbytes;
    }
    if (buf < out)
        *buf = 0;
    return nbytes;
}
