/*
 * Identifier-to-unit map.
 *
 * $Id: idToUnitMap.c,v 1.2 2006/12/02 22:33:46 steve Exp $
 */

/*LINTLIBRARY*/

#ifndef	_XOPEN_SOURCE
#   define _XOPEN_SOURCE 500
#endif

#include <search.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "units.h"
#include "unitAndId.h"
#include "systemMap.h"

extern enum utStatus	utStatus;

typedef struct {
    int			(*compare)(const void*, const void*);
    void*		tree;
} IdToUnitMap;

static SystemMap*	systemToNameToUnit;
static SystemMap*	systemToSymbolToUnit;


static int
sensitiveCompare(
    const void* const	node1,
    const void* const	node2)
{
    return strcmp(((const UnitAndId*)node1)->id, 
	((const UnitAndId*)node2)->id);
}


static int
insensitiveCompare(
    const void* const	node1,
    const void* const	node2)
{
    return strcasecmp(((const UnitAndId*)node1)->id, 
	((const UnitAndId*)node2)->id);
}


static IdToUnitMap*
itumNew(
    int		(*compare)(const void*, const void*))
{
    IdToUnitMap*	map = (IdToUnitMap*)malloc(sizeof(IdToUnitMap));

    if (map != NULL) {
	map->tree = NULL;
	map->compare = compare;
    }

    return map;
}


/*
 * Adds an entry to an identifier-to-unit map.
 *
 * Arguments:
 *	map		The database.
 *	id		The identifier.  May be freed upon return.
 *	unit		The unit.  May be freed upon return.
 * Returns:
 *	UT_INTERNAL	"map" is NULL.
 *	UT_BADUNIT	"unit" is NULL.
 *	UT_BADID	"id" is NULL.
 *	UT_OS		Operating-system error.  See "errno".
 *	UT_EXISTS	"id" already maps to a different unit.
 *	UT_SUCCESS	Success.
 */
static enum utStatus
itumAdd(
    IdToUnitMap*	map,
    const char* const	id,
    utUnit* const	unit)
{
    enum utStatus	status;

    if (map == NULL) {
	status = UT_INTERNAL;
    }
    else if (id == NULL) {
	status = UT_BADID;
    }
    else if (unit == NULL) {
	status = UT_BADUNIT;
    }
    else {
	UnitAndId*	targetEntry = uaiNew(unit, id);

	if (targetEntry != NULL) {
	    UnitAndId**	treeEntry = tsearch(targetEntry, &map->tree,
		map->compare);

	    if (treeEntry == NULL) {
		status = UT_OS;
	    }
	    else {
		status = 
		    utCompare((*treeEntry)->unit, unit) != 0
			? UT_EXISTS
			: UT_SUCCESS;
	    }				/* found entry */

	    if (status != UT_SUCCESS)
		uaiFree(targetEntry);
	}				/* "targetEntry" allocated */
    }					/* valid arguments */

    return status;
}


/*
 * Finds the entry in an identifier-to-unit map that corresponds to an
 * identifer.
 *
 * Arguments:
 *	map	The identifier-to-unit map.
 *	id	The identifier to be used as the key in the search.
 * Returns:
 *	NULL	Failure.  "utStatus" is set:
 *		    UT_INTERNAL	"map" is NULL.
 *		    UT_BADID	"id" is NULL.
 *		    else	"map" doesn't contain an entry that corresponds
 *				to "id".
 *	else	Pointer to the entry corresponding to "id".
 */
static const UnitAndId*
itumFind(
    IdToUnitMap*	map,
    const char* const	id)
{
    UnitAndId*	entry = NULL;		/* failure */

    if (map == NULL) {
	utStatus = UT_INTERNAL;
    }
    else if (id == NULL) {
	utStatus = UT_BADID;
    }
    else {
	UnitAndId	targetEntry;
	UnitAndId**	treeEntry;

	targetEntry.id = (char*)id;
	treeEntry = tfind(&targetEntry, &map->tree, map->compare);

	if (treeEntry != NULL)
	    entry = *treeEntry;
    }

    return entry;
}


/*
 * Adds to a particular unit-system a mapping from an identifier to a unit.
 *
 * Arguments:
 *	systemMap	Address of the pointer to the system-map.
 *	id		Pointer to the identifier.  May be freed upon return.
 *	unit		Pointer to the unit.  May be freed upon return.
 *	compare		Pointer to comparison function for unit-identifiers.
 * Returns:
 *	UT_BADID	"id" is NULL.
 *	UT_BADUNIT	"unit" is NULL.
 *	UT_OS		Operating-sytem failure.  See "errno".
 *	UT_SUCCESS	Success.
 */
