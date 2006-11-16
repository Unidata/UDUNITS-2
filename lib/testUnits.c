#ifndef	_XOPEN_SOURCE
#   define _XOPEN_SOURCE 500
#endif


#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <units.h>


static utSystem*	unitSystem;
static utUnit*		kilogram;
static utUnit*		meter;
static utUnit*		radian;
static utUnit*		kelvin;
static utUnit*		second;
static utUnit*		minute;
static utUnit*		kilometer;
static utUnit*		rankine;
static utUnit*		celsius;
static utUnit*		fahrenheit;
static utUnit*		watt;
static utUnit*		cubicMicron;
static utUnit*		dBZ;
static utUnit*		secondsSinceTheEpoch;
static utUnit*		hertz;
static utUnit*		megahertz;

static unsigned		asciiName = UT_ASCII | UT_NAMES;
static unsigned		asciiNameDef = UT_ASCII | UT_NAMES | UT_DEFINITION;
static unsigned		asciiSymbolDef = UT_ASCII | UT_DEFINITION;
static unsigned		latin1SymbolDef = UT_LATIN1 | UT_DEFINITION;
static unsigned		utf8SymbolDef = UT_UTF8 | UT_DEFINITION;


static int
setup(
    void)
{
    unitSystem = utNewSystem();

    return unitSystem == NULL ? -1 : 0;
}


static int
teardown(
    void)
{
    return 0;
}


static void
test_utNewBaseUnit(void)
{
    int		size = utSize(unitSystem);

    kilogram = utNewBaseUnit(unitSystem);
    CU_ASSERT_PTR_NOT_NULL(kilogram);
    CU_ASSERT_EQUAL(utSize(unitSystem), size+1);
    CU_ASSERT_EQUAL(utMapUnitToName(kilogram, "kilogram", UT_ASCII),
	UT_SUCCESS);
    CU_ASSERT_EQUAL(utMapUnitToSymbol(kilogram, "kg", UT_ASCII), UT_SUCCESS);
    CU_ASSERT_EQUAL(utMapSymbolToUnit("kg", kilogram), UT_SUCCESS);

    size = utSize(unitSystem);
    meter = utNewBaseUnit(unitSystem);
    CU_ASSERT_PTR_NOT_NULL(meter);
    CU_ASSERT_EQUAL(utSize(unitSystem), size+1);
    CU_ASSERT_EQUAL(utMapNameToUnit("meter", meter), UT_SUCCESS);
    CU_ASSERT_EQUAL(utMapUnitToName(meter, "meter", UT_ASCII), UT_SUCCESS);
    CU_ASSERT_EQUAL(utMapUnitToSymbol(meter, "m", UT_ASCII), UT_SUCCESS);
    CU_ASSERT_EQUAL(utMapSymbolToUnit("m", meter), UT_SUCCESS);

    kelvin = utNewBaseUnit(unitSystem);
    CU_ASSERT_PTR_NOT_NULL(kelvin);
    CU_ASSERT_EQUAL(utMapUnitToName(kelvin, "kelvin", UT_ASCII), UT_SUCCESS);
    CU_ASSERT_EQUAL(utMapNameToUnit("kelvin", kelvin), UT_SUCCESS);
    CU_ASSERT_EQUAL(utMapUnitToSymbol(kelvin, "K", UT_ASCII), UT_SUCCESS);
    CU_ASSERT_EQUAL(utMapSymbolToUnit("K", kelvin), UT_SUCCESS);

    CU_ASSERT_EQUAL(utMapUnitToName(kilogram, "dummy", UT_ASCII), UT_EXISTS);

    second = utNewBaseUnit(unitSystem);
    CU_ASSERT_PTR_NOT_NULL(second);
    CU_ASSERT_EQUAL(utSetSecond(unitSystem, second), UT_SUCCESS);
    CU_ASSERT_EQUAL(utMapUnitToName(second, "second", UT_ASCII), UT_SUCCESS);
    CU_ASSERT_EQUAL(utMapNameToUnit("second", second), UT_SUCCESS);
    CU_ASSERT_EQUAL(utMapUnitToSymbol(second, "s", UT_ASCII), UT_SUCCESS);
    CU_ASSERT_EQUAL(utMapSymbolToUnit("s", second), UT_SUCCESS);

    CU_ASSERT_PTR_NULL(utNewBaseUnit(NULL));
    CU_ASSERT_EQUAL(utGetStatus(), UT_BADSYSTEM);

    CU_ASSERT_EQUAL(utMapUnitToName(kilogram, "Ångström", UT_UTF8), UT_BADID);

    CU_ASSERT_EQUAL(utSetSecond(unitSystem, second), UT_SUCCESS);
    CU_ASSERT_EQUAL(utSetSecond(unitSystem, meter), UT_EXISTS);
    CU_ASSERT_EQUAL(utSetSecond(unitSystem, NULL), UT_BADUNIT);
    CU_ASSERT_EQUAL(utSetSecond(NULL, second), UT_BADSYSTEM);
}


static void
test_utNewDimensionlessUnit(void)
{
    int			size = utSize(unitSystem);

    radian = utNewDimensionlessUnit(unitSystem);
    CU_ASSERT_PTR_NOT_NULL(radian);
    CU_ASSERT_EQUAL(utMapUnitToName(radian, "radian", UT_ASCII), UT_SUCCESS);
    CU_ASSERT_EQUAL(utMapNameToUnit("radian", radian), UT_SUCCESS);
    CU_ASSERT_EQUAL(utSize(unitSystem), size+1);
    CU_ASSERT_EQUAL(utMapUnitToSymbol(radian, "rad", UT_ASCII), UT_SUCCESS);
    CU_ASSERT_EQUAL(utMapSymbolToUnit("rad", radian), UT_SUCCESS);

    CU_ASSERT_EQUAL(utMapUnitToName(radian, "dummy", UT_ASCII), UT_EXISTS);

    CU_ASSERT_EQUAL(utMapUnitToSymbol(radian, "f", UT_ASCII), UT_EXISTS);
    CU_ASSERT_EQUAL(utMapUnitToSymbol(NULL, "f", UT_ASCII), UT_BADUNIT);

    CU_ASSERT_EQUAL(utMapUnitToName(radian, "Ångström", UT_UTF8), UT_BADID);
}


static void
test_utSize(void)
{
    CU_ASSERT(utSize(unitSystem) >= 0);
    CU_ASSERT(utSize(NULL) == -1);
}


static void
test_utGetUnitByName(void)
{
    CU_ASSERT_PTR_EQUAL(utGetUnitByName(unitSystem, "meter"), meter);

    CU_ASSERT_PTR_NULL(utGetUnitByName(unitSystem, NULL));
    CU_ASSERT_EQUAL(utGetStatus(), UT_BADID);

    CU_ASSERT_PTR_NULL(utGetUnitByName(unitSystem, "foo"));
    CU_ASSERT_EQUAL(utGetStatus(), UT_SUCCESS);
}


static void
test_utGetUnitBySymbol(void)
{
    CU_ASSERT_PTR_EQUAL(utGetUnitBySymbol(unitSystem, "m"), meter);

    CU_ASSERT_PTR_NULL(utGetUnitBySymbol(unitSystem, NULL));
    CU_ASSERT_EQUAL(utGetStatus(), UT_BADID);

    CU_ASSERT_PTR_NULL(utGetUnitBySymbol(unitSystem, "M"));
    CU_ASSERT_EQUAL(utGetStatus(), UT_SUCCESS);
}


static void
test_utAddNamePrefix(void)
{
    CU_ASSERT_EQUAL(utAddNamePrefix(unitSystem, "mega", 1e6), UT_SUCCESS);
    CU_ASSERT_EQUAL(utAddNamePrefix(unitSystem, "mega", 1e6), UT_SUCCESS);
    CU_ASSERT_EQUAL(utAddNamePrefix(unitSystem, "MEGA", 1e6), UT_SUCCESS);

    CU_ASSERT_EQUAL(utAddNamePrefix(unitSystem, "mega", 1e5), UT_EXISTS);
    CU_ASSERT_EQUAL(utAddNamePrefix(unitSystem, "MEGA", 1e5), UT_EXISTS);
    CU_ASSERT_EQUAL(utAddNamePrefix(NULL, "foo", 1), UT_BADSYSTEM);
    CU_ASSERT_EQUAL(utAddNamePrefix(unitSystem, "", 2), UT_BADID);
    CU_ASSERT_EQUAL(utAddNamePrefix(unitSystem, NULL, 3), UT_BADID);
    CU_ASSERT_EQUAL(utAddNamePrefix(unitSystem, "foo", 0), UT_BADVALUE);
}


