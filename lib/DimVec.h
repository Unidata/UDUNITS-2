/**
 * This file declares a dimensional vector.
 *
 *  @file:  DimVec.h
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

#ifndef LIB_DIMVEC_H_
#define LIB_DIMVEC_H_

#include "DimPow.h"

typedef struct DimVec DimVec;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Returns a new dimensional vector. The dimension powers will be default initialized.
 *
 * @param[in] numDim  Number of dimensions
 * @return            New dimensional vector. The caller should free when it's no longer needed.
 * @see `dv_free()`
 */
DimVec* dv_new(const int numDim);

/**
 * Frees a dimensional vector.
 *
 * @param[in] dimVec  Dimension vector to be freed
 */
void dv_free(DimVec* dimVec);

/**
 * Sets a dimension power in a dimensional vector.
 *
 * @param[out] dimVec  Dimensional vector to have a dimension power set
 * @param[in]  i       Origin-0 index of the dimension power to be set
 * @param[in]  numer   Numerator of the dimension power
 * @param[in]  denom   Denominator of the dimension power
 */
void dv_setFromExp(
        DimVec*       dimVec,
        const int     i,
        const int     numer,
        const int     denom);

/**
 * Sets the value of a dimension power in a dimensional vector.
 *
 * @param[out] dimVec  Dimensional vector to have a dimension power set
 * @param[in]  i       Origin-0 index of the dimension power to be set
 * @param[in]  dimPow  Value for the dimension power
 */
void dv_set(
        DimVec*       dimVec,
        const int     i,
        const DimPow* dimPow);

/**
 * Clones a dimensional vector.
 *
 * @param[in] dimVec  Dimensional vector to be cloned
 * @return            Independent clone of the dimensional vector
 * @retval    NULL    Out-of-memory
 */
DimVec* dv_clone(const DimVec* dimVec);

/**
 * Indicates if a dimensional vector is dimensionless.
 *
 * @param[in] dimVec  The dimensional vector
 * @retval `true`     The vector is dimensionless
 * @retval `false`    The vector is not dimensionless
 */
bool dv_isDimensionless(const DimVec* dimVec);

/**
 * Indicates if two dimensional vectors are considered equal.
 *
 * @param[in] lhs      One dimensional vector
 * @param[in] rhs      The other vector
 * @retval    `true`   The two vectors are equal
 * @retval    `false`  The two vectors are not equal
 */
bool dv_equal(
        const DimVec* lhs,
        const DimVec* rhs);

/**
 * Multiplies a dimensional vector. This is called when a unit is raised to a power.
 *
 * @param[in,out] dimVec  The dimensional vector to be multiplied
 * @param[in]     numer   Numerator of the factor
 * @param[in]     denom   Denominator of the factor
 */
void dv_multiply(
        DimVec*   dimVec,
        const int numer,
        const int denom);

/**
 * Adds one dimensional vector to another. This is called when a unit is multiplied by another unit.
 *
 * @param[in,out] dimVec  The dimensional vector to be added to
 * @param[in]     term    The adding dimensional vector
 */
void dv_add(
        DimVec*       dimVec,
        const DimVec* term);

/**
 * Subtracts one dimensional vector from another. This is called when a unit is divided by another
 * unit.
 *
 * @param[in,out] dimVec  The dimensional vector to be subtracted from
 * @param[in]     term    The subtracting dimensional vector
 */
void dv_subtract(
        DimVec*       dimVec,
        const DimVec* term);

/**
 * Returns a string representation of a dimensional vector. The format is
 *     [*dp*]...
 * where:
 *     *dp* is the format of a dimension power
 * The dimension powers are bracketed and ordered by increasing index.
 *
 * @param[in]  dimVec  Dimensional vector
 * @param[out] buf     Buffer for string representation
 * @param[in]  size    Size of buffer in bytes
 * @return             Number of bytes that would be written if the buffer was sufficiently large,
 *                     excluding the terminating NUL character
 * @return             A negative value on error
 */
int dv_toString(
        const DimVec* dimVec,
        char*         buf,
        const size_t  size);

#ifdef __cplusplus
}
#endif

#endif /* LIB_DIMVEC_H_ */
