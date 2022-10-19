/**
 * This file declares the power/exponent of a unit dimension.
 *
 *  @file:  DimPow.h
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

#ifndef LIB_DIMPOW_H_
#define LIB_DIMPOW_H_

#include <stdbool.h>
#include <stdlib.h>

typedef struct DimPow {
    int  numer;       ///< Numerator of the power. May be negative.
    int  denom;       ///< Denominator of the power. Will always be positive.
    bool contributes; ///< If this contributes to the unit's dimensionality. For example, "g/g"
                      ///< doesn't contribute to dimensionality but one might want that in a string
                      ///< representation.
} DimPow; ///< Exponent of a unit dimension

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initializes a dimension power.
 *
 * @param[out] dp     Dimension power to be initialized
 * @param[in]  numer  Numerator of the power
 * @param[in]  denom  Denominator of the power
 */
void dp_init(
        DimPow* dp,
        int     numer,
        int     denom);

/**
 * Default initializes.
 *
 * @param[out] dp  Dimension power to be default initialized
 */
void dp_initDefault(DimPow* dp);

/**
 * Returns a new dimension power.
 *
 * @param[in]  numer  Numerator of the fractional power
 * @param[in]  denom  Denominator of the fractional power
 * @return            New dimension power with the given numerator and denominator. Caller should
 *                    free when it's no longer needed.
 * @see `dp_free()`
 */
DimPow* dp_new(
    const int numer,
    const int denom);

/**
 * Returns a new, default dimension power.
 *
 * @return A new, default dimension power
 */
DimPow* dp_newDefault();

/**
 * Frees a dimension power.
 *
 * @param[in] dp  Dimension power to be freed
 */
void dp_free(DimPow* dp);

/**
 * Returns the numerator of a dimension power.
 *
 * @param[in] dimPow  The dimension power
 * @return            The numerator of the power
 */
int dp_getNumerator(const DimPow* dimPow);

/**
 * Returns the denominator of a dimension power.
 *
 * @param[in] dimPow  The dimension power
 * @return            The denominator of the power
 */
int dp_getDenominator(const DimPow* dimPow);

/**
 * Indicates if this dimension power contributes to a unit's dimensionality.
 *
 * @param[in] dimPow   The dimension power
 * @retval    `true`   This dimension power does contribute
 * @retval    `false`  This dimension power does not contribute
 */
bool dp_contributes(const DimPow* dimPow);

/**
 * Copies.
 *
 * @param[out] lhs  Dimension power to be copied to
 * @param[in]  rhs  Dimension power to be copied from
 */
void dp_copy(
        DimPow*       lhs,
        const DimPow* rhs);

/**
 * Indicates if two dimension powers are equal.
 *
 * @param[in] lhs      One dimension power
 * @param[in] rhs      Other dimension power
 * @return    `true`   They are equal
 * @return    `false`  They are not equal
 */
bool dp_equal(
        const DimPow* lhs,
        const DimPow* rhs);

/**
 * Multiplies a dimension power. This is called when a unit is raised to a power.
 *
 * @param[in,out] dimPow  Dimension power to be multiplied
 * @param[in]     numer   Numerator of the multiplier
 * @param[in]     denom   Denominator of the multiplier
 */
void dp_multiply(
        DimPow* dimPow,
        int     numer,
        int     denom);

/**
 * Adds to a dimension power. This is called when a unit is multiplied by another unit.
 *
 * @param[in,out] dimPow  Dimension power to be multiplied
 * @param[in]     numer   Numerator of the multiplier
 * @param[in]     denom   Denominator of the multiplier
 */
void dp_add(
        DimPow* dimPow,
        int     numer,
        int     denom);

/**
 * Returns a string representation of a dimension power. Formats include
 *   - 0                      This dimension doesn't contribute to a unit's dimensionality
 *   - *n*                    This dimension has the given power
 *   - *n*\/*d*               This dimension has the given fractional power
 *   - *n*\/*n*               This dimension doesn't contribute to a unit's dimensionality
 *   - (*n*\/*d*)/(*n*\/*d*)  This dimension doesn't contribute to a unit's dimensionality
 * where:
 *   - *n* is the numerator of the power
 *   - *d* is the denominator of the power and doesn't appear if it's 1
 *
 * @param[in]  dimPow  Dimensional power
 * @param[out] buf     Buffer for string representation
 * @param[in]  size    Size of buffer in bytes
 * @return             Number of bytes that would be written if the buffer was sufficiently large,
 *                     excluding the terminating NUL character
 * @return             A negative value on error
 */
int dp_toString(
        const DimPow* dimPow,
        char*         buf,
        const size_t  size);

#ifdef __cplusplus
}
#endif

#endif /* LIB_DIMPOW_H_ */