static void
test_utAddSymbolPrefix(void)
{
    CU_ASSERT_EQUAL(utAddSymbolPrefix(unitSystem, "M", 1e6), UT_SUCCESS);
    CU_ASSERT_EQUAL(utAddSymbolPrefix(unitSystem, "M", 1e6), UT_SUCCESS);
    CU_ASSERT_EQUAL(utAddSymbolPrefix(unitSystem, "u", 1e-6), UT_SUCCESS);
    CU_ASSERT_EQUAL(utAddSymbolPrefix(unitSystem, "µ", 1e-6), UT_SUCCESS);

    CU_ASSERT_EQUAL(utAddSymbolPrefix(unitSystem, "M", 1e5), UT_EXISTS);
    CU_ASSERT_EQUAL(utAddSymbolPrefix(NULL, "foo", 1), UT_BADSYSTEM);
    CU_ASSERT_EQUAL(utAddSymbolPrefix(unitSystem, "", 2), UT_BADID);
    CU_ASSERT_EQUAL(utAddSymbolPrefix(unitSystem, NULL, 3), UT_BADID);
    CU_ASSERT_EQUAL(utAddSymbolPrefix(unitSystem, "f", 0), UT_BADVALUE);
}


static void
test_utMapNameToUnit(void)
{
    utUnit*	metre;

    CU_ASSERT_PTR_NULL(utGetUnitByName(unitSystem, "metre"));

    CU_ASSERT_EQUAL(utMapNameToUnit("metre", meter), UT_SUCCESS);
    CU_ASSERT_EQUAL(utMapNameToUnit("metre", meter), UT_SUCCESS);

    CU_ASSERT_EQUAL(utMapNameToUnit("metre", second), UT_EXISTS);
    CU_ASSERT_EQUAL(utMapNameToUnit("metre", NULL), UT_BADUNIT);

    metre = utGetUnitByName(unitSystem, "metre");
    CU_ASSERT_PTR_NOT_NULL(metre);
    CU_ASSERT_EQUAL(utCompare(metre, meter), 0);
    CU_ASSERT_EQUAL(metre, meter);	/* same unit */
    utFree(metre);
}


static void
test_utToString(void)
{
    char	buf[80];
    int		nchar = utFormat(meter, buf, sizeof(buf)-1, asciiSymbolDef);
    int		n;

    CU_ASSERT_EQUAL(nchar, 1);
    CU_ASSERT_STRING_EQUAL(buf, "m");

    nchar = utFormat(meter, buf, sizeof(buf)-1, asciiName);
    CU_ASSERT_STRING_EQUAL(buf, "meter");

    n = utFormat(celsius, buf, sizeof(buf)-1, asciiName);
    CU_ASSERT_TRUE(n > 0);
    CU_ASSERT_STRING_EQUAL(buf, "degrees_celsius");
}


static void
test_utScale(void)
{
    utUnit*	metre;
    char	buf[80];
    int		nchar;

    kilometer = utScale(1000, meter);
    CU_ASSERT_EQUAL(utGetStatus(), UT_SUCCESS);
    CU_ASSERT_PTR_NOT_NULL(kilometer);
    CU_ASSERT_EQUAL(utGetSystem(meter), utGetSystem(kilometer));
    CU_ASSERT_NOT_EQUAL(utCompare(meter, kilometer), 0);

    nchar = utFormat(kilometer, buf, sizeof(buf)-1, asciiSymbolDef);
    CU_ASSERT_TRUE_FATAL(nchar > 0);
    CU_ASSERT_TRUE_FATAL(nchar < sizeof(buf));
    buf[nchar] = 0;
    CU_ASSERT_STRING_EQUAL(buf, "1000 m");

    nchar = utFormat(kilometer, buf, sizeof(buf)-1, asciiNameDef);
    CU_ASSERT_TRUE_FATAL(nchar > 0);
    CU_ASSERT_TRUE_FATAL(nchar < sizeof(buf));
    buf[nchar] = 0;
    CU_ASSERT_STRING_EQUAL(buf, "1000 meter");

    metre = utScale(1, meter);
    CU_ASSERT_EQUAL(utGetStatus(), UT_SUCCESS);
    CU_ASSERT_PTR_NOT_NULL(metre);
    CU_ASSERT_EQUAL(utGetSystem(meter), utGetSystem(metre));
    CU_ASSERT_EQUAL(utCompare(meter, metre), 0);
    CU_ASSERT_EQUAL(meter, metre);
    utFree(metre);

    minute = utScale(60, second);
    CU_ASSERT_EQUAL(utGetStatus(), UT_SUCCESS);
    CU_ASSERT_PTR_NOT_NULL(minute);
    nchar = utFormat(minute, buf, sizeof(buf)-1, asciiSymbolDef);
    CU_ASSERT_TRUE_FATAL(nchar > 0);
    CU_ASSERT_TRUE_FATAL(nchar < sizeof(buf));
    buf[nchar] = 0;
    CU_ASSERT_STRING_EQUAL(buf, "60 s");

    nchar = utFormat(minute, buf, sizeof(buf)-1, asciiNameDef);
    CU_ASSERT_TRUE_FATAL(nchar > 0);
    CU_ASSERT_TRUE_FATAL(nchar < sizeof(buf));
    buf[nchar] = 0;
    CU_ASSERT_STRING_EQUAL(buf, "60 second");

    metre = utScale(1/1000., kilometer);
    CU_ASSERT_EQUAL(utGetStatus(), UT_SUCCESS);
    CU_ASSERT_PTR_NOT_NULL(metre);
    CU_ASSERT_EQUAL(utGetSystem(meter), utGetSystem(metre));
    utFree(metre);

    CU_ASSERT_PTR_NULL(utScale(0, meter));
    CU_ASSERT_EQUAL(utGetStatus(), UT_BADVALUE);

    CU_ASSERT_PTR_NULL(utScale(0, NULL));
    CU_ASSERT_EQUAL(utGetStatus(), UT_BADUNIT);

    rankine = utScale(1/1.8, kelvin);
    CU_ASSERT_EQUAL(utGetStatus(), UT_SUCCESS);
    CU_ASSERT_PTR_NOT_NULL(rankine);
}


static void
test_utOffset(void)
{
    utUnit*	dupKelvin;
    char	buf[80];
    int		nchar;

    celsius = utOffset(kelvin, 273.15);
    CU_ASSERT_EQUAL(utGetStatus(), UT_SUCCESS);
    CU_ASSERT_PTR_NOT_NULL(celsius);
    CU_ASSERT_EQUAL(utGetSystem(kelvin), utGetSystem(celsius));
    CU_ASSERT_NOT_EQUAL(utCompare(kelvin, celsius), 0);

    fahrenheit = utOffset(rankine, 459.67);
    CU_ASSERT_EQUAL(utGetStatus(), UT_SUCCESS);
    CU_ASSERT_PTR_NOT_NULL(fahrenheit);
    CU_ASSERT_EQUAL(utGetSystem(rankine), utGetSystem(fahrenheit));
    CU_ASSERT_NOT_EQUAL(utCompare(rankine, fahrenheit), 0);

    nchar = utFormat(celsius, buf, sizeof(buf)-1, asciiSymbolDef);
    CU_ASSERT_TRUE_FATAL(nchar > 0);
    CU_ASSERT_TRUE_FATAL(nchar < sizeof(buf));
    buf[nchar] = 0;
    CU_ASSERT_STRING_EQUAL(buf, "K @ 273.15");

    nchar = utFormat(celsius, buf, sizeof(buf)-1, asciiNameDef);
    CU_ASSERT_TRUE_FATAL(nchar > 0);
    CU_ASSERT_TRUE_FATAL(nchar < sizeof(buf));
    buf[nchar] = 0;
    CU_ASSERT_STRING_EQUAL(buf, "kelvin from 273.15");

    dupKelvin = utOffset(kelvin, 0);
    CU_ASSERT_EQUAL(utGetStatus(), UT_SUCCESS);
    CU_ASSERT_PTR_NOT_NULL(dupKelvin);
    CU_ASSERT_EQUAL(utGetSystem(kelvin), utGetSystem(dupKelvin));
    CU_ASSERT_EQUAL(utCompare(kelvin, dupKelvin), 0);
    CU_ASSERT_EQUAL(kelvin, dupKelvin);

    dupKelvin = utOffset(celsius, -273.15);
    CU_ASSERT_EQUAL(utGetStatus(), UT_SUCCESS);
    CU_ASSERT_PTR_NOT_NULL(dupKelvin);
    CU_ASSERT_EQUAL(utGetSystem(kelvin), utGetSystem(dupKelvin));

    utFree(dupKelvin);
}


