/**
 * @author Steven R. Emmerson
 */

#ifndef UT_PREFIX_H
#define UT_PREFIX_H

#include "udunits2.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Adds a name-prefix to a unit-system.  A name-prefix is something like "mega"
 * or "milli".  Comparisons between name-prefixes are case-insensitive.
 *
 * Arguments:
 *	system		Pointer to the unit-system.
 *	name		Pointer to the name-prefix (e.g., "mega").  May be freed
 *			upon return.
 *	value		The value of the prefix (e.g., 1e6).
 * Returns:
 *	UT_SUCCESS	Success.
 *	UT_BAD_ARG	"system" or "name" is NULL, or "value" is 0.
 *	UT_EXISTS	"name" already maps to a different value.
 *	UT_OS		Operating-system failure.  See "errno".
 */
ut_status
ut_add_name_prefix(
    ut_system* const	system,
    const char* const	name,
    const double	value);

/*
 * Adds a symbol-prefix to a unit-system.  A symbol-prefix is something like
 * "M" or "y".  Comparisons between symbol-prefixes are case-sensitive.
 *
 * Arguments:
 *	system		Pointer to the unit-system.
 *	symbol		Pointer to the symbol-prefix (e.g., "M").  May be freed
 *			upon return.
 *	value		The value of the prefix (e.g., 1e6).
 * Returns:
 *	UT_SUCCESS	Success.
 *	UT_BADSYSTEM	"system" or "symbol" is NULL.
 *	UT_BAD_ARG	"value" is 0.
 *	UT_EXISTS	"symbol" already maps to a different value.
 *	UT_OS		Operating-system failure.  See "errno".
 */
ut_status
ut_add_symbol_prefix(
    ut_system* const	system,
    const char* const	symbol,
    const double	value);

/*
 * Examines a string for a name-prefix and returns the length of the name-prefix
 * and its value if one is discovered.
 *
 * Arguments:
 *	system	Pointer to the unit-system.
 *	string	Pointer to the string to be examined for a name-prefix.
 *	value	NULL or pointer to the memory location to receive the value of
 *		the name-prefix, if one is discovered.
 *	len	NULL or pointer to the memory location to receive the number of
 *		characters in the name-prefix, if one is discovered.
 * Returns:
 *	UT_BAD_ARG	"string" is NULL.
 *	UT_UNKNOWN	A name-prefix was not discovered.
 *	UT_SUCCESS	Success.  "*value" and "*len" will be set if non-NULL.
 */
ut_status
utGetPrefixByName(
    ut_system* const	system,
    const char* const	string,
    double* const	value,
    size_t* const	len);

/*
 * Examines a string for a symbol-prefix and returns the length of the
 * symbol-prefix and its value if one is discovered.
 *
 * Arguments:
 *	system	Pointer to the unit-system.
 *	string	Pointer to the string to be examined for a symbol-prefix.
 *	value	NULL or pointer to the memory location to receive the value of
 *		the symbol-prefix, if one is discovered.
 *	len	NULL or pointer to the memory location to receive the number of
 *		characters in the symbol-prefix, if one is discovered.
 * Returns:
 *	UT_BAD_ARG	"string" is NULL.
 *	UT_UNKNOWN	A symbol-prefix was not discovered.
 *	UT_SUCCESS	Success.  "*value" and "*len" will be set if non-NULL.
 */
ut_status
utGetPrefixBySymbol(
    ut_system* const	system,
    const char* const	string,
    double* const	value,
    size_t* const	len);

#ifdef __cplusplus
}
#endif

#endif
