/*
 * Unit creation and manipulation routines for the udunits(3) library.
 *
 * The following data-structures exist in this module:
 *	BasicUnit	Like an ISO "base unit" but also for dimensionless
 *			units (e.g., "radian").
 *	ProductUnit	A unit that, when it is created, contains all the
 *			BasicUnit-s that exist at the time, each raised
 *			to an integral power (that can be zero).
 *	GalileanUnit	A unit whose value is related to another unit by a 
 *			Galilean transformation (y = ax + b).  Examples include
 *			"yard" and "degrees Fahrenheit".
 *	LogUnit		A unit that is related to another unit by a logarithmic
 *			transformation (y = a*log(x)).  The "Bel" is an example.
 *	TimestampUnit	A wrong-headed unit that shouldn't exist but does for
 *			backward compatibility.  It was intended to provide 
 *			similar functionality as the GalileanUnit, but for time
 *			units (e.g., "seconds since the epoch").  Unfortunately,
 *			people try to use it for more than it is capable (e.g.,
 *			days since some time on an imaginary world with only 360
 *			days per year).
 *	utUnit		A data-structure that encapsulates ProductUnit, 
 *			GalileanUnit, LogUnit, and TimestampUnit.
 *
 * This module is thread-compatible but not thread-safe: multi-thread access to
 * this module must be externally synchronized.
 *
 * $Id: core.c,v 1.1 2006/11/16 20:21:06 steve Exp $
 */

/*LINTLIBRARY*/

#ifndef	_XOPEN_SOURCE
#   define _XOPEN_SOURCE 500
#endif

#include <assert.h>
#include <ctype.h>
#include <float.h>
#include <inttypes.h>
#include <limits.h>
#include <math.h>
#include <search.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "units.h"		/* this module's API */
#include "converter.h"

typedef enum {
    PRODUCT_EQUAL = 0,		/* The units are equal -- ignoring dimensionless
				 * basic-units */
    PRODUCT_INVERSE,		/* The units are reciprocals of each other */
    PRODUCT_UNCONVERTIBLE,	/* The units have incompatible dimensionality */
    PRODUCT_UNKNOWN		/* The relationship is unknown */
} ProductRelationship;

typedef struct BasicUnit	BasicUnit;
typedef struct ProductUnit	ProductUnit;

struct utSystem {
    utUnit*		second;
    utUnit*		one;		/* the dimensionless-unit one */
    BasicUnit**		basicUnits;
    int			basicCount;
    int			size;
};

typedef struct {
    ProductUnit*	(*getProduct)(const utUnit*);
    utUnit*		(*clone)(const utUnit*);
    void		(*free)(utUnit*);
    /*
     * The following comparison function must only be called if the units are
     * the same type.
     */
    int			(*compare)(const utUnit*, const utUnit*);
    utUnit*		(*multiply)(utUnit*, utUnit*);
    utUnit*		(*raise)(utUnit*, const int power);
    int			(*initConverterToProduct)(utUnit*);
    int			(*initConverterFromProduct)(utUnit*);
    utStatus		(*acceptVisitor)(const utUnit*, const utVisitor*,
			    void*);
} UnitOps;

typedef enum {
    BASIC,
    PRODUCT,
    GALILEAN,
    LOG,
    TIMESTAMP,
} UnitType;

#undef	ABS
#define	ABS(a)		((a) < 0 ? -(a) : (a))
#undef	MIN
#define	MIN(a,b)	((a) < (b) ? (a) : (b))
#undef	MAX
#define	MAX(a,b)	((a) > (b) ? (a) : (b))

#define GET_PRODUCT(unit) \
			((unit)->common.ops->getProduct(unit))
#define CLONE(unit)	((unit)->common.ops->clone(unit))
#define MULTIPLY(unit1, unit2) \
			((unit1)->common.ops->multiply(unit1, unit2))
#define RAISE(unit, power) \
			((unit)->common.ops->raise(unit, power))
#define FREE(unit)	((unit)->common.ops->free(unit))
#define COMPARE(unit1, unit2) \
			((unit1)->common.ops->compare(unit1, unit2))
#define ENSURE_CONVERTER_TO_PRODUCT(unit) \
			((unit)->common.toProduct != NULL || \
			(unit)->common.ops->initConverterToProduct(unit) == 0)
#define ENSURE_CONVERTER_FROM_PRODUCT(unit) \
			((unit)->common.fromProduct != NULL || \
			(unit)->common.ops->initConverterFromProduct(unit) == 0)
#define ACCEPT_VISITOR(unit, visitor, arg) \
			((unit)->common.ops->acceptVisitor(unit, visitor, arg))

typedef struct {
    utSystem*		system;
    const UnitOps*	ops;
    UnitType		type;
    cvConverter*	toProduct;
    cvConverter*	fromProduct;
} Common;

struct BasicUnit {
    Common		common;
    ProductUnit*	product;		/* equivalent product-unit */
    int			index;			/* system->basicUnits index */
    int			isDimensionless;
};

struct ProductUnit {
    Common		common;
    short*		indexes;
    short*		powers;
    int			count;
};

typedef struct {
    Common		common;
    utUnit*		unit;
    double		scale;
    double		offset;
} GalileanUnit;

typedef struct {
    Common		common;
    utUnit*		unit;
    double		origin;
} TimestampUnit;

typedef struct {
    Common		common;
    utUnit*		reference;
    double		logE;
} LogUnit;

union utUnit {
    Common		common;
    BasicUnit		basic;
    ProductUnit		product;
    GalileanUnit	galilean;
    TimestampUnit	timestamp;
    LogUnit		log;
};

#define IS_BASIC(unit)		((unit)->common.type == BASIC)
#define IS_PRODUCT(unit)	((unit)->common.type == PRODUCT)
#define IS_GALILEAN(unit)	((unit)->common.type == GALILEAN)
#define IS_LOG(unit)		((unit)->common.type == LOG)
#define IS_TIMESTAMP(unit)	((unit)->common.type == TIMESTAMP)

static utUnit*		productMultiply(
    utUnit* const		unit1,
    utUnit* const		unit2);
static utUnit*		productRaise(
    utUnit* const		unit,
    const int			power);
static int		productInitConverterToProduct(
    utUnit* const		unit);
static int		productInitConverterFromProduct(
    utUnit* const		unit);
static void		productFree(
    utUnit* const		unit);
static BasicUnit*	basicNew(
    utSystem* const		system,
    const int			isDimensionless);
static ProductUnit*	productNew(
    utSystem* const		system,
    const short* const		indexes,
    const short* const		powers,
    const int			count);
static utUnit*		galileanNew(
    double			scale,
    utUnit*			unit,
    double			offset);
static utUnit*		timestampNewOrigin(
    utUnit*			unit,
    const double		origin);
static utUnit*		timestampNew(
    utUnit*			unit,
    const int			year,
    const int			month,
    const int			day,
    const int			hour,
    const int			minute,
    const double		second);
static utUnit*		logNew(
    const double		logE,
    utUnit* const		reference);

utStatus		unitStatus = UT_SUCCESS;
static long		juldayOrigin = 0;


/*
 * The following two functions convert between Julian day number and
 * Gregorian/Julian dates (Julian dates are used prior to October 15,
 * 1582; Gregorian dates are used after that).  Julian day number 0 is
 * midday, January 1, 4713 BCE.  The Gregorian calendar was adopted
 * midday, October 15, 1582.
 *
 * Author: Robert Iles, March 1994
 *
 * C Porter: Steve Emmerson, October 1995
 *
 * Original: http://www.nag.co.uk:70/nagware/Examples/calendar.f90
 *
 * There is no warranty on this code.
 */


/*
 * Convert a Julian day number to a Gregorian/Julian date.
 */
void
julianDayToGregorianDate(julday, year, month, day)
    long	julday;		/* Julian day number to convert */
    int		*year;		/* Gregorian year (out) */
    int		*month;		/* Gregorian month (1-12) (out) */
    int		*day;		/* Gregorian day (1-31) (out) */
{
    long	ja, jb, jd;
    int		jc;
    int		je, iday, imonth, iyear;
    double	xc;

    if (julday < 2299161)
	ja = julday;
    else
    {
	int	ia = (int)(((julday - 1867216) - 0.25) / 36524.25);

	ja = julday + 1 + ia - (int)(0.25 * ia);
    }

    jb = ja + 1524;
    xc = ((jb - 2439870) - 122.1) / 365.25;
    jc = (int)(6680.0 + xc);
    jd = 365 * jc + (int)(0.25 * jc);
    je = (int)((jb - jd) / 30.6001);

    iday = (int)(jb - jd - (int)(30.6001 * je));

    imonth = je - 1;
    if (imonth > 12)
	imonth -= 12;

    iyear = jc - 4715;
    if (imonth > 2)
	iyear -= 1;
    if (iyear <= 0)
	iyear -= 1;

    *year = iyear;
    *month = imonth;
    *day = iday;
}


/*
 * Convert a Gregorian/Julian date to a Julian day number.
 *
 * The Gregorian calendar was adopted midday, October 15, 1582.
 */
long
gregorianDateToJulianDay(year, month, day)
    int		year;	/* Gregorian year */
    int		month;	/* Gregorian month (1-12) */
    int		day;	/* Gregorian day (1-31) */
{
    int32_t	igreg = 15 + 31 * (10 + (12 * 1582));
    int32_t	iy;	/* signed, origin 0 year */
    int32_t	ja;	/* Julian century */
    int32_t	jm;	/* Julian month */
    int32_t	jy;	/* Julian year */
    long	julday;	/* returned Julian day number */

    /*
     * Because there is no 0 BC or 0 AD, assume the user wants the start of 
     * the common era if they specify year 0.
     */
    if (year == 0)
	year = 1;

    iy = year;
    if (year < 0)
	iy++;
    if (month > 2)
    {
	jy = iy;
	jm = month + 1;
    }
    else
    {
	jy = iy - 1;
	jm = month + 13;
    }

    /*
     *  Note: SLIGHTLY STRANGE CONSTRUCTIONS REQUIRED TO AVOID PROBLEMS WITH
     *        OPTIMISATION OR GENERAL ERRORS UNDER VMS!
     */
    julday = day + (int)(30.6001 * jm);
    if (jy >= 0)
    {
	julday += 365 * jy;
	julday += 0.25 * jy;
    }
    else
    {
	double	xi = 365.25 * jy;

	if ((int)xi != xi)
	    xi -= 1;
	julday += (int)xi;
    }
    julday += 1720995;

    if (day + (31* (month + (12 * iy))) >= igreg)
    {
	ja = jy/100;
	julday -= ja;
	julday += 2;
	julday += ja/4;
    }

    return julday;
}


/*
 * Encode a time as a double-precision value.
 */
