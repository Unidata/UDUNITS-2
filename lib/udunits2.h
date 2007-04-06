#ifndef UT_UNITS_H_INCLUDED
#define UT_UNITS_H_INCLUDED

#include <stdarg.h>
#include <stddef.h>

#include "converter.h"

typedef struct utSystem		utSystem;
typedef union utUnit		utUnit;

typedef enum {
    UT_SUCCESS = 0,	/* Success */
    UT_NULL_ARG,	/* An argument is NULL */
    UT_BAD_ID,		/* Identifier is NULL or empty */
    UT_BAD_VALUE,	/* Invalid numeric value */
    UT_EXISTS,		/* Unit, prefix, or identifier already exists */
    UT_NO_UNIT,		/* No such unit exists */
    UT_OS,		/* Operating-system error.  See "errno". */
    UT_NOT_SAME_SYSTEM,	/* The units belong to different unit-systems */
    UT_MEANINGLESS,	/* The operation on the unit(s) is meaningless */
    UT_NO_SECOND,	/* The unit-system doesn't have a unit named "second" */
    UT_BAD_BUF,		/* Character buffer argument is NULL */
    UT_VISIT_ERROR,	/* An error occurred while visiting a unit */
    UT_CANT_FORMAT,	/* A unit can't be formatted in the desired manner */
    UT_SYNTAX,		/* string unit representation contains syntax error */
    UT_UNKNOWN,		/* string unit representation contains unknown word */
    UT_OPEN_ARG,	/* Can't open argument-specified unit database */
    UT_OPEN_ENV,	/* Can't open environment-specified unit database */
    UT_OPEN_DEFAULT,	/* Can't open installed, default, unit database */
    UT_PARSE		/* Error parsing unit database */
} utStatus;

typedef enum {
    UT_ASCII = 0,
    UT_ISO_8859_1 = 1,
    UT_LATIN1 = UT_ISO_8859_1,
    UT_UTF8 = 2
} utEncoding;

#define UT_NAMES	4
#define UT_DEFINITION	8


/*
 * Data-structure for a visitor to a unit:
 */
typedef struct {
    /*
     * Visits a basic-unit.  A basic-unit is a base unit like "meter" or a non-
     * dimensional but named unit like "radian".
     *
     * Arguments:
     *	unit		Pointer to the basic-unit.
     *	arg		Client pointer passed to utAcceptVisitor().
     * Returns:
     *	UT_SUCCESS	Success.
     *	else		Failure.
     */
    utStatus	(*visitBasic)(const utUnit* unit, void* arg); 

    /*
     * Visits a product-unit.  A product-unit is a product of zero or more
     * basic-units, each raised to a non-zero power.
     *
     * Arguments:
     *	unit		Pointer to the product-unit.
     *	count		The number of basic-units in the product.  May be zero.
     *	basicUnits	Pointer to an array of basic-units in the product.
     *	powers		Pointer to an array of powers to which the respective
     *			basic-units are raised.
     *	arg		Client pointer passed to utAcceptVisitor().
     * Returns:
     *	UT_SUCCESS	Success.
     *	else		Failure.
     */
    utStatus	(*visitProduct)(const utUnit* unit, int count,
	const utUnit* const* basicUnits, const int* powers, void* arg); 

    /*
     * Visits a Galilean-unit.  A Galilean-unit has an underlying unit and a
     * non-unity scale factor or a non-zero offset.
     *
     * Arguments:
     *	unit		Pointer to the Galilean-unit.
     *	scale		The scale factor (e.g., 1000 for a kilometer when the
     *			underlying unit is a meter).
     *	underlyingUnit	Pointer to the underlying unit.
     *	offset		Pointer to the underlying unit.
     *	arg		Client pointer passed to utAcceptVisitor().
     * Returns:
     *	UT_SUCCESS	Success.
     *	else		Failure.
     */
    utStatus	(*visitGalilean)(const utUnit* unit, double scale,
	const utUnit* underlyingUnit, double offset, void* arg); 

    /*
     * Visits a timestamp-unit.  A timestamp-unit has an underlying unit of time
     * and an encoded time-origin.
     *
     * Arguments:
     *	unit		Pointer to the timestamp-unit.
     *	timeUnit	Pointer to the underlying unit of time.
     *  origin          Encoded origin of the timestamp-unit.
     *	arg		Client pointer passed to utAcceptVisitor().
     * Returns:
     *	UT_SUCCESS	Success.
     *	else		Failure.
     */
    utStatus	(*visitTimestamp)(const utUnit* unit,
	const utUnit* timeUnit, double origin, void* arg); 

    /*
     * Visits a logarithmic-unit.  A logarithmic-unit has a logarithmic base and
     * a unit that specifies the reference level.
     *
     * Arguments:
     *	unit		Pointer to the logarithmic-unit.
     *  base            The logarithmic base (e.g., 2, M_E, 10).
     *	reference	Pointer to the unit that specifies the reference level.
     *	arg		Client pointer passed to utAcceptVisitor().
     * Returns:
     *	UT_SUCCESS	Success.
     *	else		Failure.
     */
    utStatus	(*visitLogarithmic)(const utUnit* unit, double base,
	const utUnit* reference, void* arg); 
} utVisitor;


