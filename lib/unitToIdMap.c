/*
 * Unit-to-identifier map.
 *
 * $Id: unitToIdMap.c,v 1.2 2006/12/02 22:33:48 steve Exp $
 */

/*LINTLIBRARY*/

#ifndef	_XOPEN_SOURCE
#   define _XOPEN_SOURCE 500
#endif

#include <assert.h>
#include <search.h>
#include <stdlib.h>
#include <string.h>

#include "units.h"
#include "unitAndId.h"
#include "unitToIdMap.h"		/* this module's API */

typedef struct {
    void*		ascii;
    void*		latin1;
    void*		utf8;
} UnitToIdMap;

static UnitToIdMap	unitToNameMap;
static UnitToIdMap	unitToSymbolMap;

extern enum utStatus	utStatus;


/******************************************************************************
 * Miscellaneous Functions:
 ******************************************************************************/

/*
 * Unconditionally converts an ISO Latin-1 string into a UTF-8 string.
 *
 * Arguments:
 *	latin1String	Pointer to the ISO Latin-1 string.
 * Returns:
 *	NULL		Failure.  See errno.
 *	else		Pointer to the equivalent UTF-8 string.  Should be freed
 *			when no longer needed.
 */
static char*
latin1ToUtf8(
    const char* const	latin1String)
{
    int			nchar;
    const char*		inp;
    char*		outp;
    char*		utf8String;

    for (nchar = 0, inp = latin1String; *inp; ++inp, ++nchar)
	if ((*inp & 0x80U) != 0)
	    nchar++;

    utf8String = malloc(nchar+1);

    if (utf8String != NULL) {
	for (inp = latin1String, outp = utf8String; *inp; ++inp, ++outp) {
	    if ((*inp & 0x80U) == 0) {
		*outp = *inp;
	    }
	    else {
		*outp++ = (char)(0xC0U | ((unsigned)*inp >> 6));
		*outp = (char)(0x80U | (*inp & 0x3FU));
	    }
	}

	*outp = 0;
    }

    return utf8String;
}

/*
 * Adjust a given encoding according to a string.  Because ASCII is a subset of
 * ISO Latin-1 and because a UTF-8 encoded string must follow certain rules,
 * it's possible for strings to be mis-encoded and for an encoding specification
 * to be too restrictive or over-generous.  If the encoding specification is
 * ASCII and the given string contains a character with the high-order bit
 * set, then the encoding will be changed to ISO Latin-1.  If the encoding
 * specification is ISO Latin-1 but the string doesn't contain a character with
 * the high-order bit set, then the encoding will be changed to ASCII.  If the
 * encoding specification is UTF-8 but the string doesn't follow the UTF-8
 * encoding rules, then the function will error-return.
 *
 * Arguments:
 *	encoding	Pointer to the presumptive encoding.  Might be modified
 *			on return to reflect the actual, most restrictive,
 *			encoding of "string".
 *	string		The string to be checked.
 * Returns:
 *	0		Success.  "*encoding" might be modified.
 *	-1		Failure.  "string" doesn't conform to "encoding".
 */
static int
adjustEncoding(
    utEncoding* const	encoding,
    const char* 	string)
{
    int		status = 0;		/* success */

    if (*encoding == UT_ASCII) {
	while (*string && ((*string++ & 0x80U) == 0))
	    ;

	if (*string)
	    *encoding = UT_LATIN1;
    }
    else if (*encoding == UT_LATIN1) {
	while (*string && ((*string++ & 0x80U) == 0))
	    ;

	if (*string == 0)
	    *encoding = UT_ASCII;
    }
    else if (*encoding == UT_UTF8) {
	for (; *string; string++) {
	    if (*string & 0x80U) {
		if ((*string & 0xE0U) == 0xC0U) {
		    if ((*++string & 0xC0U) != 0x80U)
			break;
		}
		else if ((*string & 0xF0U) == 0xE0U) {
		    if ((*++string & 0xC0U) != 0x80U)
			break;
		    if ((*++string & 0xC0U) != 0x80U)
			break;
		}
		else if ((*string & 0xF8U) == 0xF0U) {
		    if ((*++string & 0xC0U) != 0x80U)
			break;
		    if ((*++string & 0xC0U) != 0x80U)
			break;
		    if ((*++string & 0xC0U) != 0x80U)
			break;
		}
	    }
	}

	if (*string)
	    status = -1;
    }

    return status;
}


/******************************************************************************
 * Internal Map Functions:
 ******************************************************************************/

static int
compareUnits(
    const void* const	entry1,
    const void* const	entry2)
{
    return utCompare(((const UnitAndId*)entry1)->unit,
	((const UnitAndId*)entry2)->unit);
}