static double
encodeClock(hours, minutes, seconds)
    int		hours;
    int		minutes;
    double	seconds;
{
    return (hours*60 + minutes)*60 + seconds;
}


/*
 * Decompose a value into a set of values accounting for uncertainty.
 */
static void
decompose(value, uncer, nbasis, basis, count)
    double	value;
    double	uncer;		/* >= 0 */
    int		nbasis;
    double	*basis;		/* all values > 0 */
    double	*count;
{
    int		i;

    for (i = 0; i < nbasis; i++)
    {
	double	r = fmod(value, basis[i]);	/* remainder */

	/* Adjust remainder to minimum magnitude. */
	if (ABS(2*r) > basis[i])
	    r += r > 0
		    ? -basis[i]
		    :  basis[i];

	if (ABS(r) <= uncer)
	{
	    /* The value equals a basis multiple within the uncertainty. */
	    double	half = value < 0 ? -basis[i]/2 : basis[i]/2;
	    modf((value+half)/basis[i], count+i);
	    break;
	}

	value = basis[i] * modf(value/basis[i], count+i);
    }

    if (i >= nbasis) {
	count[--i] += value;
    }
    else {
	for (i++; i < nbasis; i++)
	    count[i] = 0;
    }
}


/*
 * Encode a date as a double-precision value.
 */
static double
encodeDate(year, month, day)
    int		year;
    int		month;
    int		day;
{
    if (juldayOrigin == 0)
	juldayOrigin = gregorianDateToJulianDay(2001, 1, 1);

    return 86400.0 *
	(gregorianDateToJulianDay(year, month, day) - juldayOrigin);
}


/*
 * Encodes a time as a double-precision value.
 */
static double
encodeTime(
    const int		year,
    const int		month,
    const int		day,
    const int		hour,
    const int		minute,
    const double	second)
{
    return encodeDate(year, month, day) + encodeClock(hour, minute, second);
}


/*
 * Decode a time from a double-precision value.
 */
static void
decodeTime(value, year, month, day, hour, minute, second, resolution)
    double	value;
    int		*year;
    int		*month;
    int		*day;
    int		*hour;
    int		*minute;
    double	*second;
    double	*resolution;
{
    int			days;
    int			hours;
    int			minutes;
    double		seconds;
    double		uncer;		/* uncertainty of input value */
    typedef union
    {
	double	    vec[7];
	struct
	{
	    double	days;
	    double	hours12;
	    double	hours;
	    double	minutes10;
	    double	minutes;
	    double	seconds10;
	    double	seconds;
	}	    ind;
    } Basis;
    Basis		counts;
    static Basis	basis = {86400, 43200, 3600, 600, 60, 10, 1};

    uncer = ldexp(value < 0 ? -value : value, -DBL_MANT_DIG);

    days = (int)floor(value/basis.ind.days);
    value -= days * basis.ind.days;		/* make positive excess */

    decompose(value, uncer, (int)(sizeof(basis.vec)/sizeof(basis.vec[0])),
	   basis.vec, counts.vec);

    days += counts.ind.days;
    hours = (int)counts.ind.hours12 * 12 + (int)counts.ind.hours;
    minutes = (int)counts.ind.minutes10 * 10 + (int)counts.ind.minutes;
    seconds = (int)counts.ind.seconds10 * 10 + counts.ind.seconds;

    if (seconds >= 60) {
	seconds -= 60;
	if (++minutes >= 60) {
	    minutes -= 60;
	    if (++hours >= 24) {
		hours -= 24;
		days++;
	    }
	}
    }

    *second = seconds;
    *minute = minutes;
    *hour = hours;
    *resolution = uncer;

    julianDayToGregorianDate(juldayOrigin + days, year, month, day);
}


/******************************************************************************
 * Parameters common to all types of units:
 ******************************************************************************/


/*
 * Arguments:
 *	common	Pointer to unit common-area.
 *	ops	Pointer to unit-specific function-structure.
 *	system	Pointer to unit-system.
 *	type	The type of unit.
 * Returns:
 *	-1	Failure.  "unitStatus" is set:
 *		    UT_INTERNAL	"common" is NULL.
 *		    UT_INTERNAL	"ops" is NULL.
 *		    UT_INTERNAL	"system" is NULL.
 *	 0	Success.
 */
static int
commonInit(
    Common* const		common,
    const UnitOps* const	ops,
    const utSystem* const	system,
    const UnitType		type)
{
    int	retval;

    if (system == NULL || common == NULL || ops == NULL) {
	unitStatus = UT_INTERNAL;
	retval = -1;
    }
    else {
	common->system = (utSystem*)system;
	common->ops = ops;
	common->type = type;
	common->toProduct = NULL;
	common->fromProduct = NULL;
	retval = 0;
    }

    return retval;
}


/******************************************************************************
 * Basic-Unit:
 ******************************************************************************/


static ProductUnit*
basicGetProduct(
    const utUnit* const	unit)
{
    return unit->basic.product;
}


static utUnit*
basicClone(
    const utUnit* const	unit)
{
    return (utUnit*)unit;
}


static void
basicFree(
    utUnit* const	unit)
{
    free(unit);
}


static void
basicDontFree(
    utUnit* const	unit)
{
}


static int
basicCompare(
    const utUnit* const	unit1,
    const utUnit* const	unit2)
{
    return unit1 < unit2 ? -1 : unit1 == unit2 ? 0 : 1;
}


/*
 * Multiplies a basic-unit by another unit.
 *
 * Arguments:
 *	unit1	The basic-unit.
 *	unit2	The other unit.
 * Returns:
 *	NULL	Failure.  "unitStatus" is set:
 *		    UT_INTERNAL		"unit1" is NULL.
 *		    UT_INTERNAL		"unit2" is NULL.
 *		    UT_INTERNAL		"unit1" is not a product-unit.
 *		    UT_MEANINGLESS	The operation on the given units is
 *					meaningless.
 *	else	The resulting unit.
 */
static utUnit*
basicMultiply(
    utUnit* const	unit1,
    utUnit* const	unit2)
{
    utUnit*		result = NULL;	/* failure */

    if (unit1 == NULL || !IS_BASIC(unit1) || unit2 == NULL) {
	unitStatus = UT_INTERNAL;
    }
    else {
	result = productMultiply((utUnit*)unit1->basic.product, unit2);
    }

    return result;
}


/*
 * Returns the result of raising a basic-unit to a power.
 *
 * Arguments:
 *	unit	The basic-unit.
 *	power	The power.  Must be greater than -256 and less than 256.
 * Returns:
 *	NULL	Failure.  "unitStatus" is set:
 *		    UT_INTERNAL		"unit" is NULL.
 *		    UT_INTERNAL		"unit" is not a product-unit.
 *		    UT_INTERNAL		"power" is out-of-range
 *		    UT_MEANINGLESS	The operation on the given unit is
 *					meaningless.
 *	else	The resulting unit.
 */
static utUnit*
basicRaise(
    utUnit* const	unit,
    const int		power)
{
    utUnit*		result = NULL;	/* failure */

    if (unit == NULL || !IS_BASIC(unit) || power < -255 || power > 255) {
	unitStatus = UT_INTERNAL;
    }
    else if (power == 0) {
	result = unit->common.system->one;
    }
    else if (power == 1) {
	result = unit;
    }
    else {
	result = productRaise((utUnit*)unit->basic.product, power);
    }

    return result;
}


/*
 * Initializes the converter of numeric from the given product-unit to the
 * underlying product-unit (i.e., to itself).
 *
 * Arguments:
 *	unit	The product unit.
 * Returns:
 *	-1	Failure.  "unitStatus" is set:
 *		    UT_INTERNAL	"unit" is NULL.
 *		    UT_INTERNAL	"unit" is not a product-unit.
 *	 0	Success.
 */
static int
basicInitConverterToProduct(
    utUnit* const	unit)
{
    if (unit->common.toProduct == NULL)
	unit->common.toProduct = cvGetTrivial();

    return 0;
}


/*
 * Initializes the converter of numeric to the given product-unit from the
 * underlying product-unit (i.e., to itself).
 *
 * Arguments:
 *	unit	The product unit.
 * Returns:
 *	-1	Failure.  "unitStatus" is set:
 *		    UT_INTERNAL	"unit" is NULL.
 *		    UT_INTERNAL	"unit" is not a product-unit.
 *	 0	Success.
 */
static int
basicInitConverterFromProduct(
    utUnit* const	unit)
{
    if (unit->common.fromProduct == NULL)
	unit->common.fromProduct = cvGetTrivial();

    return 0;
}


static utStatus
basicAcceptVisitor(
    const utUnit* const		unit,
    const utVisitor* const	visitor,
    void* const			arg)
{
    return visitor->visitBasic(unit, arg);
}


static UnitOps	basicOps = {
    basicGetProduct,
    basicClone,
    basicDontFree,
    basicCompare,
    basicMultiply,
    basicRaise,
    basicInitConverterToProduct,
    basicInitConverterFromProduct,
    basicAcceptVisitor
};


/*
 * Returns a new instance of a basic-unit.
 *
 * Arguments:
 *	system		The unit-system in which to create a new basic-unit.
 *	isDimensionless	Whether or not the unit is dimensionless (e.g., 
 *			"radian").
 * Returns:
 *	NULL	Failure.  "unitStatus" is set:
 *		    UT_INTERNAL	"system == NULL"
 *		    UT_INTERNAL	"name" is NULL
 *		    UT_INTERNAL	"productUnit" is NULL
 *		    UT_BADID	"name" is non-ASCII.
 *		    UT_OS	Operating-system error.  See "errno".
 *	else	Pointer to newly-allocated basic-unit.
 */
static BasicUnit*
basicNew(
    utSystem* const	system,
    const int		isDimensionless)
{
    BasicUnit*	basicUnit = NULL;	/* failure */

    if (system == NULL) {
	unitStatus = UT_INTERNAL;
    }
    else {
	int		error = 1;
	short		index = system->basicCount;
	short		power = 1;
	ProductUnit*	product = productNew(system, &index, &power, 1);

	if (product == NULL) {
	    unitStatus = UT_OS;
	}
	else {
	    basicUnit = malloc(sizeof(BasicUnit));

	    if (basicUnit == NULL) {
		unitStatus = UT_OS;
	    }
	    else if (commonInit(&basicUnit->common, &basicOps, system,
		    BASIC) == 0) {
		basicUnit->index = index;
		basicUnit->isDimensionless = isDimensionless;
		basicUnit->product = product;
		error = 0;
	    }				/* "basicUnit" allocated */

	    if (error)
		productFree((utUnit*)product);
	}				/* "product" allocated */
    }					/* valid arguments */

    return basicUnit;
}


/******************************************************************************
 * Product Unit:
 ******************************************************************************/


