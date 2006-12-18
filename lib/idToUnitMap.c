/*
 * Identifier-to-unit map.
 *
 * $Id: idToUnitMap.c,v 1.3 2006/12/18 18:03:18 steve Exp $
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
 * Frees an identifier-to-unit map.  All entries are freed.
 *
 * Arguments:
 *	map		Pointer to the identifier-to-unit map.
 * Returns:
 */
static void
itumFree(
    IdToUnitMap*	map)
{
    if (map != NULL) {
	while (map->tree != NULL) {
	    UnitAndId*	uai = *(UnitAndId**)map->tree;

	    (void)tdelete(uai, &map->tree, map->compare);
	    uaiFree(uai);
	}

	free(map);
    }					/* valid arguments */
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
		if (utCompare((*treeEntry)->unit, unit) == 0) {
		    status = UT_SUCCESS;
		}
		else {
		    utHandleErrorMessage(
			"\"%s\" maps to existing but different unit", id);
		    status = UT_EXISTS;
		}
	    }				/* found entry */

	    if (status != UT_SUCCESS)
		uaiFree(targetEntry);
	}				/* "targetEntry" allocated */
    }					/* valid arguments */

    return status;
}


/*
 * Removes an entry to an identifier-to-unit map.
 *
 * Arguments:
 *	map		The database.
 *	id		The identifier.  May be freed upon return.
 * Returns:
 *	UT_INTERNAL	"map" is NULL.
 *	UT_BADID	"id" is NULL.
 *	UT_SUCCESS	Success.
 */
static enum utStatus
itumRemove(
    IdToUnitMap*	map,
    const char* const	id)
{
    enum utStatus	status;

    if (map == NULL) {
	status = UT_INTERNAL;
    }
    else if (id == NULL) {
	status = UT_BADID;
    }
    else {
	UnitAndId	targetEntry;
	UnitAndId**	treeEntry;
	
	targetEntry.id = (char*)id;

	treeEntry = tfind(&targetEntry, &map->tree, map->compare);

	if (treeEntry != NULL) {
	    (void)tdelete(*treeEntry, &map->tree, map->compare);
	    uaiFree(*treeEntry);
	}
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
	utHandleErrorMessage("itumFind(): NULL map argument");
	utStatus = UT_INTERNAL;
    }
    else if (id == NULL) {
	utHandleErrorMessage("itumFind(): NULL identifierargument");
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
 * Removes the mapping from an identifier to a unit.
 *
 * Arguments:
 *	systemMap	Address of the pointer to the system-map.
 *	id		Pointer to the identifier.  May be freed upon return.
 *	system		Pointer to the unit-system associated with the mapping.
 * Returns:
 *	UT_BADARG	"id" is NULL.
 *	UT_BADARG	"system" is NULL.
 *	UT_BADARG	"compare" is NULL.
 *	UT_SUCCESS	Success.
 */
static enum utStatus
unmapId(
    SystemMap* const	systemMap,
    const char* const	id,
    utSystem*		system)
{
    enum utStatus	status;

    if (systemMap == NULL || id == NULL || system == NULL) {
	status = UT_BADARG;
    }
    else {
	IdToUnitMap** const	idToUnit =
	    (IdToUnitMap**)smFind(systemMap, system);

	status = 
	    (idToUnit == NULL || *idToUnit == NULL)
		? UT_SUCCESS
		: itumRemove(*idToUnit, id);
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
 * Removes a mapping from a name to a unit.  After this function,
 * getUnitByName() will no longer return a unit.
 *
 * Arguments:
 *	name		The name of the unit.
 *	system		The unit-system to which the unit belongs.
 * Returns:
 *	UT_SUCCESS	Success.
 */
enum utStatus
utUnmapName(
    const char* const	name,
    utSystem*		system)
{
    return utStatus =
	unmapId(systemToNameToUnit, name, system);
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
 * Removes a mapping from a symbol to a unit.  After this function,
 * getUnitBySymbol() will no longer return a unit.
 *
 * Arguments:
 *	symbol		The symbol of the unit.
 *	system		The unit-system to which the unit belongs.
 * Returns:
 *	UT_SUCCESS	Success.
 */
enum utStatus
utUnmapSymbol(
    const char* const	symbol,
    utSystem*		system)
{
    return utStatus =
	unmapId(systemToSymbolToUnit, symbol, system);
}


/*
 * Returns the unit to which an identifier maps in a particular unit-system.
 *
 * Arguments:
 *	systemMap	NULL or pointer to the system-map.  If NULL, then
 *			NULL will be returned.
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

    if (system == NULL) {
	utHandleErrorMessage("getUnitById(): NULL unit-system argument");
	utStatus = UT_BADSYSTEM;
    }
    else if (id == NULL) {
	utHandleErrorMessage("getUnitById(): NULL identifier argument");
	utStatus = UT_BADID;
    }
    else if (systemMap != NULL) {
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


/*
 * Frees resources associated with a unit-system.
 *
 * Arguments:
 *	system		Pointer to the unit-system to have its associated
 *			resources freed.
 */
void
itumFreeSystem(
    utSystem*	system)
{
    if (system != NULL) {
	SystemMap*	systemMaps[2];
	int		i;

	systemMaps[0] = systemToNameToUnit;
	systemMaps[1] = systemToSymbolToUnit;

	for (i = 0; i < 2; i++) {
	    if (systemMaps[i] != NULL) {
		IdToUnitMap** const	idToUnit =
		    (IdToUnitMap**)smFind(systemMaps[i], system);

		if (idToUnit != NULL)
		    itumFree(*idToUnit);

		smRemove(systemMaps[i], system);
	    }
	}
    }					/* valid arguments */
}