static void
test_utMapUnitToName(void)
{
    utUnit*	metre;

    CU_ASSERT_EQUAL(utMapUnitToName(meter, "metre", UT_ASCII), UT_EXISTS);

    CU_ASSERT_EQUAL(utMapNameToUnit("metre", second), UT_EXISTS);
    CU_ASSERT_EQUAL(utMapNameToUnit("metre", NULL), UT_BADUNIT);

    metre = utGetUnitByName(unitSystem, "metre");
    CU_ASSERT_PTR_NOT_NULL(metre);
    CU_ASSERT_EQUAL(utCompare(metre, meter), 0);

    CU_ASSERT_EQUAL(utMapUnitToName(celsius, "degrees_celsius", UT_ASCII),
	UT_SUCCESS);

    utFree(metre);
}


static void
test_utMultiply(void)
{
    utUnit*	squareMeter;
    utUnit*	meterSecond;
    utUnit*	meterCelsius;
    utUnit*	meterRadian;
    utUnit*	kilometerMinute;
    char	buf[80];
    int		nchar;

    squareMeter = utMultiply(meter, meter);
    CU_ASSERT_PTR_NOT_NULL(squareMeter);
    CU_ASSERT_EQUAL(utGetStatus(), UT_SUCCESS);
    nchar = utFormat(squareMeter, buf, sizeof(buf)-1, asciiSymbolDef);
    CU_ASSERT_TRUE_FATAL(nchar > 0);
    CU_ASSERT_TRUE_FATAL(nchar < sizeof(buf));
    buf[nchar] = 0;
    CU_ASSERT_STRING_EQUAL(buf, "m2");

    nchar = utFormat(squareMeter, buf, sizeof(buf)-1, asciiNameDef);
    CU_ASSERT_TRUE_FATAL(nchar > 0);
    CU_ASSERT_TRUE_FATAL(nchar < sizeof(buf));
    buf[nchar] = 0;
    CU_ASSERT_STRING_EQUAL(buf, "meter^2");

    meterSecond = utMultiply(meter, second);
    CU_ASSERT_PTR_NOT_NULL(meterSecond);
    CU_ASSERT_EQUAL(utGetStatus(), UT_SUCCESS);
    nchar = utFormat(meterSecond, buf, sizeof(buf)-1, asciiSymbolDef);
    CU_ASSERT_TRUE_FATAL(nchar > 0);
    CU_ASSERT_TRUE_FATAL(nchar < sizeof(buf));
    buf[nchar] = 0;
    CU_ASSERT_STRING_EQUAL(buf, "m.s");

    nchar = utFormat(meterSecond, buf, sizeof(buf)-1, asciiNameDef);
    CU_ASSERT_TRUE_FATAL(nchar > 0);
    CU_ASSERT_TRUE_FATAL(nchar < sizeof(buf));
    buf[nchar] = 0;
    CU_ASSERT_STRING_EQUAL(buf, "meter-second");

    meterCelsius = utMultiply(meter, celsius);
    CU_ASSERT_PTR_NOT_NULL(meterCelsius);
    CU_ASSERT_EQUAL(utGetStatus(), UT_SUCCESS);
    nchar = utFormat(meterCelsius, buf, sizeof(buf)-1, asciiSymbolDef);
    CU_ASSERT_TRUE_FATAL(nchar > 0);
    CU_ASSERT_TRUE_FATAL(nchar < sizeof(buf));
    buf[nchar] = 0;
    CU_ASSERT_STRING_EQUAL(buf, "m.K");

    meterRadian = utMultiply(meter, radian);
    CU_ASSERT_PTR_NOT_NULL(meterRadian);
    CU_ASSERT_EQUAL(utGetStatus(), UT_SUCCESS);
    nchar = utFormat(meterRadian, buf, sizeof(buf)-1, asciiSymbolDef);
    CU_ASSERT_TRUE_FATAL(nchar > 0);
    CU_ASSERT_TRUE_FATAL(nchar < sizeof(buf));
    buf[nchar] = 0;
    CU_ASSERT_STRING_EQUAL(buf, "m.rad");

    kilometerMinute = utMultiply(kilometer, minute);
    CU_ASSERT_PTR_NOT_NULL(kilometerMinute);
    CU_ASSERT_EQUAL(utGetStatus(), UT_SUCCESS);
    nchar = utFormat(kilometerMinute, buf, sizeof(buf)-1, asciiSymbolDef);
    CU_ASSERT_TRUE_FATAL(nchar > 0);
    CU_ASSERT_TRUE_FATAL(nchar < sizeof(buf));
    buf[nchar] = 0;
    CU_ASSERT_STRING_EQUAL(buf, "60000 m.s");

    nchar = utFormat(kilometerMinute, buf, sizeof(buf)-1, asciiNameDef);
    CU_ASSERT_TRUE_FATAL(nchar > 0);
    CU_ASSERT_TRUE_FATAL(nchar < sizeof(buf));
    buf[nchar] = 0;
    CU_ASSERT_STRING_EQUAL(buf, "60000 meter-second");

    utFree(squareMeter);
    utFree(meterSecond);
    utFree(meterCelsius);
    utFree(meterRadian);
    utFree(kilometerMinute);
}


static void
test_utInvert(void)
{
    utUnit*	inverseMeter;
    utUnit*	inverseMinute;
    utUnit*	inverseCelsius;
    utUnit*	inverseRadian;
    utUnit*	inverseMeterSecond;
    char	buf[80];
    int		nchar;

    inverseMeter = utInvert(meter);
    CU_ASSERT_PTR_NOT_NULL(inverseMeter);
    CU_ASSERT_EQUAL(utGetStatus(), UT_SUCCESS);
    nchar = utFormat(inverseMeter, buf, sizeof(buf)-1, asciiSymbolDef);
    CU_ASSERT_TRUE_FATAL(nchar > 0);
    CU_ASSERT_TRUE_FATAL(nchar < sizeof(buf));
    buf[nchar] = 0;
    CU_ASSERT_STRING_EQUAL(buf, "m-1");

    inverseMinute = utInvert(minute);
    CU_ASSERT_PTR_NOT_NULL(inverseMinute);
    CU_ASSERT_EQUAL(utGetStatus(), UT_SUCCESS);
    nchar = utFormat(inverseMinute, buf, sizeof(buf)-1, asciiSymbolDef);
    CU_ASSERT_TRUE_FATAL(nchar > 0);
    CU_ASSERT_TRUE_FATAL(nchar < sizeof(buf));
    buf[nchar] = 0;
    CU_ASSERT_STRING_EQUAL(buf, "0.0166666666666667 s-1");

    inverseRadian = utInvert(radian);
    CU_ASSERT_PTR_NOT_NULL(inverseRadian);
    CU_ASSERT_EQUAL(utGetStatus(), UT_SUCCESS);
    nchar = utFormat(inverseRadian, buf, sizeof(buf)-1, asciiSymbolDef);
    CU_ASSERT_TRUE_FATAL(nchar > 0);
    CU_ASSERT_TRUE_FATAL(nchar < sizeof(buf));
    buf[nchar] = 0;
    CU_ASSERT_STRING_EQUAL(buf, "rad-1");

    inverseCelsius = utInvert(celsius);
    CU_ASSERT_PTR_NOT_NULL(inverseCelsius);
    CU_ASSERT_EQUAL(utGetStatus(), UT_SUCCESS);
    nchar = utFormat(inverseCelsius, buf, sizeof(buf)-1, asciiSymbolDef);
    CU_ASSERT_TRUE_FATAL(nchar > 0);
    CU_ASSERT_TRUE_FATAL(nchar < sizeof(buf));
    buf[nchar] = 0;
    CU_ASSERT_STRING_EQUAL(buf, "K-1");

    inverseMeterSecond = utInvert(utMultiply(meter, second));
    CU_ASSERT_PTR_NOT_NULL(inverseMeterSecond);
    CU_ASSERT_EQUAL(utGetStatus(), UT_SUCCESS);
    nchar = utFormat(inverseMeterSecond, buf, sizeof(buf)-1, asciiSymbolDef);
    CU_ASSERT_TRUE_FATAL(nchar > 0);
    CU_ASSERT_TRUE_FATAL(nchar < sizeof(buf));
    buf[nchar] = 0;
    CU_ASSERT_STRING_EQUAL(buf, "m-1.s-1");

    utFree(inverseMeter);
    utFree(inverseMinute);
    utFree(inverseCelsius);
    utFree(inverseRadian);
    utFree(inverseMeterSecond);

    hertz = utInvert(second);
    CU_ASSERT_PTR_NOT_NULL(hertz);
    CU_ASSERT_EQUAL(utGetStatus(), UT_SUCCESS);

    megahertz = utScale(1e6, utInvert(second));
    CU_ASSERT_PTR_NOT_NULL(megahertz);
    CU_ASSERT_EQUAL(utGetStatus(), UT_SUCCESS);
}