static ProductUnit*
productGetProduct(
    const utUnit* const	unit)
{
    return (ProductUnit*)&unit->product;
}


static utUnit*
productClone(
    const utUnit* const	unit)
{
    utUnit*		clone;

    if (unit == NULL) {
	clone = NULL;
	unitStatus = UT_INTERNAL;
    }
    else {
	clone = (utUnit*)productNew(unit->common.system, unit->product.indexes,
	    unit->product.powers, unit->product.count);
    }

    return clone;
}


static int
productCompare(
    const utUnit* const	unit1,
    const utUnit* const	unit2)
{
    const ProductUnit* const	product1 = &unit1->product;
    const ProductUnit* const	product2 = &unit2->product;
    int				result = product1->count - product2->count;

    if (result == 0) {
	const short* const	indexes1 = product1->indexes;
	const short* const	indexes2 = product2->indexes;
	const short* const	powers1	= product1->powers;
	const short* const	powers2	= product2->powers;
	int			i;

	for (i = 0; i < product1->count; ++i) {
	    result = indexes1[i] - indexes2[i];

	    if (result == 0)
		result = powers1[i] - powers2[i];

	    if (result != 0)
		break;
	}
    }

    return result;
}


static void
productFree(
    utUnit* const	unit)
{
    if (unit != NULL) {
	free(unit->product.indexes);
	free(unit);
    }
}


/*
 * Multiplies a product-unit by another unit.
 *
 * Arguments:
 *	unit1	The product unit.
 *	unit2	The other unit.
 * Returns:
 *	NULL	Failure.  "unitStatus" is set:
 *		    UT_INTERNAL		"unit1" is NULL.
 *		    UT_INTERNAL		"unit2" is NULL.
 *		    UT_INTERNAL		"unit1" is not a product-unit.
 *		    UT_MEANINGLESS	The operation on the given units is
 *					meaningless.
 *	else	The resulting unit.
 */
static utUnit*
productMultiply(
    utUnit* const	unit1,
    utUnit* const	unit2)
{
    utUnit*		result = NULL;	/* failure */

    if (unit1 == NULL || !IS_PRODUCT(unit1) || unit2 == NULL) {
	unitStatus = UT_INTERNAL;
    }
    else {
	const ProductUnit* const	product1 = &unit1->product;

	if (!IS_PRODUCT(unit2)) {
	    result = MULTIPLY(unit2, unit1);
	}
	else {
	    const ProductUnit* const	product2 = &unit2->product;
	    short*			indexes1 = product1->indexes;
	    short*			indexes2 = product2->indexes;
	    short*			powers1 = product1->powers;
	    short*			powers2 = product2->powers;
	    int				count1 = product1->count;
	    int				count2 = product2->count;
	    int				sumCount = count1 + count2;
	    short			indexes[sumCount];
	    short			powers[sumCount];
	    int				count = 0;
	    int				i1 = 0;
	    int				i2 = 0;

	    while (i1 < count1 || i2 < count2) {
		if (i1 >= count1) {
		    indexes[count] = indexes2[i2];
		    powers[count++] = powers2[i2++];
		}
		else if (i2 >= count2) {
		    indexes[count] = indexes1[i1];
		    powers[count++] = powers1[i1++];
		}
		else if (indexes1[i1] > indexes2[i2]) {
		    indexes[count] = indexes2[i2];
		    powers[count++] = powers2[i2++];
		}
		else if (indexes1[i1] < indexes2[i2]) {
		    indexes[count] = indexes1[i1];
		    powers[count++] = powers1[i1++];
		}
		else {
		    if (powers1[i1] != -powers2[i2]) {
			indexes[count] = indexes1[i1];
			powers[count++] = powers1[i1] + powers2[i2];
		    }

		    i1++;
		    i2++;
		}
	    }

	    result = (utUnit*)productNew(unit1->common.system, indexes,
		powers, count);
	}				/* "unit2" is a product-unit */
    }					/* valid arguments */

    return result;
}


/*
 * Returns the result of raising a product unit to a power.
 *
 * Arguments:
 *	unit	The product unit.
 *	power	The power.  Must be greater than -256 and less than 256.
 * Returns:
 *	NULL	Failure.  "unitStatus" is set:
 *		    UT_INTERNAL		"unit" is NULL.
 *		    UT_INTERNAL		"unit" is not a product-unit.
 *		    UT_INTERNAL		"power" is out-of-range
 *		    UT_MEANINGLESS	The operation on the given unit is
 *					meaningless.
 *	else	The resulting unit.
 */
static utUnit*
productRaise(
    utUnit* const	unit,
    const int		power)
{
    utUnit*		result = NULL;	/* failure */

    if (unit == NULL || !IS_PRODUCT(unit) || power < -255 || power > 255) {
	unitStatus = UT_INTERNAL;
    }
    else {
	const ProductUnit* const	product = &unit->product;

	if (power == 0) {
	    result = unit->common.system->one;
	}
	else if (power == 1) {
	    result = productClone(unit);
	}
	else {
	    int		count = product->count;
	    short*	newPowers = malloc(sizeof(short)*count);

	    if (newPowers == NULL) {
		unitStatus = UT_OS;
	    }
	    else {
		const short* const	oldPowers = product->powers;
		int			i;

		for (i = 0; i < count; i++)
		    newPowers[i] = (short)(oldPowers[i] * power);

		result = (utUnit*)productNew(unit->common.system,
		    product->indexes, newPowers, count);

		free(newPowers);
	    }				/* "newPowers" allocated */
	}				/* non-trivial power */
    }					/* valid arguments */

    return result;
}


/*
 * Initializes a converter of numeric values between the given product-unit and
 * the underlying product-unit (i.e., to itself).
 *
 * Arguments:
 *	converter	Pointer to pointer to the converter to be initialized.
 * Returns:
 *	-1	Failure.  "unitStatus" is set:
 *		    UT_INTERNAL	"unit" is NULL.
 *		    UT_INTERNAL	"unit" is not a product-unit.
 *	 0	Success.
 */
static int
productInitConverter(
    cvConverter** const	converter)
{
    int		retCode;

    if (converter == NULL) {
	unitStatus = UT_INTERNAL;
	retCode = -1;
    }
    else {
	*converter = cvGetTrivial();
	retCode = 0;
    }

    return retCode;
}


/*
 * Initializes the converter of numeric values from the given product-unit to
 * the underlying product-unit (i.e., to itself).
 *
 * Arguments:
 *	unit	The product unit.
 * Returns:
 *	-1	Failure.  "unitStatus" is set:
 *		    UT_INTERNAL	"unit" is NULL.
 *		    UT_INTERNAL	"unit" is not a product-unit.
 *	 0	Success.
 */
static int
productInitConverterToProduct(
    utUnit* const	unit)
{
    return productInitConverter(&unit->common.toProduct);
}


/*
 * Initializes the converter of numeric values to the given product-unit from
 * the underlying product-unit (i.e., to itself).
 *
 * Arguments:
 *	unit	The product unit.
 * Returns:
 *	-1	Failure.  "unitStatus" is set:
 *		    UT_INTERNAL	"unit" is NULL.
 *		    UT_INTERNAL	"unit" is not a product-unit.
 *	 0	Success.
 */
static int
productInitConverterFromProduct(
    utUnit* const	unit)
{
    return productInitConverter(&unit->common.fromProduct);
}


/*
 * Returns the relationship between two product-units.  In determining the
 * relationship, dimensionless basic-units are ignored.
 *
 * Arguments:
 *	unit1	The first product unit.
 *	unit2	The second product unit.
 * Returns:
 *	PRODUCT_EQUAL		The units are equal -- ignoring dimensionless
 *				basic-units.
 *	PRODUCT_INVERSE		The units are reciprocals of each other.
 *	PRODUCT_UNCONVERTIBLE	The dimensionalities of the units are
 *				unconvertible.
 *	PRODUCT_UNKNOWN		An error occurred.  "unitStatus" will be:
 *	    UT_INTERNAL		    "unit1" is NULL.
 *	    UT_INTERNAL		    "unit2" is NULL.
 */
static ProductRelationship
productRelationship(
    const ProductUnit* const	unit1,
    const ProductUnit* const	unit2)
{
    ProductRelationship		relationship = PRODUCT_UNKNOWN;

    if (unit1 == NULL || unit2 == NULL) {
	unitStatus = UT_INTERNAL;
    }
    else {
	const short* const	indexes1 = unit1->indexes;
	const short* const	indexes2 = unit2->indexes;
	const short* const	powers1 = unit1->powers;
	const short* const	powers2 = unit2->powers;
	const int		count1 = unit1->count;
	const int		count2 = unit2->count;
	const utSystem* const	system = unit1->common.system;
	int			i1 = 0;
	int			i2 = 0;

	while (i1 < count1 || i2 < count2) {
	    int	iBasic = -1;

	    if (i1 >= count1) {
		iBasic = i2++;
	    }
	    else if (i2 >= count2) {
		iBasic = i1++;
	    }
	    else if (indexes1[i1] > indexes2[i2]) {
		iBasic = i2++;
	    }
	    else if (indexes1[i1] < indexes2[i2]) {
		iBasic = i1++;
	    }

	    if (iBasic != -1) {
		if (!system->basicUnits[iBasic]->isDimensionless) {
		    relationship = PRODUCT_UNCONVERTIBLE;
		    break;
		}
	    }
	    else {
		if (!system->basicUnits[i1]->isDimensionless) {
		    if (powers1[i1] == powers2[i2]) {
			if (relationship == PRODUCT_INVERSE) {
			    relationship = PRODUCT_UNCONVERTIBLE;
			    break;
			}

			relationship = PRODUCT_EQUAL;
		    }
		    else if (powers1[i1] == -powers2[i2]) {
			if (relationship == PRODUCT_EQUAL) {
			    relationship = PRODUCT_UNCONVERTIBLE;
			    break;
			}

			relationship = PRODUCT_INVERSE;
		    }
		    else {
			relationship = PRODUCT_UNCONVERTIBLE;
		    }
		}

		i1++;
		i2++;
	    }
	}

	if (relationship == PRODUCT_UNKNOWN) {
	    /*
	     * Both units are dimensionless.
	     */
	    relationship = PRODUCT_EQUAL;
	}
    }				/* valid arguments */

    return relationship;
}


static utStatus
productAcceptVisitor(
    const utUnit* const		unit,
    const utVisitor* const	visitor,
    void* const			arg)
{
    const ProductUnit* const	prodUnit = &unit->product;
    const int			count = prodUnit->count;
    const BasicUnit*		basicUnits[count];
    int				powers[count];
    int				i;

    for (i = 0; i < count; ++i) {
	basicUnits[i] = unit->common.system->basicUnits[prodUnit->indexes[i]];
	powers[i] = prodUnit->powers[i];
    }

    return visitor->visitProduct(unit, count, (const utUnit**)basicUnits,
	powers, arg);
}