typedef int (*utErrorMessageHandler)(const char* fmt, va_list args);


#ifdef __cplusplus
extern "C" {
#endif


/******************************************************************************
 * Unit System:
 ******************************************************************************/


/*
 * Returns the unit-system corresponding to an XML file.  This is the usual way
 * that a client will obtain a unit-system.
 *
 * Arguments:
 *	path	The pathname of the XML file or NULL.  If NULL, then the
 *		pathname specified by the environment variable UDUNITS2_XML_PATH
 *		is used if set; otherwise, the compile-time pathname of the
 *		installed, default, unit database is used.
 * Returns:
 *	NULL	Failure.  "utGetStatus()" will be
 *		    UT_OPEN_ARG		"path" is non-NULL but file couldn't be
 *					opened.  See "errno" for reason.
 *		    UT_OPEN_ENV		"path" is NULL and environment variable
 *					UDUNITS2_XML_PATH is set but file
 *					couldn't be opened.  See "errno" for
 *					reason.
 *		    UT_OPEN_DEFAULT	"path" is NULL, environment variable
 *					UDUNITS2_XML_PATH is unset, and the
 *					installed, default, unit database
 *					couldn't be opened.  See "errno" for
 *					reason.
 *		    UT_PARSE		Couldn't parse unit database.
 *		    UT_OS		Operating-system error.  See "errno".
 *	else	Pointer to the unit-system defined by "path".
 */
utSystem*
utReadXml(
    const char*	path);


/*
 * Returns a new unit-system.  On success, the unit-system will only contain
 * the dimensionless unit one.  See "utGetDimensionlessUnitOne()".
 *
 * Returns:
 *	NULL	Failure.  "utGetStatus()" will be:
 *		    UT_OS	Operating-system error.  See "errno".
 *	else	Pointer to a new unit system.
 */
utSystem*
utNewSystem(void);


/*
 * Frees a unit-system.  All unit-to-identifier and identifier-to-unit mappings
 * will be removed.
 *
 * Arguments:
 *	system		Pointer to the unit-system to be freed.  Use of "system"
 *			upon return results in undefined behavior.
 */
void
utFreeSystem(
    utSystem*	system);


/*
 * Returns the unit-system to which a unit belongs.
 *
 * Arguments:
 *	unit	Pointer to the unit in question.
 * Returns:
 *	NULL	Failure.  "utGetStatus()" will be
 *		    UT_NULL_ARG	"unit" is NULL.
 *	else	Pointer to the unit-system to which "unit" belongs.
 */
utSystem*
utGetSystem(
    const utUnit* const	unit);


/*
 * Returns the dimensionless-unit one of a unit-system.
 *
 * Arguments:
 *	system	Pointer to the unit-system for which the dimensionless-unit one
 *		will be returned.
 * Returns:
 *	NULL	Failure.  "utgetStatus()" will be:
 *		    UT_NULL_ARG	"system" is NULL.
 *	else	Pointer to the dimensionless-unit one associated with "system".
 *		While not necessary, the pointer may be passed to utFree()
 *		when the unit is no longer needed by the client.
 */
utUnit*
utGetDimensionlessUnitOne(
    utSystem* const	system);


/*
 * Returns the unit with a given name from a unit-system.  Name comparisons
 * are case-insensitive.
 *
 * Arguments:
 *	system	Pointer to the unit-system.
 *	name	Pointer to the name of the unit to be returned.
 * Returns:
 *	NULL	Failure.  "utGetStatus()" will be
 *		    UT_SUCCESS		"name" doesn't map to a unit of
 *					"system".
 *		    UT_NULL_ARG		"system" or "name" is NULL.
 *	else	Pointer to the unit of the unit-system with the given name.
 *		The pointer should be passed to utFree() when the unit is
 *		no longer needed.
 */
utUnit*
utGetUnitByName(
    utSystem* const	system,
    const char* const	name);


/*
 * Returns the unit with a given symbol from a unit-system.  Symbol 
 * comparisons are case-sensitive.
 *
 * Arguments:
 *	system		Pointer to the unit-system.
 *	symbol		Pointer to the symbol associated with the unit to be
 *			returned.
 * Returns:
 *	NULL	Failure.  "utGetStatus()" will be
 *		    UT_SUCCESS		"symbol" doesn't map to a unit of
 *					"system".
 *		    UT_NULL_ARG		"system" or "symbol" is NULL.
 *	else	Pointer to the unit in the unit-system with the given symbol.
 *		The pointer should be passed to utFree() when the unit is no
 *		longer needed.
 */
utUnit*
utGetUnitBySymbol(
    utSystem* const	system,
    const char* const	symbol);


/*
 * Sets the "second" unit of a unit-system.  This function must be called before
 * the first call to "utOffsetByTime()". utReadXml() calls this function if the
 * resulting unit-system contains a unit named "second".
 *
 * Arguments:
 *	second		Pointer to the "second" unit.
 * Returns:
 *	UT_NULL_ARG	"second" is NULL.
 *	UT_EXISTS	The second unit of the unit-system to which "second"
 *			belongs is set to a different unit.
 *	UT_SUCCESS	Success.
 */
utStatus
utSetSecond(
    utUnit* const	second);


/******************************************************************************
 * Defining Unit Prefixes:
 ******************************************************************************/


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
 *	UT_NULL_ARG	"system" or "name" is NULL.
 *	UT_BAD_VALUE	"value" is 0.
 *	UT_EXISTS	"name" already maps to a different value.
 *	UT_OS		Operating-system failure.  See "errno".
 */
utStatus
utAddNamePrefix(
    utSystem* const	system,
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
 *	UT_BAD_VALUE	"value" is 0.
 *	UT_EXISTS	"symbol" already maps to a different value.
 *	UT_OS		Operating-system failure.  See "errno".
 */
utStatus
utAddSymbolPrefix(
    utSystem* const	system,
    const char* const	symbol,
    const double	value);


/******************************************************************************
 * Defining and Deleting Units:
 ******************************************************************************/


/*
 * Adds a base-unit to a unit-system.  Clients that use utReadXml() should not
 * normally need to call this function.
 *
 * Arguments:
 *	system	Pointer to the unit-system to which to add the new base-unit.
 * Returns:
 *	NULL	Failure.  "utGetStatus()" will be
 *		    UT_NULL_ARG		"system" or "name" is NULL.
 *		    UT_OS		Operating-system error.  See "errno".
 *	else	Pointer to the new base-unit.  The pointer should be passed to
 *		utFree() when the unit is no longer needed by the client (the
 *		unit will remain in the unit-system).
 */
utUnit*
utNewBaseUnit(
    utSystem* const	system);


/*
 * Adds a dimensionless-unit to a unit-system.  In the SI system of units, the
 * derived-unit radian is a dimensionless-unit.  Clients that use utReadXml()
 * should not normally need to call this function.
 *
 * Arguments:
 *	system	Pointer to the unit-system to which to add the new
 *		dimensionless-unit.
 * Returns:
 *	NULL	Failure.  "utGetStatus()" will be
 *		    UT_NULL_ARG		"system" is NULL.
 *		    UT_OS		Operating-system error.  See "errno".
 *	else	Pointer to the new dimensionless-unit.  The pointer should be
 *		passed to utFree() when the unit is no longer needed by the
 *		client (the unit will remain in the unit-system).
 */
utUnit*
utNewDimensionlessUnit(
    utSystem* const	system);


/*
 * Returns a clone of a unit.
 *
 * Arguments:
 *	unit	Pointer to the unit to be cloned.
 * Returns:
 *	NULL	Failure.  utGetStatus() will be
 *		    UT_OS	Operating-system failure.  See "errno".
 *		    UT_NULL_ARG	"unit" is NULL.
 *	else	Pointer to the clone of "unit".  The pointer should be
 *		passed to utFree() when the unit is no longer needed by the
 *		client.
 */
utUnit*
utClone(
    const utUnit*	unit);


/*
 * Frees resources associated with a unit.  This function should be invoked on
 * all units that are no longer needed by the client.  Use of the unit upon
 * return from this function will result in undefined behavior.
 *
 * Arguments:
 *	unit	Pointer to the unit to have its resources freed or NULL.
 */
void
utFree(
    utUnit* const	unit);


/******************************************************************************
 * Mapping between Units and Names:
 ******************************************************************************/


/*
 * Returns the name in a given encoding to which a unit maps.
 *
 * Arguments:
 *	unit		Pointer to the unit whose name should be returned.
 *	encoding	The desired encoding of the name.
 * Returns:
 *	NULL		Failure.  "utGetStatus()" will be
 *			    UT_NULL_ARG		"unit" is NULL.
 *			    UT_SUCCESS		"unit" doesn't map to a name in
 *						in the given encoding.
 *	else		Pointer to the name in the given encoding to which
 *			"unit" maps.
 */
const char*
utGetName(
    const utUnit* const	unit,
    const utEncoding	encoding);


/*
 * Adds a mapping from a name to a unit.
 *
 * Arguments:
 *	name		Pointer to the name to be mapped to "unit".  May be
 *			freed upon return.
 *	unit		Pointer to the unit to be mapped-to by "name".  May be
 *			freed upon return.
 * Returns:
 *	UT_NULL_ARG	"name" or "unit" is NULL.
 *	UT_OS		Operating-system error.  See "errno".
 *	UT_EXISTS	"name" already maps to a different unit.
 *	UT_SUCCESS	Success.
 */
utStatus
utMapNameToUnit(
    const char* const	name,
    utUnit* const	unit);


/*
 * Removes a mapping from a name to a unit.  After this function,
 * utGetUnitByName(system,name) will no longer return a unit.
 *
 * Arguments:
 *	system		The unit-system to which the unit belongs.
 *	name		The name of the unit.
 * Returns:
 *	UT_SUCCESS	Success.
 *	UT_NULL_ARG	"system" or "name" is NULL.
 */
utStatus
utUnmapNameToUnit(
    utSystem*		system,
    const char* const	name);


/*
 * Adds a mapping from a unit to a name.
 *
 * Arguments:
 *	unit		Pointer to the unit to be mapped to "name".  May be
 *			freed upon return.
 *	name		Pointer to the name to be mapped-to by "unit".  May be
 *			freed upon return.
 *	encoding	The encoding of "name".
 * Returns:
 *	UT_SUCCESS	Success.
 *	UT_NULL_ARG	"unit" or "name" is NULL.
 *	UT_BAD_ID	"name" is not in the specified encoding.
 *	UT_OS		Operating-system error.  See "errno".
 *	UT_EXISTS	"unit" already maps to a name.
 */
utStatus
utMapUnitToName(
    utUnit* const	unit,
    const char* const	name,
    utEncoding		encoding);


/*
 * Removes a mapping from a unit to a name.
 *
 * Arguments:
 *	unit		Pointer to the unit.  May be freed upon return.
 *	encoding	The encoding to be removed.  No other encodings will be
 *			removed.
 * Returns:
 *	UT_NULL_ARG	"unit" is NULL.
 *	UT_SUCCESS	Success.
 */
utStatus
utUnmapUnitToName(
    utUnit* const	unit,
    utEncoding		encoding);


/******************************************************************************
 * Mapping between Units and Symbols:
 ******************************************************************************/


/*
 * Returns the symbol in a given encoding to which a unit maps.
 *
 * Arguments:
 *	unit		Pointer to the unit whose symbol should be returned.
 *	encoding	The desired encoding of the symbol.
 * Returns:
 *	NULL		Failure.  "utGetStatus()" will be
 *			    UT_NULL_ARG		"unit" is NULL.
 *			    UT_SUCCESS		"unit" doesn't map to a symbol
 *						in the given encoding.
 *	else		Pointer to the symbol in the given encoding to which
 *			"unit" maps.
 */
const char*
utGetSymbol(
    const utUnit* const	unit,
    const utEncoding	encoding);


/*
 * Adds a mapping from a symbol to a unit.
 *
 * Arguments:
 *	symbol		Pointer to the symbol to be mapped to "unit".  May be
 *			freed upon return.
 *	unit		Pointer to the unit to be mapped-to by "symbol".  May
 *			be freed upon return.
 * Returns:
 *	UT_NULL_ARG	"symbol" or "unit" is NULL.
 *	UT_OS		Operating-system error.  See "errno".
 *	UT_EXISTS	"symbol" already maps to a different unit.
 *	UT_SUCCESS	Success.
 */
utStatus
utMapSymbolToUnit(
    const char* const	symbol,
    utUnit* const	unit);


/*
 * Removes a mapping from a symbol to a unit.  After this function,
 * utGetUnitBySymbol(system,symbol) will no longer return a unit.
 *
 * Arguments:
 *	system		The unit-system to which the unit belongs.
 *	symbol		The symbol of the unit.
 * Returns:
 *	UT_SUCCESS	Success.
 *	UT_NULL_ARG	"system" or "symbol" is NULL.
 */
utStatus
utUnmapSymbolToUnit(
    utSystem*		system,
    const char* const	symbol);


/*
 * Adds a mapping from a unit to a symbol.
 *
 * Arguments:
 *	unit		Pointer to the unit to be mapped to "symbol".  May be
 *			freed upon return.
 *	symbol		Pointer to the symbol to be mapped-to by "unit".  May
 *			be freed upon return.
 *	encoding	The encoding of "symbol".
 * Returns:
 *	UT_SUCCESS	Success.
 *	UT_NULL_ARG	"unit" or "symbol" is NULL.
 *	UT_OS		Operating-system error.  See "errno".
 *	UT_EXISTS	"unit" already maps to a symbol.
 */
utStatus
utMapUnitToSymbol(
    utUnit*		unit,
    const char* const	symbol,
    utEncoding		encoding);


/*
 * Removes a mapping from a unit to a symbol.
 *
 * Arguments:
 *	unit		Pointer to the unit to be unmapped to a symbol.  May be
 *			freed upon return.
 *	encoding	The encoding to be removed.  The mappings for "unit" in
 *			other encodings will not be removed.
 * Returns:
 *	UT_SUCCESS	Success.
 *	UT_NULL_ARG	"unit" is NULL.
 */
utStatus
utUnmapUnitToSymbol(
    utUnit* const	unit,
    utEncoding		encoding);


/******************************************************************************
 * Getting Information about a Unit:
 ******************************************************************************/


/*
 * Indicates if a given unit is dimensionless or not.  Note that logarithmic
 * units are dimensionless by definition.
 *
 * Arguments:
 *	unit	Pointer to the unit in question.
 * Returns:
 *	0	"unit" is dimensionfull or an error occurred.  "utGetStatus()"
 *		 will be
 *		    UT_NULL_ARG		"unit" is NULL.
 *		    UT_SUCCESS		"unit" is dimensionfull.
 *	else	"unit" is dimensionless.
 */
int
utIsDimensionless(
    utUnit* const	unit);


/*
 * Indicates if two units belong to the same unit-system.
 *
 * Arguments:
 *	unit1		Pointer to a unit.
 *	unit2		Pointer to another unit.
 * Returns:
 *	0		Failure or the units belong to different unit-systems.
 *			"utGetStatus()" will be
 *	    		    UT_NULL_ARG		"unit1" or "unit2" is NULL.
 *	    		    UT_SUCCESS		The units belong to different
 *						unit-systems.
 *	else		The units belong to the same unit-system.
 */
int
utSameSystem(
    const utUnit* const	unit1,
    const utUnit* const	unit2);


/*
 * Compares two units.  Returns a value less than, equal to, or greater than
 * zero as the first unit is considered less than, equal to, or greater than
 * the second unit, respectively.  Units from different unit-systems never
 * compare equal.
 *
 * Arguments:
 *	unit1		Pointer to a unit or NULL.
 *	unit2		Pointer to another unit or NULL.
 * Returns:
 *	<0	The first unit is less than the second unit.
 *	 0	The first and second units are equal or both units are NULL.
 *	>0	The first unit is greater than the second unit.
 */
int
utCompare(
    const utUnit* const	unit1,
    const utUnit* const	unit2);


/*
 * Indicates if numeric values in one unit are convertible to numeric values in
 * another unit via "utGetConverter()".  In making this determination, 
 * dimensionless units are ignored.
 *
 * Arguments:
 *	unit1		Pointer to a unit.
 *	unit2		Pointer to another unit.
 * Returns:
 *	0		Failure.  "utGetStatus()" will be
 *	    		    UT_NULL_ARG		"unit1" or "unit2" is NULL.
 *			    UT_NOT_SAME_SYSTEM	"unit1" and "unit2" belong to
 *						different unit-sytems.
 *			    UT_SUCCESS		Conversion between the units is
 *						not possible (e.g., "unit1" is
 *						"meter" and "unit2" is
 *						"kilogram").
 *	else	Numeric values can be converted between the units.
 */
int
utAreConvertible(
    utUnit* const	unit1,
    utUnit* const	unit2);


/*
 * Returns a converter of numeric values in one unit to numeric values in
 * another unit.  The returned converter should be passed to cvFree() when it is
 * no longer needed by the client.
 *
 * NOTE:  Leap seconds are not taken into account when converting between
 * timestamp units.
 *
 * Arguments:
 *	from		Pointer to the unit from which to convert values.
 *	to		Pointer to the unit to which to convert values.
 * Returns:
 *	NULL		Failure.  "utGetStatus()" will be:
 *			    UT_NULL_ARG		"from" or "to" is NULL.
 *			    UT_NOT_SAME_SYSTEM	"from" and "to" belong to
 *						different unit-systems.
 *			    UT_MEANINGLESS	Conversion between the units is
 *						not possible.  See
 *						"utAreConvertible()".
 *	else		Pointer to the appropriate converter.  The pointer
 *			should be passed to cvFree() when no longer needed by
 *			the client.
 */
cvConverter*
utGetConverter(
    utUnit* const	from,
    utUnit* const	to);


/******************************************************************************
 * Arithmetic Unit Manipulation:
 ******************************************************************************/


/*
 * Returns a unit equivalent to another unit scaled by a numeric factor,
 * e.g.,
 *	const utUnit*	meter = ...
 *	const utUnit*	kilometer = utScale(1000, meter);
 *
 * Arguments:
 *	factor		The numeric scale factor.
 *	unit		Pointer to the unit to be scaled.
 * Returns:
 *	NULL		Failure.  "utGetStatus()" will be
 *			    UT_BAD_VALUE	"factor" is 0.
 *			    UT_NULL_ARG		"unit" is NULL.
 *			    UT_OS		Operating-system error.  See
 *						"errno".
 *	else		Pointer to the resulting unit.  The pointer should be
 *			passed to utFree() when the unit is no longer needed by
 *			the client.
 */
utUnit*
utScale(
    const double	factor,
    utUnit* const	unit);


/*
 * Returns a unit equivalent to another unit offset by a numeric amount,
 * e.g.,
 *	const utUnit*	kelvin = ...
 *	const utUnit*	celsius = utOffset(kelvin, 273.15);
 *
 * Arguments:
 *	unit		Pointer to the unit to be offset.
 *	offset		The numeric offset.
 * Returns:
 *	NULL		Failure.  "utGetStatus()" will be
 *			    UT_NULL_ARG		"unit" is NULL.
 *			    UT_OS		Operating-system error.  See
 *						"errno".
 *	else		Pointer to the resulting unit.  The pointer should be
 *			passed to utFree() when the unit is no longer needed by
 *			the client.
 */
utUnit*
utOffset(
    utUnit* const	unit,
    const double	offset);


/*
 * Returns a unit equivalent to another unit relative to a particular time.
 * e.g.,
 *	const utUnit*	second = ...
 *	const utUnit*	secondsSinceTheEpoch =
 *              utOffsetByTime(second, utEncodeTime(1970, 1, 1, 0, 0, 0.0));
 *
 * Arguments:
 *	unit	Pointer to the time-unit to be made relative to a time-origin.
 *	origin	The origin as returned by utEncodeTime().
 * Returns:
 *	NULL	Failure.  "utGetStatus()" will be
 *		    UT_NULL_ARG		"unit" is NULL.
 *		    UT_OS		Operating-system error.  See "errno".
 *		    UT_MEANINGLESS	Creation of a timestamp unit based on
 *					"unit" is not meaningful.
 *		    UT_NO_SECOND	The associated unit-system doesn't
 *					contain a "second" unit.  See
 *					utSetSecond().
 *	else	Pointer to the resulting unit.  The pointer should be passed
 *		to utFree() when the unit is no longer needed by the client.
 */
utUnit*
utOffsetByTime(
    utUnit* const	unit,
    const double	origin);


/*
 * Returns the result of multiplying one unit by another unit.
 *
 * Arguments:
 *	unit1	Pointer to a unit.
 *	unit2	Pointer to another unit.
 * Returns:
 *	NULL	Failure.  "utGetStatus()" will be:
 *		    UT_NULL_ARG		"unit1" or "unit2" is NULL.
 *		    UT_NOT_SAME_SYSTEM	"unit1" and "unit2" belong to
 *					different unit-systems.
 *		    UT_OS		Operating-system error. See "errno".
 *	else	Pointer to the resulting unit.  The pointer should be passed
 *		to utFree() when the unit is no longer needed by the client.
 */
utUnit*
utMultiply(
    utUnit* const	unit1,
    utUnit* const	unit2);


/*
 * Returns the inverse (i.e., reciprocal) of a unit.  This convenience function
 * is equal to "utRaise(unit, -1)".
 *
 * Arguments:
 *	unit	Pointer to the unit.
 * Returns:
 *	NULL	Failure.  "utGetStatus()" will be:
 *		    UT_NULL_ARG		"unit" is NULL.
 *		    UT_OS		Operating-system error. See "errno".
 *	else	Pointer to the resulting unit.  The pointer should be passed to
 *		utFree() when the unit is no longer needed by the client.
 */
utUnit*
utInvert(
    utUnit* const	unit);


/*
 * Returns the result of dividing one unit by another unit.  This convenience
 * function is equivalent to the following sequence:
 *     {
 *         utUnit* inverse = utInvert(denom);
 *         utMultiply(numer, inverse);
 *         utFree(inverse);
 *     }
 *
 * Arguments:
 *	numer	Pointer to the numerator (top, dividend) unit.
 *	denom	Pointer to the denominator (bottom, divisor) unit.
 * Returns:
 *	NULL	Failure.  "utGetStatus()" will be:
 *		    UT_NULL_ARG		"numer" or "denom" is NULL.
 *		    UT_NOT_SAME_SYSTEM	"unit1" and "unit2" belong to
 *					different unit-systems.
 *		    UT_OS		Operating-system error. See "errno".
 *	else	Pointer to the resulting unit.  The pointer should be passed to
 *		utFree() when the unit is no longer needed by the client.
 */
utUnit*
utDivide(
    utUnit* const	numer,
    utUnit* const	denom);


/*
 * Returns the result of raising a unit to a power.
 *
 * Arguments:
 *	unit	Pointer to the unit.
 *	power	The power by which to raise "unit".  Must be greater than or 
 *		equal to -255 and less than or equal to 255.
 * Returns:
 *	NULL	Failure.  "utGetStatus()" will be:
 *		    UT_NULL_ARG		"unit" is NULL.
 *		    UT_BAD_VALUE	"power" is invalid.
 *		    UT_OS		Operating-system error. See "errno".
 *	else	Pointer to the resulting unit.  The pointer should be passed to
 *		utFree() when the unit is no longer needed by the client.
 */
utUnit*
utRaise(
    utUnit* const	unit,
    const int		power);


/*
 * Returns the logarithmic unit corresponding to a logarithmic base and a
 * reference level.  For example, the following creates a decibel unit with a
 * one milliwatt reference level:
 *
 *     const utUnit* watt = ...;
 *     const utUnit* milliWatt = utScale(0.001, watt);
 *
 *     if (milliWatt != NULL) {
 *         const utUnit* bel_1_mW = utLog(10.0, milliWatt);
 *
 *         if (bel_1_mW != NULL) {
 *             const utUnit* decibel_1_mW = utScale(0.1, bel_1_mW);
 *
 *             if (decibel_1_mW != NULL) {
 *                 ...
 *                 utFree(decibel_1_mW);
 *             }			// "decibel_1_mW" allocated
 *
 *             utFree(bel_1_mW);
 *         }				// "bel_1_mW" allocated
 *
 *         utFree(milliWatt);
 *     }				// "milliWatt" allocated
 *
 * Arguments:
 *	base		The logarithmic base (e.g., 2, M_E, 10).  Must be
 *                      greater than one.  "M_E" is defined in <math.h>.
 *	reference	Pointer to the reference value as a unit.
 * Returns:
 *	NULL		Failure.  "utGetStatus()" will be:
 *			    UT_BAD_VALUE	"base" is invalid.
 *			    UT_NULL_ARG		"reference" is NULL.
 *			    UT_OS		Operating-system error. See
 *						"errno".
 *	else		Pointer to the resulting unit.  The pointer should be
 *			passed to utFree() when the unit is no longer needed by
 *			the client.
 */
utUnit*
utLog(
    const double	base,
    utUnit* const	reference);


/******************************************************************************
 * Parsing and Formatting Units:
 ******************************************************************************/


/*
 * Returns the binary representation of a unit corresponding to a string
 * representation.
 *
 * Arguments:
 *	system		Pointer to the unit-system in which the parsing will
 *			occur.
 *	string		The string to be parsed (e.g., "millimeters").  There
 *			should be no leading or trailing whitespace in the
 *			string.  See utTrim().
 *	encoding	The encoding of "string".
 * Returns:
 *	NULL		Failure.  "utGetStatus()" will be one of
 *			    UT_NULL_ARG		"system" or "string" is NULL.
 *			    UT_SYNTAX		"string" contained a syntax
 *						error.
 *			    UT_UNKNOWN		"string" contained an unknown
 *						identifier.
 *			    UT_OS		Operating-system failure.  See
 *						"errno".
 *	else		Pointer to the unit corresponding to "string".
 */
utUnit*
utParse(
    utSystem* const	system,
    const char* const	string,
    const utEncoding	encoding);


/*
 * Returns the number of successfully parsed characters.  If utParse() was
 * successful, then the returned number will equal the length of the string;
 * otherwise, the returned number will be the 0-based index of the character
 * that caused the parse to fail.
 *
 * Returns:
 *	The number of successfully parsed characters.
 */
size_t
utGetParseLength(void);


/*
 * Removes leading and trailing whitespace from a string.
 *
 * Arguments:
 *	string		NUL-terminated string.  Will be modified if it contains
 *                      whitespace..
 *	encoding	The character-encoding of "string".
 * Returns:
 *      "string", with all leading and trailing whitespace removed.
 */
char*
utTrim(
    char* const	        string,
    const utEncoding	encoding);


/*
 * Formats a unit.
 *
 * Arguments:
 *	unit		Pointer to the unit to be formatted.
 *	buf		Pointer to the buffer into which to format "unit".
 *	size		Size of the buffer in bytes.
 *	opts		Formatting options: bitwise-OR of zero or more of the
 *			following:
 *			    UT_NAMES		Use unit names instead of
 *						symbols
 *                          UT_DEFINITION       The formatted string should be
 *                                              the definition of "unit" in
 *                                              terms of basic-units instead of
 *						stopping any expansion at the
 *						highest level possible.
 *			    UT_ASCII		The string should be formatted
 *						using the ASCII character set
 *						(default).
 *			    UT_LATIN1		The string should be formatted
 *						using the ISO Latin-1 (alias
 *						ISO-8859-1) character set.
 *			    UT_UTF8		The string should be formatted
 *						using the UTF-8 character set.
 *			UT_LATIN1 and UT_UTF8 are mutually exclusive: they may
 *			not both be specified.
 * Returns:
 *	-1		Failure:  "utGetStatus()" will be
 *			    UT_NULL_ARG		"unit" or "buf" is NULL
 *			    UT_BAD_VALUE	Both UT_LATIN1 and UT_UTF8
 *						specified.
 *			    UT_CANT_FORMAT	"unit" can't be formatted in
 *						the desired manner.
 *      else		Success.  Number of characters printed in "buf".  If
 *			the number is equal to the size of the buffer, then the
 *			buffer is too small to have a terminating NUL character.
 */
int
utFormat(
    const utUnit* const	unit,
    char*		buf,
    size_t		size,
    unsigned		opts);


/*
 * Accepts a visitor to a unit.
 *
 * Arguments:
 *	unit		Pointer to the unit to accept the visitor.
 *	visitor		Pointer to the visitor of "unit".
 *	arg		An arbitrary pointer that will be passed to "visitor".
 * Returns:
 *	UT_NULL_ARG	"unit" or "visitor" is NULL.
 *	UT_VISIT_ERROR	A error occurred in "visitor" while visiting "unit".
 *	UT_SUCCESS	Success.
 */
utStatus
utAcceptVisitor(
    const utUnit* const		unit,
    const utVisitor* const	visitor,
    void* const			arg);


/******************************************************************************
 * Time Handling:
 ******************************************************************************/


/*
 * Encodes a date as a double-precision value.
 *
 * Arguments:
 *	year		The year.
 *	month		The month.
 *	day		The day (1 = the first of the month).
 * Returns:
 *	The date encoded as a scalar value.
 */
double
utEncodeDate(
    int		year,
    int		month,
    int		day);


/*
 * Encodes a time as a double-precision value.
 *
 * Arguments:
 *	hours		The number of hours (0 = midnight).
 *	minutes		The number of minutes.
 *	seconds		The number of seconds.
 * Returns:
 *	The clock-time encoded as a scalar value.
 */
double
utEncodeClock(
    int		hours,
    int		minutes,
    double	seconds);


/*
 * Encodes a time as a double-precision value.  The convenience function is
 * equivalent to "utEncodeDate(year,month,day) + 
 * utEncodeClock(hour,minute,second)"
 *
 * Arguments:
 *	year	The year.
 *	month	The month.
 *	day	The day.
 *	hour	The hour.
 *	minute	The minute.
 *	second	The second.
 * Returns:
 *	The time encoded as a scalar value.
 */
double
utEncodeTime(
    const int		year,
    const int		month,
    const int		day,
    const int		hour,
    const int		minute,
    const double	second);


/*
 * Decodes a time from a double-precision value.
 *
 * Arguments:
 *      value           The value to be decoded.
 *      year            Pointer to the variable to be set to the year.
 *      month           Pointer to the variable to be set to the month.
 *      day             Pointer to the variable to be set to the day.
 *      hour            Pointer to the variable to be set to the hour.
 *      minute          Pointer to the variable to be set to the minute.
 *      second          Pointer to the variable to be set to the second.
 *      resolution      Pointer to the variable to be set to the resolution
 *                      of the decoded time in seconds.
 */
void
utDecodeTime(
    double	value,
    int		*year,
    int		*month,
    int		*day,
    int		*hour,
    int		*minute,
    double	*second,
    double	*resolution);


/******************************************************************************
 * Error Handling:
 ******************************************************************************/


/*
 * Returns the status of the last operation by the units module.  This function
 * will not change the status.
 */
utStatus
utGetStatus(void);


/*
 * Sets the status of the units module.  This function would not normally be
 * called by the user unless they were doing their own parsing or formatting.
 *
 * Arguments:
 *	status	The status of the units module.
 */
void
utSetStatus(
    utStatus	status);


/*
 * Handles an error-message.
 *
 * Arguments:
 *	fmt	The format for the error-message.
 *	...	The arguments for "fmt".
 * Returns:
 *	<0	An output error was encountered.
 *	else	The number of bytes of "fmt" and "arg" written excluding any
 *		terminating NUL.
 */
int
utHandleErrorMessage(
    const char* const	fmt,
    ...);


/*
 * Returns the previously-installed error-message handler and optionally
 * installs a new handler.  The initial handler is "utWriteToStderr()".
 *
 * Arguments:
 *      handler		NULL or pointer to the error-message handler.  If NULL,
 *			then the handler is not changed.  The 
 *			currently-installed handler can be obtained this way.
 * Returns:
 *	Pointer to the previously-installed error-message handler.
 */
utErrorMessageHandler
utSetErrorMessageHandler(
    utErrorMessageHandler	handler);


/*
 * Writes an error-message to the standard-error stream when received and
 * appends a newline.  This is the initial error-message handler.
 *
 * Arguments:
 *	fmt	The format for the error-message.
 *	args	The arguments of "fmt".
 * Returns:
 *	<0	A output error was encountered.  See "errno".
 *	else	The number of bytes of "fmt" and "arg" written excluding any
 *		terminating NUL.
 */
int
utWriteToStderr(
    const char* const	fmt,
    va_list		args);


/*
 * Does nothing with an error-message.
 *
 * Arguments:
 *	fmt	The format for the error-message.
 *	args	The arguments of "fmt".
 * Returns:
 *	0	Always.
 */
int
utIgnore(
    const char* const	fmt,
    va_list		args);


#ifdef __cplusplus
}
#endif

#endif