static void
test_utDivide(void)
{
    utUnit*	meterPerSecond;
    utUnit*	kilometerPerMinute;
    utUnit*	celsiusPerMeter;
    utUnit*	meterPerCelsius;
    char	buf[80];
    int		nchar;

    meterPerSecond = utDivide(meter, second);
    CU_ASSERT_PTR_NOT_NULL(meterPerSecond);
    CU_ASSERT_EQUAL(utGetStatus(), UT_SUCCESS);
    nchar = utFormat(meterPerSecond, buf, sizeof(buf)-1, asciiSymbolDef);
    CU_ASSERT_TRUE_FATAL(nchar > 0);
    CU_ASSERT_TRUE_FATAL(nchar < sizeof(buf));
    buf[nchar] = 0;
    CU_ASSERT_STRING_EQUAL(buf, "m.s-1");

    kilometerPerMinute = utDivide(kilometer, minute);
    CU_ASSERT_PTR_NOT_NULL(kilometerPerMinute);
    CU_ASSERT_EQUAL(utGetStatus(), UT_SUCCESS);
    nchar = utFormat(kilometerPerMinute, buf, sizeof(buf)-1, asciiSymbolDef);
    CU_ASSERT_TRUE_FATAL(nchar > 0);
    CU_ASSERT_TRUE_FATAL(nchar < sizeof(buf));
    buf[nchar] = 0;
    CU_ASSERT_STRING_EQUAL(buf, "16.6666666666667 m.s-1");

    celsiusPerMeter = utDivide(celsius, meter);
    CU_ASSERT_PTR_NOT_NULL(celsiusPerMeter);
    CU_ASSERT_EQUAL(utGetStatus(), UT_SUCCESS);
    nchar = utFormat(celsiusPerMeter, buf, sizeof(buf)-1, asciiSymbolDef);
    CU_ASSERT_TRUE_FATAL(nchar > 0);
    CU_ASSERT_TRUE_FATAL(nchar < sizeof(buf));
    buf[nchar] = 0;
    CU_ASSERT_STRING_EQUAL(buf, "m-1.K");

    meterPerCelsius = utDivide(meter, celsius);
    CU_ASSERT_PTR_NOT_NULL(meterPerCelsius);
    CU_ASSERT_EQUAL(utGetStatus(), UT_SUCCESS);
    nchar = utFormat(meterPerCelsius, buf, sizeof(buf)-1, asciiSymbolDef);
    CU_ASSERT_TRUE_FATAL(nchar > 0);
    CU_ASSERT_TRUE_FATAL(nchar < sizeof(buf));
    buf[nchar] = 0;
    CU_ASSERT_STRING_EQUAL(buf, "m.K-1");

    watt = utDivide(utMultiply(kilogram, utRaise(utDivide(meter, second), 2)),
	second);
    CU_ASSERT_PTR_NOT_NULL(watt);
    CU_ASSERT_EQUAL(utGetStatus(), UT_SUCCESS);
    nchar = utFormat(watt, buf, sizeof(buf)-1, asciiSymbolDef);
    CU_ASSERT_TRUE_FATAL(nchar > 0);
    CU_ASSERT_TRUE_FATAL(nchar < sizeof(buf));
    buf[nchar] = 0;
    CU_ASSERT_STRING_EQUAL(buf, "kg.m2.s-3");

    utFree(meterPerSecond);
    utFree(kilometerPerMinute);
    utFree(celsiusPerMeter);
    utFree(meterPerCelsius);
}


static void
test_utRaise(void)
{
    utUnit*	perCubicMeter;
    utUnit*	celsiusCubed;
    utUnit*	kilometersSquaredPerMinuteSquared;

    char	buf[80];
    int		nchar;

    perCubicMeter = utRaise(meter, -3);
    CU_ASSERT_PTR_NOT_NULL(perCubicMeter);
    CU_ASSERT_EQUAL(utGetStatus(), UT_SUCCESS);
    nchar = utFormat(perCubicMeter, buf, sizeof(buf)-1, asciiSymbolDef);
    CU_ASSERT_TRUE_FATAL(nchar > 0);
    CU_ASSERT_TRUE_FATAL(nchar < sizeof(buf));
    buf[nchar] = 0;
    CU_ASSERT_STRING_EQUAL(buf, "m-3");

    celsiusCubed = utRaise(celsius, 3);
    CU_ASSERT_PTR_NOT_NULL(celsiusCubed);
    CU_ASSERT_EQUAL(utGetStatus(), UT_SUCCESS);
    nchar = utFormat(celsiusCubed, buf, sizeof(buf)-1, asciiSymbolDef);
    CU_ASSERT_TRUE_FATAL(nchar > 0);
    CU_ASSERT_TRUE_FATAL(nchar < sizeof(buf));
    buf[nchar] = 0;
    CU_ASSERT_STRING_EQUAL(buf, "K3");

    kilometersSquaredPerMinuteSquared =
	utRaise(utDivide(minute, kilometer), -2);
    CU_ASSERT_PTR_NOT_NULL(kilometersSquaredPerMinuteSquared);
    CU_ASSERT_EQUAL(utGetStatus(), UT_SUCCESS);
    nchar = utFormat(kilometersSquaredPerMinuteSquared, buf,
	sizeof(buf)-1, asciiSymbolDef);
    CU_ASSERT_TRUE_FATAL(nchar > 0);
    CU_ASSERT_TRUE_FATAL(nchar < sizeof(buf));
    buf[nchar] = 0;
    CU_ASSERT_STRING_EQUAL(buf, "277.777777777778 m2.s-2");

    utFree(perCubicMeter);
    utFree(celsiusCubed);
    utFree(kilometersSquaredPerMinuteSquared);
}


static void
test_utLog(void)
{
    utUnit*	bel_1_mW = utLog(M_LOG10E, utScale(0.001, watt));
    utUnit*	decibel_1_mW;
    utUnit*	dummy;
    char	buf[80];
    int		nchar;

    CU_ASSERT_PTR_NOT_NULL(bel_1_mW);
    CU_ASSERT_EQUAL(utGetStatus(), UT_SUCCESS);
    nchar = utFormat(bel_1_mW, buf, sizeof(buf)-1, asciiSymbolDef);
    CU_ASSERT_TRUE_FATAL(nchar > 0);
    CU_ASSERT_TRUE_FATAL(nchar < sizeof(buf));
    buf[nchar] = 0;
    CU_ASSERT_STRING_EQUAL(buf, "lg(re 0.001 kg.m2.s-3)");

    decibel_1_mW = utScale(0.1, bel_1_mW);
    CU_ASSERT_PTR_NOT_NULL(decibel_1_mW);
    CU_ASSERT_EQUAL(utGetStatus(), UT_SUCCESS);
    nchar = utFormat(decibel_1_mW, buf, sizeof(buf)-1, asciiSymbolDef);
    CU_ASSERT_TRUE_FATAL(nchar > 0);
    CU_ASSERT_TRUE_FATAL(nchar < sizeof(buf));
    buf[nchar] = 0;
    CU_ASSERT_STRING_EQUAL(buf, "0.1 lg(re 0.001 kg.m2.s-3)");

    dummy = utLog(-M_LOG10E, utScale(0.001, watt));
    CU_ASSERT_PTR_NULL(dummy);
    CU_ASSERT_EQUAL(utGetStatus(), UT_BADVALUE);

    cubicMicron = utRaise(utScale(1e-6, meter), 3);
    CU_ASSERT_PTR_NOT_NULL(cubicMicron);

    dBZ = utScale(0.1, utLog(M_LOG10E, cubicMicron));
    CU_ASSERT_PTR_NOT_NULL(dBZ);
    CU_ASSERT_EQUAL(utGetStatus(), UT_SUCCESS);
    nchar = utFormat(dBZ, buf, sizeof(buf)-1, asciiSymbolDef);
    CU_ASSERT_TRUE_FATAL(nchar > 0);
    CU_ASSERT_TRUE_FATAL(nchar < sizeof(buf));
    buf[nchar] = 0;
    CU_ASSERT_STRING_EQUAL(buf, "0.1 lg(re 1e-18 m3)");
    CU_ASSERT_EQUAL(utMapSymbolToUnit("dBZ", dBZ), UT_SUCCESS);
    CU_ASSERT_EQUAL(utMapSymbolToUnit("dBZ", dBZ), UT_SUCCESS);
    CU_ASSERT_EQUAL(utMapSymbolToUnit("dBZ", meter), UT_EXISTS);
    {
	utUnit*	unit = utGetUnitBySymbol(unitSystem, "dBZ");

	CU_ASSERT_PTR_NOT_NULL(unit);
	CU_ASSERT_EQUAL(utCompare(unit, dBZ), 0);

	CU_ASSERT_PTR_NULL(utGetUnitBySymbol(unitSystem, "DBZ"));
    }

    utFree(bel_1_mW);
    utFree(decibel_1_mW);
    utFree(dummy);
}