static enum utStatus
mapIdToUnit(
    SystemMap** const	systemMap,
    const char* const	id,
    utUnit* const	unit,
    int			(*compare)(const void*, const void*))
{
    enum utStatus	status = UT_SUCCESS;

    if (id == NULL) {
	status = UT_BADID;
    }
    else if (unit == NULL) {
	status = UT_BADUNIT;
    }
    else {
	utSystem*	system = utGetSystem(unit);

	if (*systemMap == NULL) {
	    *systemMap = smNew();

	    if (*systemMap == NULL)
		status = UT_OS;
	}

	if (*systemMap != NULL) {
	    IdToUnitMap** const	idToUnit =
		(IdToUnitMap**)smSearch(*systemMap, system);

	    if (idToUnit == NULL) {
		status = UT_OS;
	    }
	    else {
		if (*idToUnit == NULL) {
		    *idToUnit = itumNew(compare);

		    if (*idToUnit == NULL)
			status = UT_OS;
		}

		if (*idToUnit != NULL)
		    status = itumAdd(*idToUnit, id, unit);
	    }				/* have system-map entry */
	}				/* have system-map */
    }					/* valid arguments */

    return status;
}


/*
 * Adds a mapping from a name to a unit.
 *
 * Arguments:
 *	name	Pointer to the name.  May be freed upon return.
 *	unit	Pointer to the unit.  May be freed upon return.
 * Returns:
 *	UT_BADID	"name" is NULL.
 *	UT_BADUNIT	"unit" is NULL.
 *	UT_OS		Operating-system error.  See "errno".
 *	UT_EXISTS	"name" already maps to a different unit.
 *	UT_SUCCESS	Success.
 */
enum utStatus
utMapNameToUnit(
    const char* const	name,
    utUnit* const	unit)
{
    return utStatus =
	mapIdToUnit(&systemToNameToUnit, name, unit, insensitiveCompare);
}


/*
 * Adds a mapping from a symbol to a unit.
 *
 * Arguments:
 *	symbol	Pointer to the symbol.  May be freed upon return.
 *	unit	Pointer to the unit.  May be freed upon return.
 * Returns:
 *	UT_BADID	"symbol" is NULL.
 *	UT_BADUNIT	"unit" is NULL.
 *	UT_OS		Operating-system error.  See "errno".
 *	UT_EXISTS	"symbol" already maps to a different unit.
 *	UT_SUCCESS	Success.
 */
enum utStatus
utMapSymbolToUnit(
    const char* const	symbol,
    utUnit* const	unit)
{
    return utStatus =
	mapIdToUnit(&systemToSymbolToUnit, symbol, unit, sensitiveCompare);
}


/*
 * Returns the unit to which an identifier maps in a particular unit-system.
 *
 * Arguments:
 *	systemMap	Address of the pointer to the system-map.
 *	system		Pointer to the unit-system.
 *	id		Pointer to the identifier.
 * Returns:
 *	NULL	Failure.  "utStatus" will be:
 *		    UT_BADSYSTEM	"system" is NULL.
 *		    UT_BADID		"id" is NULL.
 *	else	Pointer to the unit in "system" with the identifier "id".
 *		Should be passed to utFree() when no longer needed.
 */
static utUnit*
getUnitById(
    SystemMap* const	systemMap,
    utSystem* const	system,
    const char* const	id)
{
    utUnit*	unit = NULL;		/* failure */

    if (systemMap == NULL) {
	utStatus = UT_INTERNAL;
    }
    else if (system == NULL) {
	utStatus = UT_BADSYSTEM;
    }
    else if (id == NULL) {
	utStatus = UT_BADID;
    }
    else {
	IdToUnitMap** const	idToUnit =
	    (IdToUnitMap**)smFind(systemMap, system);

	if (idToUnit != NULL) {
	    const UnitAndId*	uai = itumFind(*idToUnit, id);

	    if (uai != NULL)
		unit = utClone(uai->unit);
	}
    }					/* valid arguments */

    return unit;
}


/*
 * Returns the unit with a given name from a unit-system.  Name comparisons
 * are case-insensitive.
 *
 * Arguments:
 *	system	Pointer to the unit-system.
 *	name	Pointer to the name of the unit to be returned.
 * Returns:
 *	NULL	Failure.  "utGetStatus()" will be
 *		    UT_SUCCESS		There is no unit in "system" with
 *					name "name".
 *		    UT_BADSYSTEM	"system" is NULL.
 *		    UT_BADID		"name" is NULL.
 *	else	Pointer to the unit in the unit-system with the given name.
 *		Should be passed to utFree() when no longer needed.
 */
utUnit*
utGetUnitByName(
    utSystem* const	system,
    const char* const	name)
{
    utStatus = UT_SUCCESS;

    return getUnitById(systemToNameToUnit, system, name);
}


/*
 * Returns the unit in a unit-system that has a given symbol.  Symbol 
 * comparisons are case-sensitive.
 *
 * Arguments:
 *	system	Pointer to the unit-system.
 *	symbol	Pointer to the symbol associated with the unit to be returned.
 * Returns:
 *	NULL	Failure.  "utGetStatus()" will be
 *		    UT_SUCCESS		There is no unit in "system" with
 *					symbol "symbol".
 *		    UT_BADSYSTEM	"system" is NULL.
 *		    UT_BADID		"symbol" is NULL.
 *	else	Pointer to the unit in the unit-system with the given name.
 *		Should be passed to utFree() when no longer needed.
 */
utUnit*
utGetUnitBySymbol(
    utSystem* const	system,
    const char* const	symbol)
{
    utStatus = UT_SUCCESS;

    return getUnitById(systemToSymbolToUnit, system, symbol);
}