static UnitOps	productOps = {
    productGetProduct,
    productClone,
    productFree,
    productCompare,
    productMultiply,
    productRaise,
    productInitConverterToProduct,
    productInitConverterFromProduct,
    productAcceptVisitor
};


/*
 * Arguments:
 *	system	The unit-system for the new unit.
 *	indexes	Pointer to array of indexes of basic-units.  May be freed upon
 *		return.
 *	powers	Pointer to array of powers.  Client may free upon return.
 *	count	The number of elements in "indexes" and "powers".
 * Returns:
 *	NULL	Failure.  "unitStatus" is set:
 *		    UT_INTERNAL	"count > 0 &&
 *				(indexes == NULL || powers == NULL)".
 *		    UT_INTERNAL	"count" < 0
 *		    UT_INTERNAL	"unit->common.system" is NULL
 *		    UT_OS	Operating-system error.  See "errno".
 *	else	The newly-allocated, product-unit.
 */
static ProductUnit*
productNew(
    utSystem* const	system,
    const short* const	indexes,
    const short* const	powers,
    const int		count)
{
    ProductUnit*	productUnit;

    if (system == NULL || (count > 0 && (indexes == NULL || powers == NULL)) ||
	    count < 0) {
	unitStatus = UT_INTERNAL;
	productUnit = NULL;
    }
    else {
	productUnit = malloc(sizeof(ProductUnit));

	if (productUnit == NULL) {
	    unitStatus = UT_OS;
	}
	else {
	    int	error = 1;

	    if (commonInit(&productUnit->common, &productOps, system, PRODUCT)
		    == 0) {
		size_t	nbytes = sizeof(short)*count;
		short*	newIndexes = malloc(nbytes*2);

		if (count == 0 || newIndexes != NULL) {
		    short*	newPowers = newIndexes + count;

		    productUnit->indexes = memcpy(newIndexes, indexes, nbytes);
		    productUnit->powers = memcpy(newPowers, powers, nbytes);
		    productUnit->count = count;
		    error = 0;
		}
	    }

	    if (error) {
		free(productUnit);
		productUnit = NULL;
	    }
	}				/* "productUnit" allocated */
    }					/* valid arguments */

    return productUnit;
}


/*
 * Indicates if a product-unit is dimensionless or not.
 *
 * Arguments:
 *	unit	The product-unit in question.
 * Returns:
 *	0	"unit" is dimensionfull or an error occurred.  "unitStatus" will be
 *		    UT_INTERNAL	"unit" is NULL.
 *		    UT_INTERNAL	"unit" is not a product-unit.
 *	else	"unit" is dimensionless.
 */
static int
productIsDimensionless(
    const ProductUnit* const	unit)
{
    int		isDimensionless = 0;

    if (unit == NULL || !IS_PRODUCT(unit)) {
	unitStatus = UT_INTERNAL;
    }
    else {
	const int		count = unit->count;
	const utSystem* const	system = unit->common.system;
	int			i = 0;

	isDimensionless = 1;

	while (i < count) {
	    if (!system->basicUnits[i]->isDimensionless) {
		isDimensionless = 0;
		break;
	    }
	}
    }				/* valid arguments */

    return isDimensionless;
}


/******************************************************************************
 * Galilean Unit:
 ******************************************************************************/


static ProductUnit*
galileanGetProduct(
    const utUnit* const	unit)
{
    return GET_PRODUCT(unit->galilean.unit);
}


static utUnit*
galileanClone(
    const utUnit* const	unit)
{
    utUnit*		clone;

    if (unit == NULL) {
	clone = NULL;
	unitStatus = UT_INTERNAL;
    }
    else {
	const GalileanUnit* const	galileanUnit = &unit->galilean;

	clone = galileanNew(galileanUnit->scale, galileanUnit->unit,
	    galileanUnit->offset);
    }

    return clone;
}


static int
galileanCompare(
    const utUnit* const	unit1,
    const utUnit* const	unit2)
{
    const GalileanUnit* const	galilean1 = &unit1->galilean;
    const GalileanUnit* const	galilean2 = &unit2->galilean;
    int				result =
	galilean1->offset < galilean2->offset
	    ? -1
	    : galilean1->offset == galilean2->offset
		? 0
		: -1;

    if (result == 0) {
	result =
	    galilean1->scale < galilean2->scale
		? -1
		: galilean1->scale == galilean2->scale
		    ? 0
		    : -1;

	if (result == 0)
	    result = COMPARE(galilean1->unit, galilean2->unit);
    }

    return result;
}


static void
galileanFree(
    utUnit* const	unit)
{
    if (unit != NULL) {
	FREE(unit->galilean.unit);
	free((void*)unit);
    }
}


/*
 * Multiplies a Galilean-unit by another unit.  Any offset is ignored.
 *
 * Arguments:
 *	unit1	The Galilean-unit.
 *	unit2	The other unit.
 * Returns:
 *	NULL	Failure.  "unitStatus" is set:
 *		    UT_INTERNAL		"unit1" is NULL.
 *		    UT_INTERNAL		"unit2" is NULL.
 *		    UT_INTERNAL		"unit1" is not a Galilean-unit.
 *		    UT_MEANINGLESS	The operation on the given units is
 *					meaningless.
 *	else	The resulting unit.
 */
static utUnit*
galileanMultiply(
    utUnit* const	unit1,
    utUnit* const	unit2)
{
    utUnit*	result = NULL;	/* failure */

    if (unit1 == NULL || !IS_GALILEAN(unit1) || unit2 == NULL) {
	unitStatus = UT_INTERNAL;
    }
    else {
	const GalileanUnit* const	galilean1 = &unit1->galilean;

	if (IS_PRODUCT(unit2)) {
	    result = galileanNew(galilean1->scale, 
		MULTIPLY(galilean1->unit, unit2), 0);
	}
	else if (IS_GALILEAN(unit2)) {
	    const GalileanUnit* const	galilean2 = &unit2->galilean;

	    result = galileanNew(galilean1->scale * galilean2->scale, 
		MULTIPLY(galilean1->unit, galilean2->unit), 0);
	}
	else {
	    result = MULTIPLY(unit2, unit1);
	}
    }					/* valid arguments */

    return result;
}


/*
 * Returns the result of raising a Galilean-unit to a power.  Any offset is
 * ignored.
 *
 * Arguments:
 *	unit	The Galilean-unit.
 *	power	The power.  Must be greater than -256 and less than 256.
 * Returns:
 *	NULL	Failure.  "unitStatus" is set:
 *		    UT_INTERNAL		"unit" is NULL.
 *		    UT_INTERNAL		"unit" is not a Galilean-unit.
 *		    UT_INTERNAL		"power" is out-of-range.
 *		    UT_MEANINGLESS	The operation on the given units is
 *					meaningless.
 *	else	The resulting unit.
 */
static utUnit*
galileanRaise(
    utUnit* const	unit,
    const int		power)
{
    utUnit*	result = NULL;	/* failure */

    if (unit == NULL || !IS_GALILEAN(unit) || power < -255 || power > 255) {
	unitStatus = UT_INTERNAL;
    }
    else {
	const GalileanUnit* const	galilean = &unit->galilean;

	if (power == 0) {
	    result = unit->common.system->one;
	}
	else {
	    result = galileanNew(pow(galilean->scale, power),
		RAISE(galilean->unit, power), 0);
	}
    }					/* valid arguments */

    return result;
}


/*
 * Initializes the converter of numeric values from the given Galilean unit to
 * the underlying product-unit.
 *
 * Arguments:
 *	unit	The Galilean unit.
 * Returns:
 *	-1	Failure.  "unitStatus" is set:
 *		    UT_INTERNAL		"unit" is NULL.
 *		    UT_INTERNAL		"unit" is not a Galilean-unit.
 *		    UT_OS		Operating-system fault.  See "errno".
 *	0	Success.
 */
static int
galileaninitConverterToProduct(
    utUnit* const	unit)
{
    int			retCode = -1;	/* failure */

    if (unit == NULL || !IS_GALILEAN(unit)) {
	unitStatus = UT_INTERNAL;
    }
    else {
	cvConverter* const	toUnderlying = cvGetGalilean(
	    unit->galilean.scale, unit->galilean.offset * unit->galilean.scale);

	if (toUnderlying == NULL) {
	    unitStatus = UT_OS;
	}
	else {
	    if (ENSURE_CONVERTER_TO_PRODUCT(unit->galilean.unit)) {
		assert(unit->common.toProduct == NULL);

		unit->common.toProduct = cvCombine(
		    toUnderlying, unit->galilean.unit->common.toProduct);

		if (unit->common.toProduct == NULL) {
		    unitStatus = UT_OS;
		}
		else {
		    retCode = 0;
		}
	    }

	    cvFree(toUnderlying);
	}				/* "toUnderlying" allocated */
    }					/* valid arguments */

    return retCode;
}


/*
 * Initializes the converter of numeric values to the given Galilean unit from
 * the underlying product-unit.
 *
 * Arguments:
 *	unit	The Galilean unit.
 * Returns:
 *	-1	Failure.  "unitStatus" is set:
 *		    UT_INTERNAL		"unit" is NULL.
 *		    UT_INTERNAL		"unit" is not a Galilean-unit.
 *		    UT_OS		Operating-system fault.  See "errno".
 *	0	Success.
 */
static int
galileaninitConverterFromProduct(
    utUnit* const	unit)
{
    int		retCode = -1;		/* failure */

    if (unit == NULL || !IS_GALILEAN(unit)) {
	unitStatus = UT_INTERNAL;
    }
    else {
	cvConverter* const	fromUnderlying = cvGetGalilean(
	    1.0/unit->galilean.scale, -unit->galilean.offset);

	if (fromUnderlying == NULL) {
	    unitStatus = UT_OS;
	}
	else {
	    if (ENSURE_CONVERTER_FROM_PRODUCT(unit->galilean.unit)) {
		assert(unit->common.fromProduct == NULL);

		unit->common.fromProduct = cvCombine(
		    unit->galilean.unit->common.fromProduct, fromUnderlying);

		if (unit->common.fromProduct == NULL) {
		    unitStatus = UT_OS;
		}
		else {
		    retCode = 0;
		}
	    }

	    cvFree(fromUnderlying);
	}				/* "fromUnderlying" allocated */
    }					/* valid arguments */

    return retCode;
}


static utStatus
galileanAcceptVisitor(
    const utUnit* const		unit,
    const utVisitor* const	visitor,
    void* const			arg)
{
    return visitor->visitGalilean(unit, unit->galilean.scale,
	unit->galilean.unit, unit->galilean.offset, arg);
}