/*
 * Selects a unit-and-identifier tree corresponding to a given encoding.
 *
 * Arguments:
 *	map		The unit-to-id map.
 *	encoding	The encoding.
 * Returns:
 *	Pointer to the root of the unit-and-identifier tree in "map" that
 *	corresponds to "encoding".
 */
static void**
selectTree(
    UnitToIdMap* const	unitToIdMap,
    const utEncoding	encoding)
{
    return
	encoding == UT_ASCII
	    ? &unitToIdMap->ascii
	    : encoding == UT_LATIN1
		? &unitToIdMap->latin1
		: &unitToIdMap->utf8;
}


/*
 * Adds an entry to a unit-to-identifier map.
 *
 * Arguments:
 *	map		The unit-to-identifier map.
 *	unit		The unit.  May be freed upon return.
 *	id		The identifier.  May be freed upon return.
 *	encoding	The ostensible encoding of "id".
 * Returns:
 *	UT_INTERNAL	"map" is NULL.
 *	UT_INTERNAL	"unit" is NULL.
 *	UT_BADID	"id" is NULL or inconsistent with "encoding".
 *	UT_OS		Operating-system error.  See "errno".
 *	UT_EXISTS	"unit" already maps to a different identifier.
 *	UT_SUCCESS	Success.
 */
static enum utStatus
utimAdd(
    UnitToIdMap* const	map,
    const utUnit*	unit,
    const char* const	id,
    utEncoding		encoding)
{
    enum utStatus	status;

    if (map == NULL) {
	status = UT_INTERNAL;
    }
    else if (unit == NULL) {
	status = UT_BADUNIT;
    }
    else if (id == NULL) {
	status = UT_BADID;
    }
    else if (adjustEncoding(&encoding, id)) {
	status = UT_BADID;
    }
    else {
	UnitAndId*	targetEntry = uaiNew(unit, id);

	if (targetEntry != NULL) {
	    UnitAndId**	treeEntry = tsearch(targetEntry, 
		selectTree(map, encoding), compareUnits);

	    if (treeEntry == NULL) {
		status = UT_OS;
	    }
	    else {
		status = 
		    strcmp((*treeEntry)->id, id) != 0
			? UT_EXISTS
			: UT_SUCCESS;
	    }

	    if (status != UT_SUCCESS)
		uaiFree(targetEntry);
	}				/* "targetEntry" allocated */
    }					/* valid arguments */

    return status;
}


/*
 * Returns the unit-and-identifier whose ASCII identifier corresponding to a
 * unit.
 *
 * Arguments:
 *	map	The unit-to-identifier map.
 *	unit	The unit to be used as the key in the search.
 * Returns:
 *	NULL	The map doesn't contain an entry corresponding to "unit" whose
 *		identifier is in ASCII.
 *	else	Pointer to the entry corresponding to "unit" whose identifier is
 *		in ASCII.
 */
static UnitAndId*
utimFindAsciiByUnit(
    UnitToIdMap* const	map,
    const utUnit* const	unit)
{
    UnitAndId	targetEntry;
    UnitAndId**	treeEntry;

    targetEntry.unit = (utUnit*)unit;
    treeEntry = tfind(&targetEntry, &map->ascii, compareUnits);

    return treeEntry == NULL ? NULL : *treeEntry;
}


/*
 * Finds a unit-search-entry with a Latin-1 identifier correcponding to a unit.
 *
 * Arguments:
 *	map	The unit-to-identifier map.
 *	unit	The unit to be used as the key in the search.
 * Returns:
 *	NULL	The map doesn't contain an entry corresponding to "unit" whose
 *		identifier is in Latin-1.
 *	else	Pointer to the entry corresponding to "unit" whose identifier is
 *		in Latin-1 (and might, actually, be in ASCII).
 */
static UnitAndId*
utimFindLatin1ByUnit(
    UnitToIdMap* const	map,
    const utUnit* const	unit)
{
    UnitAndId	targetEntry;
    UnitAndId**	treeEntry;

    targetEntry.unit = (utUnit*)unit;
    treeEntry = tfind(&targetEntry, &map->latin1, compareUnits);

    if (treeEntry == NULL)
	treeEntry = tfind(&targetEntry, &map->ascii, compareUnits);

    return treeEntry == NULL ? NULL : *treeEntry;
}


/*
 * Finds a unit-search-entry with a UTF-8 identifier correcponding to a unit.
 *
 * Arguments:
 *	map	The unit-to-identifier map.
 *	unit	The unit to be used as the key in the search.
 * Returns:
 *	NULL	The map doesn't contain an entry corresponding to "unit" whose
 *		identifier is in UTF-8.
 *	else	Pointer to the entry corresponding to "unit" whose identifier is
 *		in UTF-8 (and might, actually, be in ASCII).
 */