static int
areCloseFloats(
    float	x,
    float	y)
{
    int		areClose;

    if (x == 0 || y == 0) {
	areClose = fabs(x - y) < 1000*FLT_EPSILON;
    }
    else {
	areClose = fabs(1.0 - x / y) < 1000*FLT_EPSILON;
    }

    return areClose;
}


static int
areCloseDoubles(
    double	x,
    double	y)
{
    int		areClose;

    if (x == 0 || y == 0) {
	areClose = fabs(x - y) < 1000*DBL_EPSILON;
    }
    else {
	areClose = fabs(1.0 - x / y) < 1000*DBL_EPSILON;
    }

    return areClose;
}


static void
test_utGetConverter(void)
{
    cvConverter*	converter = utGetConverter(meter, meter);
    double		doubles[2];
    float		floats[2];

    CU_ASSERT_PTR_NOT_NULL(converter);
    CU_ASSERT_EQUAL(cvConvertDouble(converter, 1.0), 1.0);
    floats[0] = 1; floats[1] = 2;
    CU_ASSERT_EQUAL(cvConvertFloats(converter, floats, 2, floats), floats);
    CU_ASSERT_EQUAL(floats[0], 1);
    CU_ASSERT_EQUAL(floats[1], 2);
    doubles[0] = 1; doubles[1] = 2;
    CU_ASSERT_EQUAL(cvConvertDoubles(converter, doubles, 2, doubles), doubles);
    CU_ASSERT_EQUAL(doubles[0], 1);
    CU_ASSERT_EQUAL(doubles[1], 2);
    CU_ASSERT_EQUAL(cvConvertFloats(converter, floats, 1, floats+1), floats+1);
    CU_ASSERT_EQUAL(floats[1], 1);
    CU_ASSERT_EQUAL(cvConvertDoubles(converter, doubles, 1, doubles+1),
	doubles+1);
    CU_ASSERT_EQUAL(doubles[1], 1);
    floats[0] = 1; floats[1] = 2;
    CU_ASSERT_EQUAL(cvConvertFloats(converter, floats+1, 1, floats), floats);
    CU_ASSERT_EQUAL(floats[0], 2);
    doubles[0] = 1; doubles[1] = 2;
    CU_ASSERT_EQUAL(cvConvertDoubles(converter, doubles+1, 1, doubles),
	doubles);
    CU_ASSERT_EQUAL(doubles[0], 2);
    cvFree(converter);

    converter = utGetConverter(radian, radian);
    CU_ASSERT_PTR_NOT_NULL(converter);
    CU_ASSERT_EQUAL(cvConvertDouble(converter, 1.0), 1.0);
    cvFree(converter);

    converter = utGetConverter(meter, radian);
    CU_ASSERT_PTR_NULL(converter);
    converter = utGetConverter(meter, kelvin);
    CU_ASSERT_PTR_NULL(converter);
    converter = utGetConverter(meter, celsius);
    CU_ASSERT_PTR_NULL(converter);
    converter = utGetConverter(meter, fahrenheit);
    CU_ASSERT_PTR_NULL(converter);
    converter = utGetConverter(meter, dBZ);
    CU_ASSERT_PTR_NULL(converter);
    converter = utGetConverter(dBZ, radian);
    CU_ASSERT_PTR_NULL(converter);

    converter = utGetConverter(kilometer, kilometer);
    CU_ASSERT_PTR_NOT_NULL(converter);
    CU_ASSERT_EQUAL(cvConvertDouble(converter, 1.0), 1.0);
    cvFree(converter);

    converter = utGetConverter(meter, kilometer);
    CU_ASSERT_PTR_NOT_NULL(converter);
    CU_ASSERT_EQUAL(cvConvertDouble(converter, 1000.0), 1.0);
    floats[0] = 0; floats[1] = 1000;
    CU_ASSERT_EQUAL(cvConvertFloats(converter, floats, 2, floats), floats);
    CU_ASSERT_EQUAL(floats[0], 0);
    CU_ASSERT_EQUAL(floats[1], 1);
    doubles[0] = 0; doubles[1] = 1000;
    CU_ASSERT_EQUAL(cvConvertDoubles(converter, doubles, 2, doubles), doubles);
    CU_ASSERT_EQUAL(doubles[0], 0);
    CU_ASSERT_EQUAL(doubles[1], 1);
    floats[0] = 0; floats[1] = 1000;
    CU_ASSERT_EQUAL(cvConvertFloats(converter, floats, 1, floats+1), floats+1);
    CU_ASSERT_EQUAL(floats[1], 0);
    doubles[0] = 0; doubles[1] = 1000;
    CU_ASSERT_EQUAL(cvConvertDoubles(converter, doubles, 1, doubles+1), 
	doubles+1);
    CU_ASSERT_EQUAL(doubles[1], 0);
    floats[0] = 0; floats[1] = 1000;
    CU_ASSERT_EQUAL(cvConvertFloats(converter, floats+1, 1, floats), floats);
    CU_ASSERT_EQUAL(floats[0], 1);
    doubles[0] = 0; doubles[1] = 1000;
    CU_ASSERT_EQUAL(cvConvertDoubles(converter, doubles+1, 1, doubles), 
	doubles);
    CU_ASSERT_EQUAL(doubles[0], 1);
    cvFree(converter);

    converter = utGetConverter(kilometer, meter);
    CU_ASSERT_PTR_NOT_NULL(converter);
    CU_ASSERT_EQUAL(cvConvertDouble(converter, 1.0), 1000.0);
    floats[0] = 0; floats[1] = 1;
    CU_ASSERT_EQUAL(cvConvertFloats(converter, floats, 2, floats), floats);
    CU_ASSERT_EQUAL(floats[0], 0);
    CU_ASSERT_EQUAL(floats[1], 1000);
    doubles[0] = 0; doubles[1] = 1;
    CU_ASSERT_EQUAL(cvConvertDoubles(converter, doubles, 2, doubles), doubles);
    CU_ASSERT_EQUAL(doubles[0], 0);
    CU_ASSERT_EQUAL(doubles[1], 1000);
    floats[0] = 0; floats[1] = 1;
    CU_ASSERT_EQUAL(cvConvertFloats(converter, floats, 1, floats+1), floats+1);
    CU_ASSERT_EQUAL(floats[1], 0);
    doubles[0] = 0; doubles[1] = 1;
    CU_ASSERT_EQUAL(cvConvertDoubles(converter, doubles, 1, doubles+1), 
	doubles+1);
    CU_ASSERT_EQUAL(doubles[1], 0);
    floats[0] = 0; floats[1] = 1;
    CU_ASSERT_EQUAL(cvConvertFloats(converter, floats+1, 1, floats), floats);
    CU_ASSERT_EQUAL(floats[0], 1000);
    doubles[0] = 0; doubles[1] = 1;
    CU_ASSERT_EQUAL(cvConvertDoubles(converter, doubles+1, 1, doubles), 
	doubles);
    CU_ASSERT_EQUAL(doubles[0], 1000);
    cvFree(converter);

    converter = utGetConverter(kelvin, celsius);
    CU_ASSERT_PTR_NOT_NULL(converter);
    CU_ASSERT_EQUAL(cvConvertDouble(converter, 273.15), 0.0);
    floats[0] = 0; floats[1] = 273.15;
    CU_ASSERT_EQUAL(cvConvertFloats(converter, floats, 2, floats), floats);
    CU_ASSERT_TRUE(areCloseFloats(floats[0], -273.15));
    CU_ASSERT_TRUE(areCloseFloats(floats[1], 0));
    doubles[0] = 0; doubles[1] = 273.15;
    CU_ASSERT_EQUAL(cvConvertDoubles(converter, doubles, 2, doubles), doubles);
    CU_ASSERT_EQUAL(doubles[0], -273.15);
    CU_ASSERT_EQUAL(doubles[1], 0);
    floats[0] = 0; floats[1] = 273.15;
    CU_ASSERT_EQUAL(cvConvertFloats(converter, floats, 1, floats+1), floats+1);
    CU_ASSERT_TRUE(areCloseFloats(floats[1], -273.15));
    doubles[0] = 0; doubles[1] = 273.15;
    CU_ASSERT_EQUAL(cvConvertDoubles(converter, doubles, 1, doubles+1), 
	doubles+1);
    CU_ASSERT_EQUAL(doubles[1], -273.15);
    floats[0] = 0; floats[1] = 273.15;
    CU_ASSERT_EQUAL(cvConvertFloats(converter, floats+1, 1, floats), floats);
    CU_ASSERT_TRUE(areCloseFloats(floats[0], 0));
    doubles[0] = 0; doubles[1] = 273.15;
    CU_ASSERT_EQUAL(cvConvertDoubles(converter, doubles+1, 1, doubles), 
	doubles);
    CU_ASSERT_EQUAL(doubles[0], 0);
    cvFree(converter);

    converter = utGetConverter(celsius, kelvin);
    CU_ASSERT_PTR_NOT_NULL(converter);
    CU_ASSERT_EQUAL(cvConvertDouble(converter, 0.0), 273.15);
    floats[0] = 0; floats[1] = -273.15;
    CU_ASSERT_EQUAL(cvConvertFloats(converter, floats, 2, floats), floats);
    CU_ASSERT_TRUE(areCloseFloats(floats[0], 273.15));
    CU_ASSERT_TRUE(areCloseFloats(floats[1], 0));
    doubles[0] = 0; doubles[1] = -273.15;
    CU_ASSERT_EQUAL(cvConvertDoubles(converter, doubles, 2, doubles), doubles);
    CU_ASSERT_EQUAL(doubles[0], 273.15);
    CU_ASSERT_EQUAL(doubles[1], 0);
    floats[0] = 0; floats[1] = -273.15;
    CU_ASSERT_EQUAL(cvConvertFloats(converter, floats, 1, floats+1), floats+1);
    CU_ASSERT_TRUE(areCloseFloats(floats[1], 273.15));
    doubles[0] = 0; doubles[1] = -273.15;
    CU_ASSERT_EQUAL(cvConvertDoubles(converter, doubles, 1, doubles+1), 
	doubles+1);
    CU_ASSERT_EQUAL(doubles[1], 273.15);
    floats[0] = 0; floats[1] = -273.15;
    CU_ASSERT_EQUAL(cvConvertFloats(converter, floats+1, 1, floats), floats);
    CU_ASSERT_TRUE(areCloseFloats(floats[0], 0));
    doubles[0] = 0; doubles[1] = -273.15;
    CU_ASSERT_EQUAL(cvConvertDoubles(converter, doubles+1, 1, doubles), 
	doubles);
    CU_ASSERT_EQUAL(doubles[0], 0);
    cvFree(converter);

    converter = utGetConverter(celsius, fahrenheit);
    CU_ASSERT_PTR_NOT_NULL(converter);
    CU_ASSERT_TRUE(areCloseDoubles(cvConvertDouble(converter, 0.0), 32));
    floats[0] = 0; floats[1] = 100;
    CU_ASSERT_EQUAL(cvConvertFloats(converter, floats, 2, floats), floats);
    CU_ASSERT_TRUE(areCloseFloats(floats[0], 32));
    CU_ASSERT_TRUE(areCloseFloats(floats[1], 212));
    doubles[0] = 0; doubles[1] = 100;
    CU_ASSERT_EQUAL(cvConvertDoubles(converter, doubles, 2, doubles), doubles);
    CU_ASSERT_TRUE(areCloseDoubles(doubles[0], 32));
    CU_ASSERT_TRUE(areCloseDoubles(doubles[1], 212));
    floats[0] = 0; floats[1] = 100;
    CU_ASSERT_EQUAL(cvConvertFloats(converter, floats, 1, floats+1), floats+1);
    CU_ASSERT_TRUE(areCloseFloats(floats[1], 32));
    doubles[0] = 0; doubles[1] = 100;
    CU_ASSERT_EQUAL(cvConvertDoubles(converter, doubles, 1, doubles+1), 
	doubles+1);
    CU_ASSERT_TRUE(areCloseDoubles(doubles[1], 32));
    floats[0] = 0; floats[1] = 100;
    CU_ASSERT_EQUAL(cvConvertFloats(converter, floats+1, 1, floats), floats);
    CU_ASSERT_TRUE(areCloseFloats(floats[0], 212));
    doubles[0] = 0; doubles[1] = 100;
    CU_ASSERT_EQUAL(cvConvertDoubles(converter, doubles+1, 1, doubles), 
	doubles);
    CU_ASSERT_TRUE(areCloseDoubles(doubles[0], 212));
    cvFree(converter);

    converter = utGetConverter(fahrenheit, celsius);
    CU_ASSERT_PTR_NOT_NULL(converter);
    CU_ASSERT_TRUE(areCloseDoubles(cvConvertDouble(converter, 32.0), 0.0));
    floats[0] = 32; floats[1] = 212;
    CU_ASSERT_EQUAL(cvConvertFloats(converter, floats, 2, floats), floats);
    CU_ASSERT_TRUE(areCloseFloats(floats[0], 0));
    CU_ASSERT_TRUE(areCloseFloats(floats[1], 100));
    doubles[0] = 32; doubles[1] = 212;
    CU_ASSERT_EQUAL(cvConvertDoubles(converter, doubles, 2, doubles), doubles);
    CU_ASSERT_TRUE(areCloseDoubles(doubles[0], 0));
    CU_ASSERT_TRUE(areCloseDoubles(doubles[1], 100));
    floats[0] = 32; floats[1] = 212;
    CU_ASSERT_EQUAL(cvConvertFloats(converter, floats, 1, floats+1), floats+1);
    CU_ASSERT_TRUE(areCloseFloats(floats[1], 0));
    doubles[0] = 32; doubles[1] = 212;
    CU_ASSERT_EQUAL(cvConvertDoubles(converter, doubles, 1, doubles+1), 
	doubles+1);
    CU_ASSERT_TRUE(areCloseDoubles(doubles[1], 0));
    floats[0] = 32; floats[1] = 212;
    CU_ASSERT_EQUAL(cvConvertFloats(converter, floats+1, 1, floats), floats);
    CU_ASSERT_TRUE(areCloseFloats(floats[0], 100));
    doubles[0] = 32; doubles[1] = 212;
    CU_ASSERT_EQUAL(cvConvertDoubles(converter, doubles+1, 1, doubles), 
	doubles);
    CU_ASSERT_TRUE(areCloseDoubles(doubles[0], 100));
    cvFree(converter);

    converter = utGetConverter(cubicMicron, dBZ);
    CU_ASSERT_PTR_NOT_NULL(converter);
    CU_ASSERT_TRUE(areCloseDoubles(cvConvertDouble(converter, 1000), 30.0));
    floats[0] = 10; floats[1] = 1000;
    CU_ASSERT_EQUAL(cvConvertFloats(converter, floats, 2, floats), floats);
    CU_ASSERT_TRUE(areCloseFloats(floats[0], 10));
    CU_ASSERT_TRUE(areCloseFloats(floats[1], 30));
    doubles[0] = 10; doubles[1] = 1000;
    CU_ASSERT_EQUAL(cvConvertDoubles(converter, doubles, 2, doubles), doubles);
    CU_ASSERT_TRUE(areCloseDoubles(doubles[0], 10));
    CU_ASSERT_TRUE(areCloseDoubles(doubles[1], 30));
    floats[0] = 10; floats[1] = 1000;
    CU_ASSERT_EQUAL(cvConvertFloats(converter, floats, 1, floats+1), floats+1);
    CU_ASSERT_TRUE(areCloseFloats(floats[1], 10));
    doubles[0] = 10; doubles[1] = 1000;
    CU_ASSERT_EQUAL(cvConvertDoubles(converter, doubles, 1, doubles+1), 
	doubles+1);
    CU_ASSERT_TRUE(areCloseDoubles(doubles[1], 10));
    floats[0] = 10; floats[1] = 1000;
    CU_ASSERT_EQUAL(cvConvertFloats(converter, floats+1, 1, floats), floats);
    CU_ASSERT_TRUE(areCloseFloats(floats[0], 30));
    doubles[0] = 10; doubles[1] = 1000;
    CU_ASSERT_EQUAL(cvConvertDoubles(converter, doubles+1, 1, doubles), 
	doubles);
    CU_ASSERT_TRUE(areCloseDoubles(doubles[0], 30));
    cvFree(converter);

    converter = utGetConverter(dBZ, cubicMicron);
    CU_ASSERT_PTR_NOT_NULL(converter);
    CU_ASSERT_TRUE(areCloseFloats(cvConvertFloat(converter, 10), 10.0));
    CU_ASSERT_TRUE(areCloseFloats(cvConvertFloat(converter, 30), 1000.0));
    CU_ASSERT_TRUE(areCloseDoubles(cvConvertDouble(converter, 10), 10.0));
    CU_ASSERT_TRUE(areCloseDoubles(cvConvertDouble(converter, 30), 1000.0));
    floats[0] = 10; floats[1] = 30;
    CU_ASSERT_EQUAL(cvConvertFloats(converter, floats, 2, floats), floats);
    CU_ASSERT_TRUE(areCloseFloats(floats[0], 10));
    CU_ASSERT_TRUE(areCloseFloats(floats[1], 1000));
    doubles[0] = 10; doubles[1] = 30;
    CU_ASSERT_EQUAL(cvConvertDoubles(converter, doubles, 2, doubles), doubles);
    CU_ASSERT_TRUE(areCloseDoubles(doubles[0], 10));
    CU_ASSERT_TRUE(areCloseDoubles(doubles[1], 1000));
    floats[0] = 10; floats[1] = 30;
    CU_ASSERT_EQUAL(cvConvertFloats(converter, floats, 1, floats+1), floats+1);
    CU_ASSERT_TRUE(areCloseFloats(floats[1], 10));
    doubles[0] = 10; doubles[1] = 30;
    CU_ASSERT_EQUAL(cvConvertDoubles(converter, doubles, 1, doubles+1), 
	doubles+1);
    CU_ASSERT_TRUE(areCloseDoubles(doubles[1], 10));
    floats[0] = 10; floats[1] = 30;
    CU_ASSERT_EQUAL(cvConvertFloats(converter, floats+1, 1, floats), floats);
    CU_ASSERT_TRUE(areCloseFloats(floats[0], 1000));
    doubles[0] = 10; doubles[1] = 30;
    CU_ASSERT_EQUAL(cvConvertDoubles(converter, doubles+1, 1, doubles), 
	doubles);
    CU_ASSERT_TRUE(areCloseDoubles(doubles[0], 1000));
    cvFree(converter);

    converter = utGetConverter(second, hertz);
    CU_ASSERT_PTR_NOT_NULL(converter);
    CU_ASSERT_TRUE(areCloseFloats(cvConvertFloat(converter, 1.0), 1.0));
    CU_ASSERT_TRUE(areCloseFloats(cvConvertFloat(converter, 5.0), 1/5.0));
    CU_ASSERT_TRUE(areCloseDoubles(cvConvertDouble(converter, 1.0), 1.0));
    CU_ASSERT_TRUE(areCloseDoubles(cvConvertDouble(converter, 5.0), 1/5.0));
    floats[0] = 1; floats[1] = 5;
    CU_ASSERT_EQUAL(cvConvertFloats(converter, floats, 2, floats), floats);
    CU_ASSERT_TRUE(areCloseFloats(floats[0], 1.0));
    CU_ASSERT_TRUE(areCloseFloats(floats[1], 1.0/5));
    doubles[0] = 1; doubles[1] = 5;
    CU_ASSERT_EQUAL(cvConvertDoubles(converter, doubles, 2, doubles), doubles);
    CU_ASSERT_TRUE(areCloseDoubles(doubles[0], 1));
    CU_ASSERT_TRUE(areCloseDoubles(doubles[1], 1.0/5));
    cvFree(converter);

    converter = utGetConverter(second, megahertz);
    CU_ASSERT_PTR_NOT_NULL(converter);
    CU_ASSERT_TRUE(areCloseFloats(cvConvertFloat(converter, 1), 1e-6));
    CU_ASSERT_TRUE(areCloseFloats(cvConvertFloat(converter, 1e-6), 1.0));
    CU_ASSERT_TRUE(areCloseDoubles(cvConvertDouble(converter, 1), 1e-6));
    CU_ASSERT_TRUE(areCloseDoubles(cvConvertDouble(converter, 1e-6), 1.0));
    floats[0] = 1; floats[1] = 1e-6;
    CU_ASSERT_EQUAL(cvConvertFloats(converter, floats, 2, floats), floats);
    CU_ASSERT_TRUE(areCloseFloats(floats[0], 1e-6));
    CU_ASSERT_TRUE(areCloseFloats(floats[1], 1));
    doubles[0] = 1; doubles[1] = 1e-6;
    CU_ASSERT_EQUAL(cvConvertDoubles(converter, doubles, 2, doubles), doubles);
    CU_ASSERT_TRUE(areCloseDoubles(doubles[0], 1e-6));
    CU_ASSERT_TRUE(areCloseDoubles(doubles[1], 1));
    cvFree(converter);
}


