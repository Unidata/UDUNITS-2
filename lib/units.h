#ifndef UT_UNITS_H_INCLUDED
#define UT_UNITS_H_INCLUDED

#include "converter.h"

typedef struct utSystem		utSystem;
typedef union utUnit		utUnit;


typedef enum utStatus {
    UT_SUCCESS = 0,	/* Success */
    UT_BADARG,		/* An argument is invalid (e.g., NULL) */
    UT_BADSYSTEM,	/* Unit-system argument is NULL */
    UT_BADID,		/* Identifier is NULL or empty */
    UT_BADVALUE,	/* Invalid numeric value */
    UT_EXISTS,		/* Unit, prefix, or identifier already exists */
    UT_BADUNIT,		/* Invalid unit argument */
    UT_NOUNIT,		/* No such unit exists */
    UT_OS,		/* Operating-system error.  See "errno". */
    UT_NOTSAMESYSTEM,	/* The units belong to different unit-systems */
    UT_MEANINGLESS,	/* The operation on the unit(s) is meaningless */
    UT_NOSECOND,	/* The unit-system doesn't have a unit named "second" */
    UT_BADBUF,		/* Character buffer argument is NULL */
    UT_BADVISITOR,	/* Visitor argument is NULL */
    UT_VISIT_ERROR,	/* An error occurred while visiting a unit */
    UT_CANT_FORMAT,	/* A unit can't be formatted in the desired manner */
    UT_INTERNAL,	/* Internal, assertion-type failure. Shouldn't occur. */
    UT_PARSE,
} utStatus;

typedef enum {
    UT_ASCII = 0,
    UT_LATIN1 = 1,
    UT_UTF8 = 2
} utEncoding;

#define UT_NAMES	4
#define UT_DEFINITION	8


/*
 * Data-structure for a visitor to a unit:
 */
typedef struct {
    utStatus	(*visitBasic)(const utUnit*, void*); 
    utStatus	(*visitProduct)(const utUnit*, int count,
	const utUnit* const* basicUnits, const int* powers, void*); 
    utStatus	(*visitGalilean)(const utUnit*, double scale,
	const utUnit* unit, double offset, void*); 
    utStatus	(*visitTimestamp)(const utUnit*, const utUnit* timeUnit,
	int year, int month, int day, int hour, int minute, double second, 
	double resolution, void*); 
    utStatus	(*visitLogarithmic)(const utUnit*, double logE,
	const utUnit* reference, void*); 
} utVisitor;