static UnitAndId*
utimFindUtf8ByUnit(
    UnitToIdMap* const	map,
    const utUnit* const	unit)
{
    UnitAndId	targetEntry;
    UnitAndId**	treeEntry = NULL;	/* failure */

    targetEntry.unit = (utUnit*)unit;
    treeEntry = tfind(&targetEntry, &map->utf8, compareUnits);

    if (treeEntry == NULL) {
	treeEntry = tfind(&targetEntry, &map->latin1, compareUnits);

	if (treeEntry == NULL) {
	    treeEntry = tfind(&targetEntry, &map->ascii, compareUnits);
	}
	else {
	    /*
	     * Create the UTF-8 version of the Latin-1 identifier and add it to
	     * the UTF-8 unit-to-id map so that it will be found next time.
	     */
	    char* const	id = latin1ToUtf8((*treeEntry)->id);

	    if (id == NULL) {
		utStatus = UT_OS;
		treeEntry = NULL;
	    }
	    else {
		UnitAndId*	newEntry = uaiNew(unit, id);

		if (newEntry != NULL) {
		    treeEntry = tsearch(newEntry, &map->utf8, compareUnits);

		    if (treeEntry == NULL) {
			uaiFree(newEntry);
			utStatus = UT_OS;
		    }
		}

		free(id);
	    }				/* UTF-8 identifier created */
	}				/* found Latin-1 identifier */
    }					/* no UTF-8 identifier */

    return treeEntry == NULL ? NULL : *treeEntry;
}


/*
 * Returns the identifier in a given encoding to which a unit maps.
 *
 * Arguments:
 *	unit		Pointer to the unit whose identifier should be returned.
 *	encoding	The desired encoding of the identifier.
 * Returns:
 *	NULL		Failure.  "utStatus" will be
 *			    UT_BADUNIT	"unit" was NULL.
 *	else		Pointer to the identifier in the given encoding
 *			associated with "unit".
 */
static const char*
getId(
    UnitToIdMap* const	map,
    const utUnit* const	unit,
    const utEncoding	encoding)
{
    const char*	id = NULL;		/* failure */

    if (unit == NULL) {
	utStatus = UT_BADUNIT;
    }
    else {
	UnitAndId*	mapEntry = 
	    encoding == UT_LATIN1
		? mapEntry = utimFindLatin1ByUnit(map, unit)
		: encoding == UT_UTF8
		    ? mapEntry = utimFindUtf8ByUnit(map, unit)
		    : utimFindAsciiByUnit(map, unit);

	if (mapEntry != NULL)
	    id = mapEntry->id;
    }

    return id;
}


/******************************************************************************
 * Public API:
 ******************************************************************************/

/*
 * Adds to a mapping from a unit to a name.
 *
 * Arguments:
 *	unit		Pointer to the unit.  May be freed upon return.
 *	name		Pointer to the name.  May be freed upon return.
 *	encoding	The encoding of "name".
 * Returns:
 *	UT_BADUNIT	"unit" is NULL.
 *	UT_BADID	"name" is NULL or not in the specified encoding.
 *	UT_OS		Operating-system error.  See "errno".
 *	UT_EXISTS	"unit" already maps to a name.
 *	UT_SUCCESS	Success.
 */
enum utStatus
utMapUnitToName(
    utUnit* const	unit,
    const char* const	name,
    utEncoding		encoding)
{
    return utStatus = utimAdd(&unitToNameMap, unit, name, encoding);
}


/*
 * Adds to a unit-system a mapping from a unit to a symbol.
 *
 * Arguments:
 *	unit		Pointer to the unit.  May be freed upon return.
 *	symbol		Pointer to the symbol.  May be freed upon return.
 *	encoding	The encoding of "symbol".
 * Returns:
 *	UT_BADUNIT	"unit" is NULL.
 *	UT_BADID	"symbol" is NULL.
 *	UT_OS		Operating-system error.  See "errno".
 *	UT_EXISTS	"unit" already maps to a symbol.
 *	UT_SUCCESS	Success.
 */
enum utStatus
utMapUnitToSymbol(
    utUnit*		unit,
    const char* const	symbol,
    utEncoding		encoding)
{
    return utStatus = utimAdd(&unitToSymbolMap, unit, symbol, encoding);
}


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
    const utEncoding	encoding)
{
    utStatus = UT_SUCCESS;

    return getId(&unitToNameMap, unit, encoding);
}


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
    const utEncoding	encoding)
{
    utStatus = UT_SUCCESS;

    return getId(&unitToSymbolMap, unit, encoding);
}