static UnitOps	galileanOps = {
    galileanGetProduct,
    galileanClone,
    galileanFree,
    galileanCompare,
    galileanMultiply,
    galileanRaise,
    galileaninitConverterToProduct,
    galileaninitConverterFromProduct,
    galileanAcceptVisitor
};


/*
 * Returns a new unit instance.  The returned instance is not necessarily a
 * Galilean unit.
 *
 * Arguments:
 *	scale	The scale-factor for the new unit.
 *	unit	The underlying unit.  May be freed upon return.
 *	offset	The offset for the new unit.
 * Returns:
 *	NULL	Failure.  "unitStatus" is set:
 *		    UT_INTERNAL	"scale == 0"
 *		    UT_INTERNAL	"unit" is NULL
 *		    UT_OS	Operating-system error.  See "errno".
 *	else	The newly-allocated, galilean-unit.
 */
static utUnit*
galileanNew(
    double	scale,
    utUnit*	unit,
    double	offset)
{
    utUnit*	newUnit = NULL;	/* failure */

    if (scale == 0 || unit == NULL) {
	unitStatus = UT_INTERNAL;
    }
    else {
	if (IS_GALILEAN(unit)) {
	    scale *= unit->galilean.scale;
	    offset += (unit->galilean.scale * unit->galilean.offset) / scale;
	    unit = unit->galilean.unit;
	}

	if (scale == 1 && offset == 0) {
	    newUnit = CLONE(unit);
	}
	else {
	    GalileanUnit*	galileanUnit = malloc(sizeof(GalileanUnit));

	    if (galileanUnit == NULL) {
		unitStatus = UT_OS;
	    }
	    else {
		int	error = 1;

		if (commonInit(&galileanUnit->common, &galileanOps,
			unit->common.system, GALILEAN) == 0) {
		    galileanUnit->scale = scale;
		    galileanUnit->offset = offset;
		    galileanUnit->unit = CLONE(unit);
		    error = 0;
		}

		if (error) {
		    free(galileanUnit);
		    galileanUnit = NULL;
		}
	    }				/* "galileanUnit" allocated */

	    newUnit = (utUnit*)galileanUnit;
	}				/* Galilean unit necessary */
    }					/* valid arguments */

    return newUnit;
}


/******************************************************************************
 * Timestamp Unit:
 ******************************************************************************/


static ProductUnit*
timestampGetProduct(
    const utUnit* const	unit)
{
    return GET_PRODUCT(unit->timestamp.unit);
}


static utUnit*
timestampClone(
    const utUnit* const	unit)
{
    utUnit*		clone;

    if (unit == NULL) {
	clone = NULL;
	unitStatus = UT_INTERNAL;
    }
    else {
	clone =
	    timestampNewOrigin(unit->timestamp.unit, unit->timestamp.origin);
    }

    return clone;
}


static int
timestampCompare(
    const utUnit* const	unit1,
    const utUnit* const	unit2)
{
    const TimestampUnit* const	timestamp1 = &unit1->timestamp;
    const TimestampUnit* const	timestamp2 = &unit2->timestamp;
    int				result =
	timestamp1->origin < timestamp2->origin
	    ? -1
	    : timestamp1->origin == timestamp2->origin
		? 0
		: -1;

    if (result == 0)
	result = COMPARE(timestamp1->unit, timestamp2->unit);

    return result;
}


static void
timestampFree(
    utUnit* const	unit)
{
    if (unit != NULL) {
	FREE(unit->timestamp.unit);
	free((void*)unit);
    }
}


/*
 * Multiplies a timestamp-unit by another unit.  The origin is ignored.
 *
 * Arguments:
 *	unit1	The timestamp-unit.
 *	unit2	The other unit.
 * Returns:
 *	NULL	Failure.  "unitStatus" is set:
 *		    UT_INTERNAL		"unit1" is NULL.
 *		    UT_INTERNAL		"unit2" is NULL.
 *		    UT_INTERNAL		"unit1" is not a timestamp-unit.
 *		    UT_MEANINGLESS	The operation on the given units is
 *					meaningless.
 *	else	The resulting unit.
 */
static utUnit*
timestampMultiply(
    utUnit* const	unit1,
    utUnit* const	unit2)
{
    utUnit*	result = NULL;	/* failure */

    if (unit1 == NULL || !IS_TIMESTAMP(unit1) || unit2 == NULL) {
	unitStatus = UT_INTERNAL;
    }
    else {
	result = MULTIPLY(unit1->timestamp.unit, unit2);
    }					/* valid arguments */

    return result;
}


/*
 * Returns the result of raising a Timestamp-unit to a power.  The origin is
 * ignored.
 *
 * Arguments:
 *	unit	The Timestamp-unit.
 *	power	The power.  Must be greater than -256 and less than 256.
 * Returns:
 *	NULL	Failure.  "unitStatus" is set:
 *		    UT_INTERNAL		"unit" is NULL.
 *		    UT_INTERNAL		"unit" is not a Timestamp-unit.
 *		    UT_INTERNAL		"power" is out-of-range.
 *		    UT_MEANINGLESS	The operation on the given units is
 *					meaningless.
 *	else	The resulting unit.
 */
static utUnit*
timestampRaise(
    utUnit* const	unit,
    const int		power)
{
    utUnit*	result = NULL;	/* failure */

    if (unit == NULL || !IS_TIMESTAMP(unit) || power < -255 || power > 255) {
	unitStatus = UT_INTERNAL;
    }
    else {
	result = RAISE(unit->timestamp.unit, power);
    }					/* valid arguments */

    return result;
}


/*
 * Initializes the converter of numeric values from the given Timestamp unit to
 * the underlying product-unit.
 *
 * Arguments:
 *	unit	The Timestamp unit.
 * Returns:
 *	-1	Failure.  "unitStatus" is set:
 *		    UT_INTERNAL		"unit" is NULL.
 *		    UT_INTERNAL		"unit" is not a Timestamp-unit.
 *		    UT_OS		Operating-system fault.  See "errno".
 *	0	Success.
 */
static int
timestampinitConverterToProduct(
    utUnit* const	unit)
{
    int			retCode = -1;	/* failure */

    if (unit == NULL || !IS_TIMESTAMP(unit)) {
	unitStatus = UT_INTERNAL;
    }
    else {
	cvConverter* const	toUnderlying =
	    cvGetGalilean(1.0, unit->timestamp.origin);

	if (toUnderlying == NULL) {
	    unitStatus = UT_OS;
	}
	else {
	    if (ENSURE_CONVERTER_TO_PRODUCT(unit->timestamp.unit)) {
		assert(unit->common.toProduct == NULL);

		unit->common.toProduct = cvCombine(
		    toUnderlying, unit->timestamp.unit->common.toProduct);

		if (unit->common.toProduct == NULL) {
		    unitStatus = UT_OS;
		}
		else {
		    retCode = 0;
		}
	    }

	    cvFree(toUnderlying);
	}				/* "toUnderlying" allocated */
    }					/* valid arguments */

    return retCode;
}


/*
 * Initializes the converter of numeric values to the given Timestamp unit from
 * the underlying product-unit.
 *
 * Arguments:
 *	unit	The Timestamp unit.
 * Returns:
 *	-1	Failure.  "unitStatus" is set:
 *		    UT_INTERNAL		"unit" is NULL.
 *		    UT_INTERNAL		"unit" is not a Timestamp-unit.
 *		    UT_OS		Operating-system fault.  See "errno".
 *	0	Success.
 */
static int
timestampinitConverterFromProduct(
    utUnit* const	unit)
{
    int		retCode = -1;		/* failure */

    if (unit == NULL || !IS_TIMESTAMP(unit)) {
	unitStatus = UT_INTERNAL;
    }
    else {
	cvConverter* const	fromUnderlying =
	    cvGetGalilean(1.0, -unit->timestamp.origin);

	if (fromUnderlying == NULL) {
	    unitStatus = UT_OS;
	}
	else {
	    if (ENSURE_CONVERTER_FROM_PRODUCT(unit->timestamp.unit)) {
		assert(unit->common.fromProduct == NULL);

		unit->common.fromProduct = cvCombine(
		    unit->timestamp.unit->common.fromProduct, fromUnderlying);

		if (unit->common.fromProduct == NULL) {
		    unitStatus = UT_OS;
		}
		else {
		    retCode = 0;
		}
	    }

	    cvFree(fromUnderlying);
	}				/* "fromUnderlying" allocated */
    }					/* valid arguments */

    return retCode;
}


static utStatus
timestampAcceptVisitor(
    const utUnit* const		unit,
    const utVisitor* const	visitor,
    void* const			arg)
{
    int		year, month, day, hour, minute;
    double	second, resolution;

    decodeTime(unit->timestamp.origin, &year, &month, &day, &hour, &minute,
	&second, &resolution);

    return visitor->visitTimestamp(unit, unit->timestamp.unit, year, month, day,
	hour, minute, second, resolution, arg);
}


static UnitOps	timestampOps = {
    timestampGetProduct,
    timestampClone,
    timestampFree,
    timestampCompare,
    timestampMultiply,
    timestampRaise,
    timestampinitConverterToProduct,
    timestampinitConverterFromProduct,
    timestampAcceptVisitor
};


/*
 * Returns a new unit instance.
 *
 * Arguments:
 *	unit	The underlying unit.  May be freed upon return.
 *	origin	The timestamp origin.
 * Returns:
 *	NULL	Failure.  "unitStatus" is set:
 *		    UT_INTERNAL		"unit" is NULL
 *		    UT_OS		Operating-system error.  See "errno".
 *		    UT_MEANINGLESS	Creation of a timestamp unit based on
 *					"unit" is not meaningful.
 *		    UT_NOSECOND		The associated unit-system doesn't
 *					contain a second unit.
 *	else	The newly-allocated, timestamp-unit.
 */
static utUnit*
timestampNewOrigin(
    utUnit*		unit,
    const double	origin)
{
    utUnit*	newUnit = NULL;	/* failure */

    if (unit == NULL) {
	unitStatus = UT_INTERNAL;
    }
    if (IS_TIMESTAMP(unit)) {
	unitStatus = UT_MEANINGLESS;
    }
    else {
	utUnit* const	secondUnit = unit->common.system->second;

	if (secondUnit == NULL) {
	    unitStatus = UT_NOSECOND;
	}
	else if (utAreConvertible(secondUnit, unit)) {
	    TimestampUnit*	timestampUnit = malloc(sizeof(TimestampUnit));

	    if (timestampUnit == NULL) {
		unitStatus = UT_OS;
	    }
	    else {
		if (commonInit(&timestampUnit->common, &timestampOps,
			unit->common.system, TIMESTAMP) == 0) {
		    timestampUnit->origin = origin;
		    timestampUnit->unit = CLONE(unit);
		}
		else {
		    free(timestampUnit);
		    timestampUnit = NULL;
		}
	    }			/* "timestampUnit" allocated */

	    newUnit = (utUnit*)timestampUnit;
	}				/* "secondUnit != NULL" && time unit */
    }					/* valid arguments */

    return newUnit;
}


