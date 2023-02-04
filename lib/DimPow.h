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
#include <stdint.h>
#include <stdlib.h>

typedef int8_t DimPowType;
#define DIMPOW_MAX_DIGITS 3

typedef struct DimPow {
    DimPowType numer;       ///< Numerator of the power. May be negative.
    DimPowType denom;       ///< Denominator of the power. Will always be positive.
    bool       contributes; ///< If this contributes to the unit's dimensionality. For example, "g/g"
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
        short   numer,
        short   denom);

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
    const short numer,
    const short denom);

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
short dp_getNumerator(const DimPow* dimPow);

/**
 * Returns the denominator of a dimension power.
 *
 * @param[in] dimPow  The dimension power
 * @return            The denominator of the power
 */
short dp_getDenominator(const DimPow* dimPow);

/**
 * Indicates if this dimension power contributes to a unit's dimensionality.
 *
 * @param[in] dimPow   The dimension power
 * @retval    `true`   This dimension power does contribute
 * @retval    `false`  This dimension power does not contribute
 */
bool dp_contributes(const DimPow* dimPow);

/**
 * Indicates if this dimension power is unity (i.e., one).
 *
 * @param[in] dimPow   The dimension power
 * @retval    `true`   This dimension power is unity
 * @retval    `false`  This dimension power is not unity
 */
bool dp_isUnity(const DimPow* dimPow);

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
 * Compares two dimension powers. Returns an integer that is less than, equal to, or greater than
 * zero as the first power is considered less than, equal to, or greater than the second power,
 * respectively.
 *
 * @param[in] lhs  First dimension power
 * @param[in] rhs  Second dimension power
 * @return         A value less than, equal to, or greater than zero.
 */
int dp_compare(
        const DimPow* lhs,
        const DimPow* rhs);

/**
 * Indicates if two dimension powers are equal.
 *
 * @param[in] lhs      One dimension power
 * @param[in] rhs      Other dimension power
 * @return    `true`   They are equal
 * @return    `false`  They are not equal
 */
bool dp_areEqual(
        const DimPow* lhs,
        const DimPow* rhs);

/**
 * Indicates if two dimension powers are the inverse of each other.
 *
 * @param[in] lhs      One dimension power
 * @param[in] rhs      Other dimension power
 * @return    `true`   They are the inverse of each other
 * @return    `false`  They are not the inverse of each other
 */
bool dp_areInverse(
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
        short   numer,
        short   denom);

/**
 * Adds to a dimension power. This is called when a unit is multiplied by another unit.
 *
 * @param[in,out] dimPow  Dimension power to be multiplied
 * @param[in]     term    Dimension power to multiply
 */
void dp_add(
        DimPow*       dimPow,
        const DimPow* term);

/**
 * Subtracts from a dimension power. This is called when a unit is divided by another unit.
 *
 * @param[in,out] dimPow  Dimension power to be subtracted from
 * @param[in]     term    Dimension power to subtract
 */
void dp_subtract(
        DimPow*       dimPow,
        const DimPow* term);

/**
 * Returns a string representation of a dimension power. Formats include
 *   - 0            This dimension doesn't contribute to a unit's dimensionality
 *   - n            This dimension has the given power
 *   - n/d          This dimension has the given fractional power
 *   - n/n          This dimension doesn't contribute to a unit's dimensionality
 *   - (n/d)/(n/d)  This dimension doesn't contribute to a unit's dimensionality
 * where:
 *   - n is the integer numerator of the power
 *   - d is the unsigned integer denominator of the power and doesn't appear if it's 1
 *
 * @param[in]  dimPow  Dimensional power
 * @param[out] buf     Buffer for string representation
 * @param[in]  size    Size of buffer in bytes
 * @return             Number of bytes that would be written if the buffer was sufficiently large,
 *                     excluding the terminating NUL character
 * @return             A negative value on error
 */
int dp_toString(
        const DimPow* const dimPow,
        char*               buf,
        const size_t        size);

#ifdef __cplusplus
}
#endif

#endif /* LIB_DIMPOW_H_ */