#ifdef __cplusplus
extern "C" {
#endif


/*
 * Returns the status of the last operation by this library.
 *
 * Returns one of:
 *	UT_SUCCESS
 *	UT_BADSYSTEM
 *	UT_BADID	
 *	UT_BADVALUE
 *	UT_EXISTS
 *	UT_BADUNIT
 *	UT_NOUNIT
 *	UT_INTERNAL
 *	UT_OS
 */
utStatus
utGetStatus();


/*
 * Returns a new unit system.
 *
 * Returns:
 *	NULL	Failure.  errno is set:
 *		    ENOMEM	Out of memory.
 *	else	Pointer to a new unit system.
 */
utSystem*
utNewSystem();


/*
 * Returns the unit-system to which a unit belongs.
 *
 * Arguments:
 *	unit	Pointer to the unit in question.
 * Returns:
 *	NULL	"unit" is NULL.
 *	else	Pointer to the unit-system to which "unit" belongs.
 */
utSystem*
utGetSystem(
    const utUnit* const	unit);


/*
 * Returns the dimensionless unit one associated with a unit-system.
 *
 * Arguments:
 *	system	The unit-system for which the dimensionless unit one will be
 *		returned.
 * Returns:
 *	NULL	Failure.  "utgetStatus()" will be:
 *		    UT_BADSYSTEM	"system" is NULL.
 *	else	The dimensionless unit one associated with "system".
 */
utUnit*
utGetDimensionlessUnitOne(
    utSystem* const	system);

/*
 * Adds a base-unit to a unit-system.
 *
 * Arguments:
 *	system	Pointer to the unit-system to which to add the new base-unit.
 * Returns:
 *	NULL	Failure.  "utGetStatus()" will be
 *		    UT_BADSYSTEM	"system" is NULL.
 *		    UT_BADID		"name" is NULL or empty.
 *		    UT_OS		Operating-system error.  See "errno".
 *		    UT_EXISTS		A unit with the same name already
 *					exists.
 *	else	Pointer to the new base-unit.
 */
utUnit*
utNewBaseUnit(
    utSystem* const	system);


/*
 * Adds a dimensionless-unit to a unit-system.  In the SI system of units,
 * the derived-unit radian is a dimensionless-unit.
 *
 * Arguments:
 *	system	Pointer to the unit-system to which to add the new
 *		dimensionless-unit.
 *	name	Pointer to the ASCII name of the unit.
 * Returns:
 *	NULL	Failure.  "utGetStatus()" will be
 *		    UT_BADSYSTEM	"system" is NULL.
 *		    UT_BADID		"name" is NULL.
 *		    UT_OS		Operating-system error.  See "errno".
 *		    UT_EXISTS		A unit with the same name already
 *					exists.
 *	else	Pointer to the new dimensionless-unit.
 */
utUnit*
utNewDimensionlessUnit(
    utSystem* const	system);

/*
 * Adds a mapping from a name to a unit.
 *
 * Arguments:
 *	name	The name.  Copied.
 *	unit	The unit.
 * Returns:
 *	UT_BADSYSTEM	"system" is NULL.
 *	UT_BADID	"name" is NULL or empty.
 *	UT_BADUNIT	"unit" is NULL.
 *	UT_NOMEM	Out of memory.
 *	UT_EXISTS	"name" already maps to a unit.
 */
utStatus
utMapNameToUnit(
    const char* const	name,
    utUnit* const	unit);


/*
 * Adds a mapping from a symbol to a unit.
 *
 * Arguments:
 *	symbol	The symbol.  Copied.
 *	unit	The unit.
 * Returns:
 *	UT_BADSYSTEM	"system" is NULL.
 *	UT_BADID	"symbol" is NULL or empty.
 *	UT_BADUNIT	"unit" is NULL.
 *	UT_NOMEM	Out of memory.
 *	UT_EXISTS	"symbol" already maps to a unit.
 */
utStatus
utMapSymbolToUnit(
    const char* const	symbol,
    utUnit* const	unit);


/*
 * Adds a name-prefix to a unit-system.
 *
 * Arguments:
 *	system	The unit-system.
 *	name	The name-prefix (e.g., "mega").
 *	value	The value of the prefix (e.g., 1e6).
 * Returns:
 *	UT_SUCCESS	Success.
 *	UT_BADSYSTEM	"system" is NULL.
 *	UT_BADNAME	"name" is NULL or empty.
 *	UT_BADVALUE	"value" is 0.
 *	UT_EXISTS	A prefix with the same name but a different value
 *			already exists.
 */
utStatus
utAddNamePrefix(
    utSystem* const	system,
    const char* const	name,
    const double	value);


/*
 * Adds a symbol-prefix to a unit-system.
 *
 * Arguments:
 *	system	The unit-system.
 *	symbol	The symbol-prefix (e.g., "M").
 *	value	The value of the prefix (e.g., 1e6).
 * Returns:
 *	UT_SUCCESS	Success.
 *	UT_BADSYSTEM	"system" is NULL.
 *	UT_BADSYMBOL	"symbol" is NULL or empty.
 *	UT_BADVALUE	"value" is 0.
 *	UT_EXISTS	A prefix with the same symbol but a different value
 *			already exists.
 */
utStatus
utAddSymbolPrefix(
    utSystem* const	system,
    const char* const	symbol,
    const double	value);


/*
 * Returns the total number of units in a unit system.
 *
 * Arguments:
 *	system	The unit system in question.
 * Returns:
 *	-1	"system" is NULL.
 *	else	The total number of units in "system".
 */
int
utSize(
    const utSystem* const	system);


/*
 * Returns the unit in a unit-system that has a given name.  Name comparisons
 * are case-insensitive.
 *
 * Arguments:
 *	system	The unit-system.
 *	name	The name of the unit to be returned.
 * Returns:
 *	NULL	Failure.  "utGetErrorCode(system)" will be
 *		    UT_BADSYSTEM	"system" is NULL.
 *		    UT_BADNAME		"name" is NULL.
 *		    UT_NOUNIT		No unit with name "name" was found in
 *					"system".
 *	else	The unit in the unit-system with the given name.
 */
utUnit*
utGetUnitByName(
    utSystem* const	system,
    const char* const	name);


/*
 * Returns the unit in a unit-system that has a given symbol.  Symbol 
 * comparisons are case-sensitive.
 *
 * Arguments:
 *	system	The unit-system.
 *	symbol	The symbol of the unit to be returned.
 * Returns:
 *	NULL	Failure.  "utGetErrorCode(system)" will be
 *		    UT_BADSYSTEM	"system" is NULL.
 *		    UT_BADSYMBOL	"symbol" is NULL.
 *		    UT_NOUNIT		No unit with symbol "symbol" was found
 *					in "system".
 *	else	The unit in the unit-system with the given name.
 */
utUnit*
utGetUnitBySymbol(
    utSystem* const	system,
    const char* const	symbol);


/*
 * Returns a base unit.
 *
 * ARGUMENTS:
 *	name	The (case insensitive) name of the unit.
 *	symbol	The (case sensitive) symbol for the unit.
 * RETURNS:
 *	NULL	Failure.  errno is set:
 *		    EINVAL	"name" or "symbol" is NULL or empty or a base
 *				unit with the same name or symbol already
 *				exists.
 *		    ENOMEM	Out-of-memory.
 */
utUnit*
utGetBaseUnit(
    const char* const	name,
    const char* const	symbol);


/*
 * Sets the second unit of a unit-system.  This function must be called before
 * the first call to "utOffsetByTime()".
 *
 * Arguments:
 *	system	The unit-system to have its second unit set.
 *	second	The second unit.
 * Returns:
 *	UT_BADSYSTEM	"system" is NULL.
 *	UT_BADUNIT	"second" is NULL.
 *	UT_BADUNIT	"utGetSystem(second) != system".
 *	UT_EXISTS	The second unit of "system" is set to a different unit.
 *	UT_SUCCESS	Success.
 */
utStatus
utSetSecond(
    utSystem* const	system,
    utUnit* const	second);


/*
 * Indicates if two units belong to the same unit-system.
 *
 * Arguments:
 *	unit1	Pointer to the first unit.
 *	unit2	Pointer to the second unit.
 * Returns:
 *	0	The units belong to different unit-systems.
 *	else	The units belong to the same unit-system.
 */
int
utSameSystem(
    const utUnit* const	unit1,
    const utUnit* const	unit2);


/*
 * Compares two units.  The behavior of this function is undefined if
 * "utSameSystem(unit1, unit2)" is false.
 *
 * Arguments:
 *	unit1	Pointer to the first unit.
 *	unit2	Pointer to the second unit in the same unit-system as "unit1".
 * Returns:
 *	<0	The first unit is less than the second unit.
 *	 0	The first and second units are equal.
 *	>0	The first unit is greater than the second unit.
 */
int
utCompare(
    const utUnit* const	unit1,
    const utUnit* const	unit2);


/*
 * Returns a unit equivalent to another unit scaled by a numeric factor,
 * e.g.,
 *	const utUnit*	centimeter = ...
 *	const utUnit*	inch = utScale(2.54, centimeter);
 *
 * Arguments:
 *	factor	The numeric scale factor.
 *	unit	The unit to be scaled.
 * Returns:
 *	NULL	Failure.  "utGetErrorCode(utGetSystem(unit))" will be
 *		    UT_BADVALUE		"factor" is 0.
 *		    UT_BADSYSTEM	"unit" is NULL.
 *		    UT_OS		Operating-system error.  See "errno".
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
 *	unit	The unit to be offset.
 *	offset	The numeric offset.
 * Returns:
 *	NULL	Failure.  "utGetStatus()" will be
 *		    UT_BADUNIT	"unit" is NULL.
 *		    UT_OS	Operating-system error.  See "errno".
 *	else	Pointer to offset-unit.
 */
utUnit*
utOffset(
    utUnit* const	unit,
    const double	offset);


/*
 * Returns the product of two units.
 *
 * ARGUMENTS:
 *	unit1	Pointer to the first unit.
 *	unit2	Pointer to the second unit.
 * RETURNS:
 *	NULL	Failure.  errno is set:
 *		    EINVAL	"unit1" or "unit2" is NULL.
 *		    ENOMEM	Out of memory.
 *	else	Pointer to unit corresponding to the product.
 */
utUnit*
utMultiply(
    utUnit* const	unit1,
    utUnit* const	unit2);


/*
 * Returns the reciprocal of a unit.
 *
 * ARGUMENTS:
 *	unit	Pointer to the unit.
 * RETURNS:
 *	NULL	Failure.  "utGetStatus()" will be:
 *		    UT_BADUNIT	"unit" is NULL.
 *		    UT_OS	Operating-system error. See "errno".
 *	else	Pointer to unit corresponding to the reciprocal.
 */
utUnit*
utInvert(
    utUnit* const	unit);


/*
 * Returns the result of dividing one unit by another unit.
 *
 * ARGUMENTS:
 *	numer	Pointer to the numerator (top, dividend) unit.
 *	denom	Pointer to the denominator (bottom, divisor) unit.
 * RETURNS:
 *	NULL	Failure.  "utGetStatus()" will be:
 *		    UT_BADUNIT	"numer" or "denom" is NULL.
 *		    UT_OS	Operating-system error. See "errno".
 *	else	Pointer to unit corresponding to the product.
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
 *		    UT_BADUNIT	"unit" is NULL.
 *		    UT_BADVALUE	"power" is invalid.
 *		    UT_OS	Operating-system error. See "errno".
 *	else	Pointer to unit corresponding to the reciprocal.
 */
utUnit*
utRaise(
    utUnit* const	unit,
    const int		power);


/*
 * Returns the logarithmic unit corresponding to a logarithmic base and a 
 * reference value.  For example, the following:
 *
 *     #include <math.h>
 *     ...
 *     const utUnit*	watt = ...;
 *     const utUnit*	bel_1_mW = utLog(M_LOG10E, utScale(0.001, watt));
 *     const utUnit*	dB_1_mW = utScale(0.1, bel_1_mW);
 *
 * creates a Bel unit with a one milliwatt reference power and a decibel
 * unit, also with a one milliwatt reference power.
 *
 * Arguments:
 *	logE		The logarithm of "e" in the logarithmic base.  Must
 *			be positive.
 *	reference	The reference value.
 * Returns:
 *	NULL	Failure.  "utGetStatus()" will be:
 *		    UT_BADVALUE	"logE" is invalid.
 *		    UT_BADUNIT	"reference" is NULL.
 *		    UT_OS	Operating-system error. See "errno".
 *	else	Pointer to corresponding logarithmic unit.
 */
utUnit*
utLog(
    const double	logE,
    utUnit* const	reference);


/*
 * Returns a unit equivalent to another unit relative to a particular time.
 * e.g.,
 *	const utUnit*	second = ...
 *	const utUnit*	secondsSinceTheEpoch =
 *	    utOffsetByTime(day, 1970, 1, 1, 0, 0, 0.0);
 *
 * Arguments:
 *	unit	The unit to be offset.
 *	year	The year of the origin.
 *	month	The month of the origin (1 through 12).
 *	day	The day of the origin (1 through 31).
 *	hour	The hour of the origin (0 through 23).
 *	minute	The minute of the origin (0 through 59).
 *	second	The second of the origin (0 through 61).
 * Returns:
 *	NULL	Failure.  "utGetStatus()" will be
 *		    UT_BADUNIT		"unit" is NULL.
 *		    UT_OS		Operating-system error.  See "errno".
 *		    UT_MEANINGLESS	Creation of a timestamp unit based on
 *					"unit" is not meaningful.
 *	else	Pointer to the timestamp-unit.
 */
utUnit*
utOffsetByTime(
    utUnit* const	unit,
    const int		year,
    const int		month,
    const int		day,
    const int		hour,
    const int		minute,
    const double	second);


/*
 * Returns a clone of a unit.
 *
 * Arguments:
 *	unit	Pointer to the unit to be cloned.
 * Returns:
 *	NULL	Failure.  utGetStatus() will be
 *		    UT_OS	Operating-system failure.  See "errno".
 *	else	Pointer to the clone of "unit".  Might equal "unit".
 */
utUnit*
utClone(
    const utUnit*	unit);


/*
 * Frees resources associated with a unit, when appropriate.  This function
 * should be invoked when a unit is no longer needed.  If the unit has been
 * added to the unit-system, then this function has no effect; otherwise, use of
 * the unit upon return from this function may result in undefined behavior.
 *
 * ARGUMENTS:
 *	unit	Pointer to the unit to have it resources freed or NULL.
 */
void
utFree(
    const utUnit* const unit);


/*
 * Indicates if numeric values in one unit are convertible to numeric
 * values in another unit.  In making this determination, units created by
 * "utNewDimensionlessUnit()" are ignored.
 *
 * Arguments:
 *	unit1	One unit.
 *	unit2	The other unit.
 * Returns:
 *	0	Numeric values cannot be converted between the units.  
 *		"utGetStatus()" will be
 *	    	    UT_BADUNIT		"unit1" or "unit2" is NULL.
 *		    UT_NOTSAMESYSTEM	"unit1" and "unit2" belong to different
 *					unit-sytems.
 *		    UT_SUCCESS		Conversion between the units is not
 *					possible.
 *	else	Numeric values can be converted between the units.
 */
int
utAreConvertible(
    utUnit* const	unit1,
    utUnit* const	unit2);


/*
 * Returns a converter of values in one unit to values in another unit.  The
 * returned converter should be freed by cvFree() when it is no longer needed.
 *
 * Arguments:
 *	from	The unit from which to convert values.
 *	to	The unit to which to convert values.
 * Returns:
 *	NULL	Failure.  "utGetStatus()" will be:
 *		    UT_BADUNIT		"from" or "to" is NULL.
 *		    UT_NOTSAMESYSTEM	"from" and "to" belong to different
 *					unit-systems.
 *	else	Pointer to the appropriate converter.
 */
cvConverter*
utGetConverter(
    utUnit* const	from,
    utUnit* const	to);


/*
 * Parses a string representation of a unit and returns the corresponding unit.
 *
 * Arguments:
 *	system	Pointer to the unit-system in which the parsing will occur.
 *	string	The string to be parsed (e.g., "millimeters").  There should be
 *		no leading or trailing whitespace in the string.
 *	nchar	NULL or pointer to storage for the number of characters at the
 *		beginning of "string" that correspond to the returned unit.
 * Returns:
 *	NULL	"string" is NULL.
 *	NULL	"string" couldn't be parsed into a known unit.
 *	else	The unit corresponding to the first "*nchar" characters of
 *		"string".
 */
utUnit*
utParse(
    utSystem* const	system,
    const char* const	string,
    const utEncoding	encoding,
    int*		nchar);


/*
 * Accepts a visitor to a unit.
 *
 * Arguments:
 *	unit	Pointer to the unit that will accept the visitor.
 *	visitor	Pointer to the visitor of "unit".
 *	arg	An arbitrary pointer that will be passed to "visitor".
 * Returns:
 *	UT_BADUNIT	"unit" is NULL.
 *	UT_BADVISITOR	"visitor" is NULL.
 *	UT_VISIT_ERROR	A error occurred in "visitor" while visiting "unit".
 *	UT_SUCCESS	Success.
 */
utStatus
utAcceptVisitor(
    const utUnit* const		unit,
    const utVisitor* const	visitor,
    void* const			arg);


/*
 * Formats a unit.
 *
 * Arguments:
 *	unit		Pointer to the unit to be formatted.
 *	buf		Pointer to the buffer into which to print the formatted
 *			unit.
 *	size		Size of the buffer in bytes.
 *	useNames	Whether or not to use unit names rather than unit
 *			symbols.
 *	getDefinition	Whether or not the returned string should be the
 *			definition of "unit" in terms of basic-units.  If
 *			false, then the expansion of "unit" will stop at the
 *			highest level possible; otherwise, the expansion will
 *			continue down to basic-units.
 *	encoding	The type of encoding to use.
 * Returns:
 *	-1	Failure:  "utStatus()" will be
 *		    UT_BADUNIT		"unit" is NULL.
 *		    UT_BADBUF		"buf" is NULL.
 *		    UT_CANT_FORMAT	"unit" can't be formatted in the desired
 *					manner.
 *      else    Success.  Number of characters printed in "buf".  If the number
 *              is equal to the size of the buffer, then the buffer is too small
 *              to have a terminating NUL character.
 */
int
utFormat(
    const utUnit* const	unit,
    char*		buf,
    size_t		size,
    unsigned		opts);


#ifdef __cplusplus
}
#endif

#endif