/*
 * Returns a new unit instance.
 *
 * Arguments:
 *	unit	The underlying unit.  May be freed upon return.
 *	year	The year of the origin.
 *	month	The month of the origin.
 *	day	The day of the origin.
 *	hour	The hour of the origin.
 *	minute	The minute of the origin.
 *	second	The second of the origin.
 * Returns:
 *	NULL	Failure.  "unitStatus" is set:
 *		    UT_INTERNAL		"unit" is NULL
 *		    UT_OS		Operating-system error.  See "errno".
 *		    UT_MEANINGLESS	Creation of a timestamp unit base on
 *					"unit" is not meaningful.
 *		    UT_NOSECOND		The associated unit-system doesn't
 *					contain a unit named "second".
 *	else	The newly-allocated, timestamp-unit.
 */
static utUnit*
timestampNew(
    utUnit*	unit,
    const int	year,
    const int	month,
    const int	day,
    const int	hour,
    const int	minute,
    double	second)
{
    return timestampNewOrigin(
	unit, encodeTime(year, month, day, hour, minute, second));
}


/******************************************************************************
 * Logarithmic Unit:
 ******************************************************************************/


static ProductUnit*
logGetProduct(
    const utUnit* const	unit)
{
    return GET_PRODUCT(unit->log.reference);
}


static utUnit*
logClone(
    const utUnit* const	unit)
{
    utUnit*		clone;

    if (unit == NULL) {
	clone = NULL;
	unitStatus = UT_INTERNAL;
    }
    else {
	clone = logNew(unit->log.logE, unit->log.reference);
    }

    return clone;
}


static int
logCompare(
    const utUnit* const	unit1,
    const utUnit* const	unit2)
{
    const LogUnit* const	u1 = &unit1->log;
    const LogUnit* const	u2 = &unit2->log;
    int				result =
	utCompare(u1->reference, u2->reference);

    if (result == 0)
	result =
	    u1->logE < u2->logE
		? -1
		: u1->logE == u2->logE
		    ? 0
		    : 1;

    return result;
}


static void
logFree(
    utUnit* const	unit)
{
    if (unit != NULL) {
	FREE(unit->log.reference);
	free((void*)unit);
    }
}


/*
 * Multiplies a logarithmic-unit by another unit.
 *
 * Arguments:
 *	unit1	The logarithmic-unit.
 *	unit2	The other unit.
 * Returns:
 *	NULL	Failure.  "unitStatus" is set:
 *		    UT_INTERNAL		"unit1" is NULL.
 *		    UT_INTERNAL		"unit2" is NULL.
 *		    UT_INTERNAL		"unit1" is not a logarithmic-unit.
 *		    UT_MEANINGLESS	The operation on the given units is
 *					meaningless.
 *	else	The resulting unit.
 */
static utUnit*
logMultiply(
    utUnit* const	unit1,
    utUnit* const	unit2)
{
    utUnit*	result;

    if (unit1 == NULL || unit2 == NULL || !IS_LOG(unit1)) {
	result = NULL;
	unitStatus = UT_INTERNAL;
    }
    else {
	/*
	 * Because a logarithmic unit is dimensionless, the result is simply
	 * a scaling of the other unit.
	 */
	result = galileanNew(1.0/unit1->log.logE, unit2, 0.0);
    }

    return result;
}


/*
 * Returns the result of raising a logarithmic-unit to a power.
 *
 * Arguments:
 *	unit	The logarithmic-unit.
 *	power	The power.  Must be greater than -256 and less than 256.
 * Returns:
 *	NULL	Failure.  "unitStatus" is set:
 *		    UT_INTERNAL		"unit" is NULL.
 *		    UT_INTERNAL		"unit" is not a logarithmic-unit.
 *		    UT_INTERNAL		"power" is out-of-range.
 *		    UT_MEANINGLESS	The operation on the given units is
 *					meaningless.
 *	else	The resulting unit.
 */
static utUnit*
logRaise(
    utUnit* const	unit,
    const int		power)
{
    utUnit*	result;

    if (unit == NULL || !IS_LOG(unit) || power < -255 || power > 255) {
	result = NULL;
	unitStatus = UT_INTERNAL;
    }
    else {
	if (power == 0) {
	    result = unit->common.system->one;
	}
	else {
	    /*
	     * Because a logarithmic unit is dimensionless, the result is simply
	     * a scaling of the dimensionless-unit one.
	     */
	    result = galileanNew(pow(unit->log.logE, -power),
		unit->common.system->one, 0.0);
	}
    }

    return result;
}


/*
 * Initializes the converter of numeric values from the given logarithmic unit
 * to the underlying product-unit.
 *
 * Arguments:
 *	unit	The logarithmic unit.
 * Returns:
 *	-1	Failure.  "unitStatus" is set:
 *		    UT_INTERNAL		"unit" is NULL.
 *		    UT_INTERNAL		"unit" is not a logarithmic-unit.
 *		    UT_OS		Operating-system fault.  See "errno".
 *	0	Success.
 */
static int
logInitConverterToProduct(
    utUnit* const	unit)
{
    int			retCode = -1;		/* failure */

    if (unit == NULL || !IS_LOG(unit)) {
	unitStatus = UT_INTERNAL;
    }
    else {
	cvConverter* const	toUnderlying = cvGetExp(unit->log.logE);

	if (toUnderlying == NULL) {
	    unitStatus = UT_OS;
	}
	else {
	    if (ENSURE_CONVERTER_TO_PRODUCT(unit->log.reference)) {
		assert(unit->common.toProduct == NULL);

		unit->common.toProduct = cvCombine(
		    toUnderlying, unit->log.reference->common.toProduct);

		if (unit->common.toProduct == NULL) {
		    unitStatus = UT_OS;
		}
		else {
		    retCode = 0;
		}
	    }

	    cvFree(toUnderlying);
	}				/* "toUnderlying" allocated */
    }					/* valid arguments */

    return retCode;
}


/*
 * Initializes the converter of numeric values to the given logarithmic unit
 * from the underlying product-unit.
 *
 * Arguments:
 *	unit	The logarithmic unit.
 * Returns:
 *	-1	Failure.  "unitStatus" is set:
 *		    UT_INTERNAL		"unit" is NULL.
 *		    UT_INTERNAL		"unit" is not a logarithmic-unit.
 *		    UT_OS		Operating-system fault.  See "errno".
 *	0	Success.
 */
static int
logInitConverterFromProduct(
    utUnit* const	unit)
{
    int		retCode = -1;		/* failure */

    if (unit == NULL || !IS_LOG(unit)) {
	unitStatus = UT_INTERNAL;
    }
    else {
	cvConverter* const	fromUnderlying = cvGetLog(unit->log.logE);

	if (fromUnderlying == NULL) {
	    unitStatus = UT_OS;
	}
	else {
	    if (ENSURE_CONVERTER_FROM_PRODUCT(unit->log.reference)) {
		assert(unit->common.fromProduct == NULL);

		unit->common.fromProduct = cvCombine(
		    unit->log.reference->common.fromProduct, fromUnderlying);

		if (unit->common.fromProduct == NULL) {
		    unitStatus = UT_OS;
		}
		else {
		    retCode = 0;
		}
	    }

	    cvFree(fromUnderlying);
	}				/* "fromUnderlying" allocated */
    }					/* valid arguments */

    return retCode;
}


static utStatus
logAcceptVisitor(
    const utUnit* const		unit,
    const utVisitor* const	visitor,
    void* const			arg)
{
    return visitor->visitLogarithmic(unit, unit->log.logE, unit->log.reference,
	arg);
}


static UnitOps	logOps = {
    logGetProduct,
    logClone,
    logFree,
    logCompare,
    logMultiply,
    logRaise,
    logInitConverterToProduct,
    logInitConverterFromProduct,
    logAcceptVisitor
};


/*
 * Arguments:
 *	logE		The logarithm of "e" in the logarithmic base.
 *	reference	The reference value.
 * Returns:
 *	NULL	Failure.  "utGetStatus()" will be:
 *		    UT_INTERNAL	"logE" <= 0.
 *		    UT_INTERNAL	"reference" is NULL.
 *		    UT_INTERNAL	"unit->common.system" is NULL.
 *		    UT_OS	Operating-system error.  See "errno".
 *	else	The newly-allocated, logarithmic-unit.
 */
static utUnit*
logNew(
    const double	logE,
    utUnit* const	reference)
{
    LogUnit*	logUnit;

    if (logE <= 0 || reference == NULL) {
	unitStatus = UT_INTERNAL;
	logUnit = NULL;
    }
    else {
	logUnit = malloc(sizeof(LogUnit));

	if (logUnit == NULL) {
	    unitStatus = UT_OS;
	}
	else {
	    if (commonInit(&logUnit->common, &logOps, reference->common.system,
		    LOG) != 0) {
		free(logUnit);
	    }
	    else {
		logUnit->reference = CLONE(reference);

		if (logUnit->reference != NULL) {
		    logUnit->logE = logE;
		}
		else {
		    free(logUnit);
		    logUnit = NULL;
		}
	    }
	}
    }

    return (utUnit*)logUnit;
}


/******************************************************************************
 * Public API:
 ******************************************************************************/


/*
 * Returns the status of the last operation by this module.
 *
 * Returns one of:
 *	UT_SUCCESS		Success
 *	UT_BADSYSTEM		The unit-system pointer is NULL
 *	UT_BADID		A name or symbol is invalid
 *	UT_BADVALUE		A value is invalid
 *	UT_EXISTS		The unit or mapping already exists
 *	UT_BADUNIT		A unit pointer is NULL
 *	UT_NOUNIT		No such unit exists
 *	UT_MEANINGLESS		The requested operation on the given unit(s)
 *				is meaningless
 *	UT_NOSECOND		The relevant unit-system doesn't contain a unit
 *				named "second".
 *	UT_OS			Operating system error
 *	UT_NOSYMBOL		The unit-system doesn't contain a necessary
 *				symbol.
 *	UT_INTERNAL		Internal error (this should not occur)
 */
utStatus
utGetStatus()
{
    return unitStatus;
}


/*
 * Returns a new unit system.
 *
 * Returns:
 *	NULL	Failure.  "utGetStatus()" will be:
 *		    UT_OS	Operating-system error.  See "errno".
 *	else	Pointer to a new unit system.
 */