static void
test_utOffsetByTime(void)
{
    char		buf[80];
    int			nchar;
    cvConverter*	converter;
    utUnit*		day;
    utUnit*		daysSinceTheEpoch;
    double		doubles[2];
    float		floats[2];

    secondsSinceTheEpoch = utOffsetByTime(second, 1970, 1, 1, 0, 0, 0);
    CU_ASSERT_PTR_NOT_NULL(secondsSinceTheEpoch);
    nchar = utFormat(secondsSinceTheEpoch, buf, sizeof(buf)-1, asciiSymbolDef);
    CU_ASSERT_TRUE_FATAL(nchar > 0);
    CU_ASSERT_TRUE_FATAL(nchar < sizeof(buf));
    buf[nchar] = 0;
    CU_ASSERT_STRING_EQUAL(buf, "s @ 19700101T000000.0000000 UTC");

    nchar = utFormat(secondsSinceTheEpoch, buf, sizeof(buf)-1, asciiNameDef);
    CU_ASSERT_TRUE_FATAL(nchar > 0);
    CU_ASSERT_TRUE_FATAL(nchar < sizeof(buf));
    buf[nchar] = 0;
    CU_ASSERT_STRING_EQUAL(buf, "second since 1970-01-01 00:00:00.0000000 UTC");

    converter = utGetConverter(secondsSinceTheEpoch, secondsSinceTheEpoch);
    CU_ASSERT_PTR_NOT_NULL(converter);
    CU_ASSERT_TRUE(areCloseDoubles(cvConvertDouble(converter, 1000), 1000));
    floats[0] = 0; floats[1] = 1000;
    CU_ASSERT_EQUAL(cvConvertFloats(converter, floats, 2, floats), floats);
    CU_ASSERT_TRUE(areCloseFloats(floats[0], 0));
    CU_ASSERT_TRUE(areCloseFloats(floats[1], 1000));
    doubles[0] = 0; doubles[1] = 1000;
    CU_ASSERT_EQUAL(cvConvertDoubles(converter, doubles, 2, doubles), doubles);
    CU_ASSERT_TRUE(areCloseDoubles(doubles[0], 0));
    CU_ASSERT_TRUE(areCloseDoubles(doubles[1], 1000));
    floats[0] = 0; floats[1] = 1000;
    CU_ASSERT_EQUAL(cvConvertFloats(converter, floats, 1, floats+1), floats+1);
    CU_ASSERT_TRUE(areCloseFloats(floats[1], 0));
    doubles[0] = 0; doubles[1] = 1000;
    CU_ASSERT_EQUAL(cvConvertDoubles(converter, doubles, 1, doubles+1), 
	doubles+1);
    CU_ASSERT_TRUE(areCloseDoubles(doubles[1], 0));
    floats[0] = 0; floats[1] = 1000;
    CU_ASSERT_EQUAL(cvConvertFloats(converter, floats+1, 1, floats), floats);
    CU_ASSERT_TRUE(areCloseFloats(floats[0], 1000));
    doubles[0] = 0; doubles[1] = 1000;
    CU_ASSERT_EQUAL(cvConvertDoubles(converter, doubles+1, 1, doubles), 
	doubles);
    CU_ASSERT_TRUE(areCloseDoubles(doubles[0], 1000));
    cvFree(converter);

    day = utScale(86400, second);
    CU_ASSERT_PTR_NOT_NULL_FATAL(day);
    daysSinceTheEpoch = utOffsetByTime(day, 1970, 1, 1, 0, 0, 0);
    utFree(day);
    CU_ASSERT_PTR_NOT_NULL_FATAL(daysSinceTheEpoch);
    nchar = utFormat(daysSinceTheEpoch, buf, sizeof(buf)-1, asciiSymbolDef);
    CU_ASSERT_TRUE_FATAL(nchar > 0);
    CU_ASSERT_TRUE_FATAL(nchar < sizeof(buf));
    buf[nchar] = 0;
    CU_ASSERT_STRING_EQUAL(buf, "(86400 s) @ 19700101T000000.0000000 UTC");

    converter = utGetConverter(secondsSinceTheEpoch, daysSinceTheEpoch);
    CU_ASSERT_PTR_NOT_NULL(converter);
    CU_ASSERT_TRUE(areCloseDoubles(cvConvertDouble(converter, 86400), 1));
    floats[0] = 0; floats[1] = 86400;
    CU_ASSERT_EQUAL(cvConvertFloats(converter, floats, 2, floats), floats);
    CU_ASSERT_TRUE(areCloseFloats(floats[0], 0));
    CU_ASSERT_TRUE(areCloseFloats(floats[1], 1));
    doubles[0] = 0; doubles[1] = 86400;
    CU_ASSERT_EQUAL(cvConvertDoubles(converter, doubles, 2, doubles), doubles);
    CU_ASSERT_TRUE(areCloseDoubles(doubles[0], 0));
    CU_ASSERT_TRUE(areCloseDoubles(doubles[1], 1));
    floats[0] = 0; floats[1] = 86400;
    CU_ASSERT_EQUAL(cvConvertFloats(converter, floats, 1, floats+1), floats+1);
    CU_ASSERT_TRUE(areCloseFloats(floats[1], 0));
    doubles[0] = 0; doubles[1] = 86400;
    CU_ASSERT_EQUAL(cvConvertDoubles(converter, doubles, 1, doubles+1), 
	doubles+1);
    CU_ASSERT_TRUE(areCloseDoubles(doubles[1], 0));
    floats[0] = 0; floats[1] = 86400;
    CU_ASSERT_EQUAL(cvConvertFloats(converter, floats+1, 1, floats), floats);
    CU_ASSERT_TRUE(areCloseFloats(floats[0], 1));
    doubles[0] = 0; doubles[1] = 86400;
    CU_ASSERT_EQUAL(cvConvertDoubles(converter, doubles+1, 1, doubles), 
	doubles);
    CU_ASSERT_TRUE(areCloseDoubles(doubles[0], 1));
    cvFree(converter);

    converter = utGetConverter(daysSinceTheEpoch, secondsSinceTheEpoch);
    CU_ASSERT_PTR_NOT_NULL(converter);
    CU_ASSERT_TRUE(areCloseDoubles(cvConvertDouble(converter, 1), 86400));
    floats[0] = 0; floats[1] = 1;
    CU_ASSERT_EQUAL(cvConvertFloats(converter, floats, 2, floats), floats);
    CU_ASSERT_TRUE(areCloseFloats(floats[0], 0));
    CU_ASSERT_TRUE(areCloseFloats(floats[1], 86400));
    doubles[0] = 0; doubles[1] = 1;
    CU_ASSERT_EQUAL(cvConvertDoubles(converter, doubles, 2, doubles), doubles);
    CU_ASSERT_TRUE(areCloseDoubles(doubles[0], 0));
    CU_ASSERT_TRUE(areCloseDoubles(doubles[1], 86400));
    floats[0] = 0; floats[1] = 1;
    CU_ASSERT_EQUAL(cvConvertFloats(converter, floats, 1, floats+1), floats+1);
    CU_ASSERT_TRUE(areCloseFloats(floats[1], 0));
    doubles[0] = 0; doubles[1] = 1;
    CU_ASSERT_EQUAL(cvConvertDoubles(converter, doubles, 1, doubles+1), 
	doubles+1);
    CU_ASSERT_TRUE(areCloseDoubles(doubles[1], 0));
    floats[0] = 0; floats[1] = 1;
    CU_ASSERT_EQUAL(cvConvertFloats(converter, floats+1, 1, floats), floats);
    CU_ASSERT_TRUE(areCloseFloats(floats[0], 86400));
    doubles[0] = 0; doubles[1] = 1;
    CU_ASSERT_EQUAL(cvConvertDoubles(converter, doubles+1, 1, doubles), 
	doubles);
    CU_ASSERT_TRUE(areCloseDoubles(doubles[0], 86400));
    cvFree(converter);

    utFree(daysSinceTheEpoch);
}



