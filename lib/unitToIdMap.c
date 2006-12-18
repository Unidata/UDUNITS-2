/*
 * Unit-to-identifier map.
 *
 * $Id: unitToIdMap.c,v 1.3 2006/12/18 18:03:19 steve Exp $
 */

/*LINTLIBRARY*/

#ifndef	_XOPEN_SOURCE
#   define _XOPEN_SOURCE 500
#endif

#include <assert.h>
#include <errno.h>
#include <search.h>
#include <stdlib.h>
#include <string.h>

#include "units.h"
#include "unitAndId.h"
#include "unitToIdMap.h"		/* this module's API */
#include "systemMap.h"

typedef struct {
    void*		ascii;
    void*		latin1;
    void*		utf8;
} UnitToIdMap;

static SystemMap*	systemToUnitToName = NULL;
static SystemMap*	systemToUnitToSymbol = NULL;

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
	for (; *string && ((*string & 0x80U) == 0); string++)
	    ;

	if (*string != 0)
	    *encoding = UT_LATIN1;
    }
    else if (*encoding == UT_LATIN1) {
	for (; *string && ((*string & 0x80U) == 0); string++)
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
 * Returns a new instance of a unit-to-identifier map.
 *
 * Returns:
 *	NULL	Failure.  See "errno".
 *	else	Pointer to a new unit-to-identifier map.
 */
static UnitToIdMap*
utimNew(void)
{
    UnitToIdMap* const	map = malloc(sizeof(UnitToIdMap));

    if (map != NULL) {
	map->ascii = NULL;
	map->latin1 = NULL;
	map->utf8 = NULL;
    }

    return map;
}


/*
 * Frees a unit-to-identifier map.  All entries in all encodings are freed.
 *
 * Arguments:
 *	map		Pointer to the map to be freed.
 */
static void
utimFree(
    UnitToIdMap*	map)
{
    if (map != NULL) {
	utEncoding	encodings[] = {UT_ASCII, UT_LATIN1, UT_UTF8};
	int		i;

	for (i = 0; i < sizeof(encodings)/sizeof(encodings[0]); ++i) {
	    void**	rootp = selectTree(map, encodings[i]);

	    while (*rootp != NULL) {
		UnitAndId*	uai = *(UnitAndId**)rootp;

		(void)tdelete(uai, rootp, compareUnits);
		uaiFree(uai);
	    }
	}

	free(map);
    }
}


/*
 * Adds an entry to a unit-to-identifier map.
 *
 * Arguments:
 *	map		Pointer to unit-to-identifier map.
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
	utHandleErrorMessage("NULL map argument");
	status = UT_INTERNAL;
    }
    else if (unit == NULL) {
	utHandleErrorMessage("NULL unit argument");
	status = UT_BADUNIT;
    }
    else if (id == NULL) {
	utHandleErrorMessage("NULL identifier argument");
	status = UT_BADID;
    }
    else if (adjustEncoding(&encoding, id)) {
	utHandleErrorMessage("Identifier not in given encoding");
	status = UT_BADID;
    }
    else {
	UnitAndId*	targetEntry = uaiNew(unit, id);

	if (targetEntry != NULL) {
	    UnitAndId**	treeEntry = tsearch(targetEntry, 
		selectTree(map, encoding), compareUnits);

	    if (treeEntry == NULL) {
		utHandleErrorMessage(strerror(errno));
		utHandleErrorMessage("Couldn't add search-tree entry");
		status = UT_OS;
	    }
	    else {
		if (strcmp((*treeEntry)->id, id) != 0) {
		    utHandleErrorMessage("Unit already maps to \"%s\"",
			(*treeEntry)->id);
		    status = UT_EXISTS;
		}
		else {
		    status = UT_SUCCESS;
		}
	    }

	    if (status != UT_SUCCESS)
		uaiFree(targetEntry);
	}				/* "targetEntry" allocated */
    }					/* valid arguments */

    return status;
}