utSystem*
utNewSystem()
{
    utSystem*	system = malloc(sizeof(utSystem));

    unitStatus = UT_SUCCESS;

    if (system == NULL) {
	unitStatus = UT_OS;
    }
    else {
	system->second = NULL;
	system->basicUnits = NULL;
	system->basicCount = 0;
	system->size = 0;

	if ((system->one = (utUnit*)productNew(system, NULL, NULL, 0)) != NULL)
	    system->size++;

	if (unitStatus != UT_SUCCESS)
	    free(system);
    }

    return system;
}


/*
 * Returns the dimensionless-unit one of a unit-system.
 *
 * Arguments:
 *	system	Pointer to the unit-system for which the dimensionless-unit one
 *		will be returned.
 * Returns:
 *	NULL	Failure.  "utgetStatus()" will be:
 *		    UT_BADSYSTEM	"system" is NULL.
 *	else	Pointer to the dimensionless-unit one associated with "system".
 */
utUnit*
utGetDimensionlessUnitOne(
    utSystem* const	system)
{
    utUnit*	one;

    unitStatus = UT_SUCCESS;

    if (system == NULL) {
	one = NULL;
	unitStatus = UT_BADSYSTEM;
    }
    else {
	one = system->one;
    }

    return one;
}


/*
 * Returns the unit-system to which a unit belongs.
 *
 * Arguments:
 *	unit	Pointer to the unit in question.
 * Returns:
 *	NULL	Failure.  "utGetStatus()" will be
 *		    UT_BADUNIT	"unit" is NULL.
 *	else	Pointer to the unit-system to which "unit" belongs.
 */
utSystem*
utGetSystem(
    const utUnit* const	unit)
{
    utSystem*	system;

    unitStatus = UT_SUCCESS;

    if (unit == NULL) {
	system = NULL;
	unitStatus = UT_BADUNIT;
    }
    else {
	system = unit->common.system;
    }

    return system;
}


/*
 * Indicates if two units belong to the same unit-system.
 *
 * Arguments:
 *	unit1	Pointer to a unit.
 *	unit2	Pointer to another unit.
 * Returns:
 *	0	Failure or the units belong to different unit-systems.
 *		"utGetStatus()" will be
 *	    	    UT_BADUNIT	"unit1" or "unit2" is NULL.
 *	    	    UT_SUCCESS	The units belong to different unit-systems.
 *	else	The units belong to the same unit-system.
 */
int
utSameSystem(
    const utUnit* const	unit1,
    const utUnit* const	unit2)
{
    int	sameSystem;

    if (unit1 == NULL || unit2 == NULL) {
	unitStatus = UT_BADUNIT;
    }
    else {
	unitStatus = UT_SUCCESS;
	sameSystem = unit1->common.system == unit2->common.system;
    }

    return sameSystem;
}


/*
 * Adds a basic-unit to a unit-system.
 *
 * Arguments:
 *	system		The unit-system to which to add the new basic-unit.
 *	isDimensionless	Whether or not the basic-unit is dimensionless (e.g.,
 *			a radian).
 * Returns:
 *	NULL	Failure.  "utGetStatus()" will be
 *		    UT_BADSYSTEM	"system" is NULL.
 *		    UT_OS		Operating-system error.  See "errno".
 *	else	Pointer to the new base-unit.
 */
static BasicUnit*
newBasicUnit(
    utSystem* const	system,
    const int		isDimensionless)
{
    BasicUnit*	basicUnit = NULL;	/* failure */

    if (system == NULL) {
	unitStatus = UT_BADSYSTEM;
    }
    else {
	basicUnit = basicNew(system, isDimensionless);

	if (basicUnit != NULL) {
	    BasicUnit**	basicUnits = realloc(system->basicUnits,
		(system->basicCount+1)*sizeof(BasicUnit*));

	    if (basicUnits == NULL) {
		basicFree((utUnit*)basicUnit);

		basicUnit = NULL;
		unitStatus = UT_OS;
	    }
	    else {
		system->size++;
		system->basicUnits = basicUnits;
		basicUnits[system->basicCount++] = basicUnit;
	    }				/* "basicUnits" allocated */
	}				/* "basicUnit" allocated */
    }					/* valid arguments */

    return basicUnit;
}


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
    utSystem* const	system)
{
    return (utUnit*)newBasicUnit(system, 0);
}


/*
 * Adds a dimensionless-unit to a unit-system.  In the SI system of units,
 * the derived-unit radian is a dimensionless-unit.
 *
 * Arguments:
 *	system	Pointer to the unit-system to which to add the new
 *		dimensionless-unit.
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
    utSystem* const	system)
{
    return (utUnit*)newBasicUnit(system, 1);
}


/*
 * Returns the total number of units that have been added to a unit-system.
 *
 * Arguments:
 *	system	Pointer to the unit-system in question.
 * Returns:
 *	-1	"system" is NULL.  "utGetStatus()" will be UT_BADSYSTEM.
 *	else	The total number of units that have been added to "system".
 */
int
utSize(
    const utSystem* const	system)
{
    int	size;

    if (system == NULL) {
	size = -1;
	unitStatus = UT_BADSYSTEM;
    }
    else {
	size = system->size;
	unitStatus = UT_SUCCESS;
    }

    return size;
}


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
    utUnit* const	second)
{
    unitStatus = UT_SUCCESS;

    if (system == NULL) {
	unitStatus = UT_BADSYSTEM;
    }
    else if (second == NULL) {
	unitStatus = UT_BADUNIT;
    }
    else if (system != utGetSystem(second)) {
	unitStatus = UT_BADUNIT;
    }
    else if (system->second == NULL) {
	system->second = CLONE(second);
    }
    else {
	if (utCompare(system->second, second) != 0)
	    unitStatus = UT_EXISTS;
    }

    return unitStatus;
}


/*
 * Compares two units.  Returns a value less than, equal to, or greater than
 * zero as the first unit is considered less than, equal to, or greater than
 * the second unit, respectively.  Units from different unit-systems will never
 * compare equal.
 *
 * Arguments:
 *	unit1	Pointer to a unit or NULL.
 *	unit2	Pointer to another unit or NULL.
 * Returns:
 *	<0	The first unit is less than the second unit.
 *	 0	The first and second units are equal or both units are NULL.
 *	>0	The first unit is greater than the second unit.
 */
int
utCompare(
    const utUnit* const	unit1,
    const utUnit* const	unit2)
{
    int	cmp = 0;

    unitStatus = UT_SUCCESS;

    if (unit1 == NULL) {
	cmp = unit2 != NULL ? -1 : 0;
    }
    else if (unit2 == NULL) {
	cmp = 1;
    }
    else if (unit1->common.system < unit2->common.system) {
	cmp = -1;
    }
    else if (unit1->common.system > unit2->common.system) {
	cmp = 1;
    }
    else {
	const UnitOps* const	ops1 = unit1->common.ops;
	const UnitOps* const	ops2 = unit2->common.ops;

	/*
	 * NB: The unit-specific comparison function is called if and only
	 * if the units are the same type.
	 */
	cmp = 
	    ops1 < ops2
		? -1
		: ops1 == ops2
		    ? ops1->compare(unit1, unit2)
		    : 1;
    }

    return cmp;
}


/*
 * Returns a unit equivalent to another unit scaled by a numeric factor,
 * e.g.,
 *	const utUnit*	meter = ...
 *	const utUnit*	kilometer = utScale(1000, meter);
 *
 * Arguments:
 *	factor	The numeric scale factor.
 *	unit	Pointer to the unit to be scaled.
 * Returns:
 *	NULL	Failure.  "utGetStatus()" will be
 *		    UT_BADVALUE		"factor" is 0.
 *		    UT_BADUNIT		"unit" is NULL.
 *		    UT_OS		Operating-system error.  See "errno".
 *	else	Pointer to scaled-unit.
 */
utUnit*
utScale(
    const double	factor,
    utUnit* const	unit)
{
    utUnit*		result = NULL;	/* failure */

    unitStatus = UT_SUCCESS;

    if (unit == NULL) {
	unitStatus = UT_BADUNIT;
    }
    else {
	if (factor == 0) {
	    unitStatus = UT_BADVALUE;
	}
	else {
	    result = factor == 1
		? CLONE(unit)
		: galileanNew(factor, unit, 0.0);
	}
    }

    return result;
}


/*
 * Returns a unit equivalent to another unit offset by a numeric amount,
 * e.g.,
 *	const utUnit*	kelvin = ...
 *	const utUnit*	celsius = utOffset(kelvin, 273.15);
 *
 * Arguments:
 *	unit	Pointer to the unit to be offset.
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
    const double	offset)
{
    utUnit*		result = NULL;	/* failure */

    unitStatus = UT_SUCCESS;

    if (unit == NULL) {
	unitStatus = UT_BADUNIT;
    }
    else {
	result = offset == 0
	    ? CLONE(unit)
	    : galileanNew(1.0, unit, offset);
    }

    return result;
}


/*
 * Returns a unit equivalent to another unit relative to a particular time.
 * e.g.,
 *	const utUnit*	second = ...
 *	const utUnit*	secondsSinceTheEpoch =
 *	    utOffsetByTime(second, 1970, 1, 1, 0, 0, 0.0);
 *
 * "utSetSecond()" must be called before the first call to this function.
 *
 * Arguments:
 *	unit	Pointer to the time-unit to be made relative to a time-origin.
 *	year	The year of the origin.
 *	month	The month of the origin.
 *	day	The day of the origin.
 *	hour	The hour of the origin.
 *	minute	The minute of the origin.
 *	second	The second of the origin.
 * Returns:
 *	NULL	Failure.  "utGetStatus()" will be
 *		    UT_BADUNIT		"unit" is NULL.
 *		    UT_OS		Operating-system error.  See "errno".
 *		    UT_MEANINGLESS	Creation of a timestamp unit based on
 *					"unit" is not meaningful.
 *		    UT_NOSECOND		The associated unit-system doesn't
 *					contain a second unit.
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
    const double	second)
{
    utUnit*		result = NULL;	/* failure */

    unitStatus = UT_SUCCESS;

    if (unit == NULL) {
	unitStatus = UT_BADUNIT;
    }
    else {
	result = timestampNew(unit, year, month, day, hour, minute, second);
    }

    return result;
}


/*
 * Returns the result of multiplying one unit by another unit.
 *
 * Arguments:
 *	unit1	Pointer to a unit.
 *	unit2	Pointer to another unit.
 * Returns:
 *	NULL	Failure.  "utGetStatus()" will be:
 *		    UT_BADUNIT		"unit1" or "unit2" is NULL.
 *		    UT_NOTSAMESYSTEM	"unit1" and "unit2" belong to
 *					different unit-systems.
 *		    UT_OS		Operating-system error. See "errno".
 *	else	Pointer to unit corresponding to the product.
 */
