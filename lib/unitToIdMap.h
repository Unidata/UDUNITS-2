#ifndef UT_UNIT_TO_ID_MAP_H_INCLUDED
#define UT_UNIT_TO_ID_MAP_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Adds to a mapping from a unit to a name.
 *
 * Arguments:
 *	unit		Pointer to the unit.
 *	name		Pointer to the name.  May be freed upon return.
 *	encoding	The encoding of "name".
 * Returns:
 *	UT_BADUNIT	"unit" is NULL.
 *	UT_BADID	"name" is NULL or not in the specified encoding.
 *	UT_OS		Operating-system error.  See "errno".
 *	UT_EXISTS	"unit" already maps to a name.
 *	UT_SUCCESS	Success.
 */
utStatus
utMapUnitToName(
    utUnit* const	unit,
    const char* const	name,
    utEncoding		encoding);


/*
 * Adds to a unit-system a mapping from a unit to a symbol.
 *
 * Arguments:
 *	unit		Pointer to the unit.
 *	symbol		Pointer to the symbol.  May be freed upon return.
 *	encoding	The encoding of "symbol".
 * Returns:
 *	UT_BADUNIT	"unit" is NULL.
 *	UT_BADID	"symbol" is NULL.
 *	UT_OS		Operating-system error.  See "errno".
 *	UT_EXISTS	"unit" already maps to a symbol.
 *	UT_SUCCESS	Success.
 */
utStatus
utMapUnitToSymbol(
    utUnit* const	unit,
    const char* const	symbol,
    utEncoding		encoding);


/*
 * Returns the name in a given encoding to which a unit maps.
 *
 * Arguments:
 *	unit		Pointer to the unit whose name should be returned.
 *	encoding	The desired encoding of the name.
 * Returns:
 *	NULL		Failure.  "utGetStatus()" will be
 *			    UT_BADUNIT	"unit" is NULL.
 *			    UT_SUCCESS	There is no name in the given encoding
 *					associated with "unit".
 *	else		Pointer to the name in the given encoding associated
 *			with "unit".
 */
const char*
utGetName(
    const utUnit* const	unit,
    const utEncoding	encoding);


/*
 * Returns the symbol in a given encoding to which a unit maps.
 *
 * Arguments:
 *	unit		Pointer to the unit whose symbol should be returned.
 *	encoding	The desired encoding of the symbol.
 * Returns:
 *	NULL		Failure.  "utGetStatus()" will be
 *			    UT_BADUNIT	"unit" is NULL.
 *			    UT_SUCCESS	There is no symbol in the given encoding
 *					associated with "unit".
 *	else		Pointer to the symbol in the given encoding associated
 *			with "unit".
 */
const char*
utGetSymbol(
    const utUnit* const	unit,
    const utEncoding	encoding);

#ifdef __cplusplus
}
#endif

#endif
