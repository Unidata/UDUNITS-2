#ifndef UT_ID_TO_UNIT_MAP_H_INCLUDED
#define UT_ID_TO_UNIT_MAP_H_INCLUDED

#include "units.h"


#ifdef __cplusplus
extern "C" {
#endif


/*
 * Adds a mapping from a name to a unit.
 *
 * Arguments:
 *	name	Pointer to the name.  May be freed upon return.
 *	unit	Pointer to the unit.
 * Returns:
 *	UT_BADID	"name" is NULL.
 *	UT_BADUNIT	"unit" is NULL.
 *	UT_OS		Operating-system error.  See "errno".
 *	UT_EXISTS	"name" already maps to a different unit.
 *	UT_SUCCESS	Success.
 */
utStatus
utMapNameToUnit(
    char* const		name,
    utUnit* const	unit);


/*
 * Adds a mapping from a symbol to a unit.
 *
 * Arguments:
 *	system	Pointer to the unit-system.
 *	symbol	Pointer to the symbol.  May be freed upon return.
 *	unit	Pointer to the unit.
 * Returns:
 *	UT_BADID	"symbol" is NULL.
 *	UT_BADUNIT	"unit" is NULL.
 *	UT_OS		Operating-system error.  See "errno".
 *	UT_EXISTS	"symbol" already maps to a different unit.
 *	UT_SUCCESS	Success.
 */
utStatus
utMapSymbolToUnit(
    char* const		symbol,
    utUnit* const	unit);


/*
 * Returns the unit in a unit-system that has a given name.  Name comparisons
 * are case-insensitive.
 *
 * Arguments:
 *	system	Pointer to the unit-system.
 *	name	Pointer to the name of the unit to be returned.
 * Returns:
 *	NULL	Failure.  "utGetStatus()" will be
 *		    UT_BADSYSTEM	"system" is NULL.
 *		    UT_BADID		"name" is NULL.
 *		    UT_NOUNIT		"system" contains no mapping from "name"
 *					to a unit.
 *	else	Pointer to the unit in the unit-system with the given name.
 */
utUnit*
utGetUnitByName(
    utSystem* const	system,
    char* const		name);


/*
 * Returns the unit in a unit-system that has a given symbol.  Symbol 
 * comparisons are case-sensitive.
 *
 * Arguments:
 *	system	Pointer to the unit-system.
 *	symbol	Pointer to the symbol associated with the unit to be returned.
 * Returns:
 *	NULL	Failure.  "utGetStatus()" will be
 *		    UT_BADSYSTEM	"system" is NULL.
 *		    UT_BADID		"symbol" is NULL.
 *		    UT_NOUNIT		"system" contains no mapping from
 *					"symbol" to a unit.
 *	else	Pointer to the unit in the unit-system with the given name.
 */
utUnit*
utGetUnitBySymbol(
    utSystem* const	system,
    char* const		symbol);


#ifdef __cplusplus
}
#endif

#endif