static void
test_utSetEncoding(void)
{
    char		buf[80];
    int			nchar;

    nchar = utFormat(watt, buf, sizeof(buf)-1, asciiSymbolDef);
    CU_ASSERT_TRUE_FATAL(nchar > 0);
    CU_ASSERT_TRUE_FATAL(nchar < sizeof(buf));
    buf[nchar] = 0;
    CU_ASSERT_STRING_EQUAL(buf, "kg.m2.s-3");

    nchar = utFormat(watt, buf, sizeof(buf)-1, latin1SymbolDef);
    CU_ASSERT_TRUE_FATAL(nchar > 0);
    CU_ASSERT_TRUE_FATAL(nchar < sizeof(buf));
    buf[nchar] = 0;
    CU_ASSERT_STRING_EQUAL(buf, "m\xb2\xb7kg/s\xb3");

    nchar = utFormat(watt, buf, sizeof(buf)-1, utf8SymbolDef);
    CU_ASSERT_TRUE_FATAL(nchar > 0);
    CU_ASSERT_TRUE_FATAL(nchar < sizeof(buf));
    buf[nchar] = 0;
    CU_ASSERT_STRING_EQUAL(buf,
	"kg\xc2\xb7m\xc2\xb2\xc2\xb7s\xe2\x81\xbb\xc2\xb3");
}