/*
 * Removes an entry from a unit-to-identifier map.
 *
 * Arguments:
 *	map		Pointer to the unit-to-identifier map.
 *	unit		The unit.  May be freed upon return.
 *	encoding	The encoding to be removed.
 * Returns:
 *	UT_INTERNAL	"map" is NULL.
 *	UT_INTERNAL	"unit" is NULL.
 *	UT_SUCCESS	Success.
 */
static enum utStatus
utimRemove(
    UnitToIdMap* const	map,
    const utUnit*	unit,
    utEncoding		encoding)
{
    enum utStatus	status;

    if (map == NULL) {
	utHandleErrorMessage("NULL map argument");
	status = UT_INTERNAL;
    }
    else if (unit == NULL) {
	utHandleErrorMessage("NULL unit argument");
	status = UT_BADUNIT;
    }
    else {
	UnitAndId	targetEntry;
	UnitAndId**	treeEntry;

	targetEntry.unit = (utUnit*)unit;

	treeEntry =
	    tfind(&targetEntry, selectTree(map, encoding), compareUnits);

	if (treeEntry == NULL || *treeEntry == NULL) {
	    status = UT_SUCCESS;
	}
	else {
	    (void)tdelete(*treeEntry, selectTree(map, encoding),
		compareUnits);
	    uaiFree(*treeEntry);
	}
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
		utHandleErrorMessage(strerror(errno));
		utHandleErrorMessage(
		    "Couldn't convert identifier from ISO-8859-1 to UTF-8");
		utStatus = UT_OS;
		treeEntry = NULL;
	    }
	    else {
		UnitAndId*	newEntry = uaiNew(unit, id);

		if (newEntry != NULL) {
		    treeEntry = tsearch(newEntry, &map->utf8, compareUnits);

		    if (treeEntry == NULL) {
			uaiFree(newEntry);
			utHandleErrorMessage(strerror(errno));
			utHandleErrorMessage("Couldn't add unit-and-identifier "
			    "to search-tree");
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
 * Adds an entry to the unit-to-identifier map associated with a unit-system.
 *
 * Arguments:
 *	sytemMap	Address of the pointer to the
 *			system-to-unit-to-identifier map.
 *	unit		The unit.  May be freed upon return.
 *	id		The identifier.  May be freed upon return.
 *	encoding	The ostensible encoding of "id".
 * Returns:
 *	UT_INTERNAL	"systemMap" is NULL.
 *	UT_INTERNAL	"unit" is NULL.
 *	UT_BADID	"id" is NULL or inconsistent with "encoding".
 *	UT_OS		Operating-system error.  See "errno".
 *	UT_EXISTS	"unit" already maps to a different identifier.
 *	UT_SUCCESS	Success.
 */
static enum utStatus
mapUnitToId(
    SystemMap** const	systemMap,
    utUnit* const	unit,
    const char* const	id,
    utEncoding		encoding)
{
    enum utStatus	status;

    if (systemMap == NULL || unit == NULL || id == NULL) {
	status = UT_BADARG;
    }
    else {
	if (*systemMap == NULL) {
	    *systemMap = smNew();

	    if (*systemMap == NULL)
		status = UT_OS;
	}

	if (*systemMap != NULL) {
	    UnitToIdMap** const	unitToIdMap =
		(UnitToIdMap**)smSearch(*systemMap, utGetSystem(unit));

	    if (unitToIdMap == NULL) {
		status = UT_OS;
	    }
	    else {
		if (*unitToIdMap == NULL) {
		    *unitToIdMap = utimNew();

		    if (*unitToIdMap == NULL)
			status = UT_OS;
		}

		if (*unitToIdMap != NULL)
		    status = utimAdd(*unitToIdMap, unit, id, encoding);
	    }
	}
    }

    return status;
}


/*
 * Removes an entry from the unit-to-identifier map associated with a
 * unit-system.
 *
 * Arguments:
 *	sytemMap	Pointer to the system-to-unit-to-identifier map.
 *	unit		The unit.  May be freed upon return.
 *	encoding	The ostensible encoding of "id".
 * Returns:
 *	UT_BADARG	"systemMap" is NULL.
 *	UT_BADARG	"unit" is NULL.
 *	UT_SUCCESS	Success.
 */
static enum utStatus
unmapUnitToId(
    SystemMap* const	systemMap,
    utUnit* const	unit,
    utEncoding		encoding)
{
    enum utStatus	status;

    if (systemMap == NULL || unit == NULL) {
	status = UT_BADARG;
    }
    else {
	UnitToIdMap** const	unitToIdMap =
	    (UnitToIdMap**)smFind(systemMap, utGetSystem(unit));

	status =
	    (unitToIdMap == NULL || *unitToIdMap == NULL)
		? UT_SUCCESS
		: utimRemove(*unitToIdMap, unit, encoding);
    }

    return status;
}


/*
 * Returns the identifier in a given encoding to which a unit associated with
 * a unit-system maps.
 *
 * Arguments:
 *	systemMap	Pointer to the system-to-unit-to-id map.
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
    SystemMap* const	systemMap,
    const utUnit* const	unit,
    const utEncoding	encoding)
{
    const char*	id = NULL;		/* failure */

    if (unit == NULL) {
	utHandleErrorMessage("NULL unit argument");
	utStatus = UT_BADUNIT;
    }
    else {
	UnitToIdMap** const	unitToId = 
	    (UnitToIdMap**)smFind(systemMap, utGetSystem(unit));

	if (unitToId != NULL) {
	    UnitAndId*	mapEntry = 
		encoding == UT_LATIN1
		    ? utimFindLatin1ByUnit(*unitToId, unit)
		    : encoding == UT_UTF8
			? utimFindUtf8ByUnit(*unitToId, unit)
			: utimFindAsciiByUnit(*unitToId, unit);

	    if (mapEntry != NULL)
		id = mapEntry->id;
	}
    }

    return id;
}


/******************************************************************************
 * Public API:
 ******************************************************************************/


/*
 * Adds a mapping from a unit to a name.
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
    return utStatus = mapUnitToId(&systemToUnitToName, unit, name, encoding);
}


/*
 * Removes a mapping from a unit to a name.
 *
 * Arguments:
 *	unit		Pointer to the unit.  May be freed upon return.
 *	encoding	The encoding to be removed.
 * Returns:
 *	UT_BADUNIT	"unit" is NULL.
 *	UT_SUCCESS	Success.
 */
enum utStatus
utUnmapUnitToName(
    utUnit* const	unit,
    utEncoding		encoding)
{
    return utStatus = unmapUnitToId(systemToUnitToName, unit, encoding);
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
    return
	utStatus = mapUnitToId(&systemToUnitToSymbol, unit, symbol, encoding);
}


/*
 * Removes a mapping from a unit to a symbol.
 *
 * Arguments:
 *	unit		Pointer to the unit.  May be freed upon return.
 *	encoding	The encoding to be removed.
 * Returns:
 *	UT_BADUNIT	"unit" is NULL.
 *	UT_SUCCESS	Success.
 */
enum utStatus
utUnmapUnitToSymbol(
    utUnit* const	unit,
    utEncoding		encoding)
{
    return utStatus = unmapUnitToId(systemToUnitToSymbol, unit, encoding);
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

    return getId(systemToUnitToName, unit, encoding);
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

    return getId(systemToUnitToSymbol, unit, encoding);
}


/*
 * Frees resources associated with a unit-system.
 *
 * Arguments:
 *	system		Pointer to the unit-system to have its associated
 *			resources freed.
 */
void
utimFreeSystem(
    utSystem*	system)
{
    if (system != NULL) {
	SystemMap*	systemMaps[2];
	int		i;

	systemMaps[0] = systemToUnitToName;
	systemMaps[1] = systemToUnitToSymbol;

	for (i = 0; i < 2; i++) {
	    if (systemMaps[i] != NULL) {
		UnitToIdMap** const	unitToId =
		    (UnitToIdMap**)smFind(systemMaps[i], system);

		if (unitToId != NULL)
		    utimFree(*unitToId);

		smRemove(systemMaps[i], system);
	    }
	}
    }
}