utUnit*
utMultiply(
    utUnit* const	unit1,
    utUnit* const	unit2)
{
    utUnit*	result = NULL;	/* failure */

    unitStatus = UT_SUCCESS;

    if (unit1 == NULL || unit2 == NULL) {
	unitStatus = UT_BADUNIT;
    }
    else if (unit1->common.system != unit2->common.system) {
	unitStatus = UT_NOTSAMESYSTEM;
    }
    else {
	result = MULTIPLY(unit1, unit2);
    }

    return result;
}


/*
 * Returns the inverse (i.e., reciprocal) of a unit.  This convenience function
 * is equal to "utRaise(unit, -1)".
 *
 * Arguments:
 *	unit	Pointer to the unit.
 * Returns:
 *	NULL	Failure.  "utGetStatus()" will be:
 *		    UT_BADUNIT	"unit" is NULL.
 *		    UT_OS	Operating-system error. See "errno".
 *	else	Pointer to unit corresponding to the inverse.
 */
utUnit*
utInvert(
    utUnit* const	unit)
{
    return utRaise(unit, -1);
}


/*
 * Returns the result of dividing one unit by another unit.  This convenience
 * function is equivalent to the following code:
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
 *		    UT_BADUNIT		"numer" or "denom" is NULL.
 *		    UT_NOTSAMESYSTEM	"unit1" and "unit2" belong to
 *					different unit-systems.
 *		    UT_OS		Operating-system error. See "errno".
 *	else	Pointer to unit corresponding to the product.
 */
utUnit*
utDivide(
    utUnit* const	numer,
    utUnit* const	denom)
{
    utUnit*	result = NULL;		/* failure */

    unitStatus = UT_SUCCESS;

    if (numer == NULL || denom == NULL) {
	unitStatus = UT_BADUNIT;
    }
    else if (numer->common.system != denom->common.system) {
	unitStatus = UT_NOTSAMESYSTEM;
    }
    else {
	utUnit*	inverse = RAISE(denom, -1);

	if (inverse != NULL) {
	    result = MULTIPLY(numer, inverse);

	    utFree(inverse);
	}
    }

    return result;
}


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
 *	else	Pointer to unit corresponding to "unit" raised to "power".
 */
utUnit*
utRaise(
    utUnit* const	unit,
    const int		power)
{
    utUnit*		result = NULL;	/* failure */

    unitStatus = UT_SUCCESS;

    if (unit == NULL) {
	unitStatus = UT_BADUNIT;
    }
    else if (power < -255 || power > 255) {
	unitStatus = UT_BADVALUE;
    }
    else {
	result = 
	    power == 0
		? unit->common.system->one
		: power == 1
		    ? CLONE(unit)
		    : RAISE(unit, power);
    }

    return result;
}


/*
 * Returns the logarithmic unit corresponding to a logarithmic base and a
 * reference level.  For example, the following creates a decibel unit with a
 * one milliwatt reference level:
 *
 *     #include <math.h>		// for M_LOG10E
 *     ...
 *     const utUnit* watt = ...;
 *     const utUnit* milliWatt = utScale(0.001, watt);
 *
 *     if (milliWatt != NULL) {
 *         const utUnit* bel_1_mW = utLog(M_LOG10E, milliWatt);
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
 *	logE		The logarithm of "e" in the logarithmic base.  Must
 *			be positive.
 *	reference	Pointer to the reference value as a unit.
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
    utUnit* const	reference)
{
    utUnit*		result = NULL;	/* failure */

    unitStatus = UT_SUCCESS;

    if (logE <= 0) {
	unitStatus = UT_BADVALUE;
    }
    else if (reference == NULL) {
	unitStatus = UT_BADUNIT;
    }
    else {
	result = logNew(logE, reference);
    }

    return result;
}


/*
 * Indicates if numeric values in one unit are convertible to numeric values in
 * another unit via "utGetConverter()".  In making this determination, units
 * created by "utNewDimensionlessUnit()" are ignored.
 *
 * Arguments:
 *	unit1	Pointer to a unit.
 *	unit2	Pointer to another unit.
 * Returns:
 *	0	Numeric values cannot be converted between the units.  
 *		"utGetStatus()" will be
 *	    	    UT_BADUNIT		"unit1" or "unit2" is NULL.
 *		    UT_NOTSAMESYSTEM	"unit1" and "unit2" belong to different
 *					unit-sytems.
 *		    UT_SUCCESS		Conversion between the units is not
 *					possible (e.g., "unit1" is "meter"
 *					and "unit2" is "kilogram").
 *	else	Numeric values can be converted between the units.
 */
int
utAreConvertible(
    utUnit* const	unit1,
    utUnit* const	unit2)
{
    int			areConvertible = 0;

    if (unit1 == NULL || unit2 == NULL) {
	unitStatus = UT_BADUNIT;
    }
    else if (unit1->common.system != unit2->common.system) {
	unitStatus = UT_NOTSAMESYSTEM;
    }
    else {
	unitStatus = UT_SUCCESS;

	if (IS_TIMESTAMP(unit1) || IS_TIMESTAMP(unit2)) {
	    areConvertible = IS_TIMESTAMP(unit1) && IS_TIMESTAMP(unit2);
	}
	else {
	    ProductRelationship	relationship =
		productRelationship(GET_PRODUCT(unit1), GET_PRODUCT(unit2));

	    areConvertible = relationship == PRODUCT_EQUAL ||
		relationship == PRODUCT_INVERSE;
	}
    }

    return areConvertible;
}


/*
 * Returns a converter of numeric values in one unit to numeric values in
 * another unit.  The returned converter should be passed to cvFree() when it is
 * no longer needed.
 *
 * Arguments:
 *	from	Pointer to the unit from which to convert values.
 *	to	Pointer to the unit to which to convert values.
 * Returns:
 *	NULL	Failure.  "utGetStatus()" will be:
 *		    UT_BADUNIT		"from" or "to" is NULL.
 *		    UT_NOTSAMESYSTEM	"from" and "to" belong to different
 *					unit-systems.
 *		    UT_MEANINGLESS	Conversion between the units is not
 *					possible.  See "utAreConvertible()".
 *	else	Pointer to the appropriate converter.
 */
cvConverter*
utGetConverter(
    utUnit* const	from,
    utUnit* const	to)
{
    cvConverter*	converter = NULL;	/* failure */

    if (from == NULL || to == NULL) {
	unitStatus = UT_BADUNIT;
    }
    else if (from->common.system != to->common.system) {
	unitStatus = UT_NOTSAMESYSTEM;
    }
    else {
	unitStatus = UT_SUCCESS;

	if (!IS_TIMESTAMP(from) && !IS_TIMESTAMP(to)) {
	    ProductRelationship	relationship =
		productRelationship(GET_PRODUCT(from), GET_PRODUCT(to));

	    if (relationship == PRODUCT_UNCONVERTIBLE) {
		unitStatus = UT_MEANINGLESS;
	    }
	    else if (ENSURE_CONVERTER_TO_PRODUCT(from) &&
			ENSURE_CONVERTER_FROM_PRODUCT(to)) {
		if (relationship == PRODUCT_EQUAL) {
		    converter = cvCombine(
			from->common.toProduct, to->common.fromProduct);
		}
		else {
		    /*
		     * The underlying product-units are reciprocals of each
		     * other.
		     */
		    cvConverter*	invert = cvGetInverse();

		    if (invert != NULL) {
			cvConverter*	phase1 =
			    cvCombine(from->common.toProduct, invert);

			if (phase1 != NULL) {
			    converter =
				cvCombine(phase1, to->common.fromProduct);

			    cvFree(phase1);
			}		/* "phase1" allocated */

			cvFree(invert);
		    }			/* "invert" allocated */
		}			/* reciprocal product-units */

		if (converter == NULL)
		    unitStatus = UT_OS;
	    }				/* got necessary product converters */
	}				/* neither unit is a timestamp */
	else {
	    cvConverter*	toSeconds = utGetConverter(from->timestamp.unit,
		from->common.system->second);

	    if (toSeconds == NULL) {
		unitStatus = UT_OS;
	    }
	    else {
		cvConverter*	shiftOrigin =
		    cvGetOffset(from->timestamp.origin - to->timestamp.origin);

		if (shiftOrigin == NULL) {
		    unitStatus = UT_OS;
		}
		else {
		    cvConverter*	toToUnit =
			cvCombine(toSeconds, shiftOrigin);

		    if (toToUnit == NULL) {
			unitStatus = UT_OS;
		    }
		    else {
			cvConverter*	fromSeconds = utGetConverter(
			    to->common.system->second, to->timestamp.unit); 

			if (fromSeconds == NULL) {
			    unitStatus = UT_OS;
			}
			else {
			    converter = cvCombine(toToUnit, fromSeconds);

			    if (converter == NULL)
				unitStatus = UT_OS;

			    cvFree(fromSeconds);
			}		/* "fromSeconds" allocated */

			cvFree(toToUnit);
		    }			/* "toToUnit" allocated */

		    cvFree(shiftOrigin);
		}			/* "shiftOrigin" allocated */

		cvFree(toSeconds);
	    }				/* "toSeconds" allocated */
	}				/* units are timestamps */
    }					/* valid arguments */

    return converter;
}


/*
 * Indicates if a given unit is dimensionless or not.  Note that logarithmic
 * units are dimensionless by definition.
 *
 * Arguments:
 *	unit	Pointer to the unit in question.
 * Returns:
 *	0	"unit" is dimensionfull or an error occurred.  "utGetStatus()"
 *		 will be
 *		    UT_BADUNIT	"unit" is NULL.
 *		    UT_SUCCESS	"unit" is dimensionfull.
 *	else	"unit" is dimensionless.
 */
int
utIsDimensionless(
    utUnit* const	unit)
{
    unitStatus = UT_SUCCESS;

    return productIsDimensionless(GET_PRODUCT(unit));
}


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
    const utUnit* const	unit)
{
    return CLONE(unit);
}


/*
 * Frees resources associated with a unit, when appropriate.  This function
 * should be invoked when a unit is no longer needed.  Use of the unit upon
 * return from this function may result in undefined behavior.
 *
 * Arguments:
 *	unit	Pointer to the unit to have its resources freed or NULL.
 */
void
utFree(
    const utUnit* const unit)
{
    unitStatus = UT_SUCCESS;

    if (unit != NULL)
	FREE((utUnit*)unit);
}


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
    void* const			arg)
{
    unitStatus = UT_SUCCESS;

    if (unit == NULL) {
	unitStatus = UT_BADUNIT;
    }
    else if (visitor == NULL) {
	unitStatus = UT_BADVISITOR;
    }
    else {
	unitStatus = ACCEPT_VISITOR(unit, visitor, arg);
    }

    return unitStatus;
}