static void
test_parsing(void)
{
    int	nchar;

    CU_ASSERT_PTR_NOT_NULL(utParse(unitSystem, "m", UT_ASCII, &nchar));
}


int
main(
    const int		argc,
    const char* const	argv)
{
    int	exitCode = EXIT_FAILURE;

    if (CU_initialize_registry() == CUE_SUCCESS) {
	CU_Suite*	testSuite = CU_add_suite(__FILE__, setup, teardown);

	if (testSuite != NULL) {
	    CU_ADD_TEST(testSuite, test_utNewBaseUnit);
	    CU_ADD_TEST(testSuite, test_utNewDimensionlessUnit);
	    CU_ADD_TEST(testSuite, test_utSize);
	    CU_ADD_TEST(testSuite, test_utGetUnitByName);
	    CU_ADD_TEST(testSuite, test_utGetUnitBySymbol);
	    CU_ADD_TEST(testSuite, test_utAddNamePrefix);
	    CU_ADD_TEST(testSuite, test_utAddSymbolPrefix);
	    CU_ADD_TEST(testSuite, test_utMapNameToUnit);
	    CU_ADD_TEST(testSuite, test_utScale);
	    CU_ADD_TEST(testSuite, test_utOffset);
	    CU_ADD_TEST(testSuite, test_utMultiply);
	    CU_ADD_TEST(testSuite, test_utInvert);
	    CU_ADD_TEST(testSuite, test_utDivide);
	    CU_ADD_TEST(testSuite, test_utRaise);
	    CU_ADD_TEST(testSuite, test_utLog);
	    CU_ADD_TEST(testSuite, test_utMapUnitToName);
	    CU_ADD_TEST(testSuite, test_utToString);
	    CU_ADD_TEST(testSuite, test_utGetConverter);
	    CU_ADD_TEST(testSuite, test_utOffsetByTime);
	    CU_ADD_TEST(testSuite, test_utSetEncoding);
	    CU_ADD_TEST(testSuite, test_parsing);
	    /*
	    */

	    if (CU_basic_run_tests() == CUE_SUCCESS) {
		if (CU_get_number_of_tests_failed() == 0)
		    exitCode = EXIT_SUCCESS;
	    }
	}

	CU_cleanup_registry();
    }

    return exitCode;
}
