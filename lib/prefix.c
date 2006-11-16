/*
 * Module for handling unit prefixes -- both names and symbols.
 *
 * $Id: prefix.c,v 1.1 2006/11/16 20:21:06 steve Exp $
 */

/*LINTLIBRARY*/

#ifndef	_XOPEN_SOURCE
#   define _XOPEN_SOURCE 500
#endif

#include <ctype.h>
#include <search.h>
#include <stdlib.h>
#include <string.h>

#include "units.h"
#include "systemMap.h"

typedef struct {
    void*	tree;
    int		(*compare)(const void*, const void*);
} PrefixToValueMap;

typedef struct {
    void*	nextTree;
    double	value;
    size_t	position;
    int		character;
} PrefixSearchEntry;

static SystemMap*	systemToNameToValue = NULL;
static SystemMap*	systemToSymbolToValue = NULL;

extern utStatus		unitStatus;


/******************************************************************************
 * Prefix Search Entry:
 ******************************************************************************/


static PrefixSearchEntry*
pseNew(
    int			character,
    size_t		position)
{
    PrefixSearchEntry*	entry = malloc(sizeof(PrefixSearchEntry));

    if (entry == NULL) {
	unitStatus = UT_OS;
    }
    else {
	entry->character = character;
	entry->position = position;
	entry->value = 0;
	entry->nextTree = NULL;
    }

    return entry;
}


static void
pseFree(
    PrefixSearchEntry* const	entry)
{
    free(entry);
}


static int
pseSensitiveCompare(
    const void* const	entry1,
    const void* const	entry2)
{
    int	char1 = ((const PrefixSearchEntry*)entry1)->character;
    int	char2 = ((const PrefixSearchEntry*)entry2)->character;

    return char1 < char2 ? -1 : char1 == char2 ? 0 : 1;
}


static int
pseInsensitiveCompare(
    const void* const	entry1,
    const void* const	entry2)
{
    int	char1 = tolower(((const PrefixSearchEntry*)entry1)->character);
    int	char2 = tolower(((const PrefixSearchEntry*)entry2)->character);

    return char1 < char2 ? -1 : char1 == char2 ? 0 : 1;
}


/******************************************************************************
 * Prefix-to-Value Map:
 ******************************************************************************/


static PrefixToValueMap*
ptvmNew(
    int		(*compare)(const void*, const void*))
{
    PrefixToValueMap*	map =
	(PrefixToValueMap*)malloc(sizeof(PrefixToValueMap));

    if (map != NULL) {
	map->tree = NULL;
	map->compare = compare;
    }

    return map;
}


/*
 * Returns the prefix search-entry that matches an identifier.  Inserts a
 * new prefix search-entry if no matching element is found.  Note that the
 * returned entry might have a different prefix value if it was previously
 * inserted.
 *
 * Arguments:
 *	map		Pointer to the prefix-to-value map.
 *	id		The prefix identifier.  May be freed upon return.
 *	value		The prefix value.
 * Returns:
 *	NULL		"map" is NULL.
 *	NULL		"id" is NULL or the empty string.
 *	NULL		"value" is 0.
 *	NULL		Insufficient storage space is available.
 *	else		Pointer to the prefix-search-entry that matches "id".
 */
static const PrefixSearchEntry*
ptvmSearch(
    PrefixToValueMap*	map,
    const char* const	id,
    const double	value)
{
    PrefixSearchEntry*	entry = NULL;	/* failure */

    if (id != NULL && map != NULL && value != 0) {
	size_t	len = strlen(id);

	if (len > 0) {
	    size_t			i;
	    PrefixSearchEntry* const*	treeEntry = NULL;
	    void**			tree = &map->tree;

	    for (i = 0; i < len; i++) {
		PrefixSearchEntry* const	newEntry = pseNew(id[i], i);

		if (newEntry == NULL)
		    break;

		treeEntry = tsearch(newEntry, tree, map->compare);

		if (treeEntry == NULL) {
		    pseFree(newEntry);
		    break;
		}

		tree = &(*treeEntry)->nextTree;	/* next binary-search tree */

		if (newEntry != *treeEntry)
		    pseFree(newEntry);
	    }

	    if (i >= len) {
		entry = *treeEntry;

		if (entry->value == 0)
		    entry->value = value;
	    }
	}
    }

    return entry;
}


/******************************************************************************
 * Public API:
 ******************************************************************************/


/*
 * Adds a prefix to a unit-system.
 *
 * Arguments:
 *	system		Pointer to the unit-system.
 *	prefix		Pointer to the prefix (e.g., "mega", "M").  May be freed
 *			upon return.
 *	value		The value of the prefix (e.g., 1e6).
 *	systemMap	Pointer to system-map.
 *	compare		Prefix comparison function.
 * Returns:
 *	UT_SUCCESS	Success.
 *	UT_BADSYSTEM	"system" is NULL.
 *	UT_BADID	"prefix" is NULL or empty.
 *	UT_BADVALUE	"value" is 0.
 *	UT_EXISTS	"prefix" already maps to a different value.
 *	UT_OS		Operating-system failure.  See "errno".
 */
static utStatus
addPrefix(
    utSystem* const	system,
    const char* const	prefix,
    const double	value,
    SystemMap** const	systemMap,
    int			(*compare)(const void*, const void*))
{
    utStatus	status;

    if (system == NULL) {
	status = UT_BADSYSTEM;
    }
    else if (prefix == NULL || strlen(prefix) == 0) {
	status = UT_BADID;
    }
    else if (value == 0) {
	status = UT_BADVALUE;
    }
    else {
	if (*systemMap == NULL) {
	    *systemMap = smNew();

	    if (*systemMap == NULL)
		status = UT_OS;
	}

	if (*systemMap != NULL) {
	    PrefixToValueMap** const	prefixToValue =
		(PrefixToValueMap**)smSearch(*systemMap, system);

	    if (prefixToValue == NULL) {
		status = UT_OS;
	    }
	    else {
		if (*prefixToValue == NULL) {
		    *prefixToValue = ptvmNew(compare);

		    if (*prefixToValue == NULL)
			status = UT_OS;
		}

		if (*prefixToValue != NULL) {
		    const PrefixSearchEntry*	entry =
			ptvmSearch(*prefixToValue, prefix, value);

		    status = 
			entry == NULL
			    ? UT_OS
			    : (entry->value == value)
				? UT_SUCCESS
				: UT_EXISTS;
		}
	    }				/* have system-map entry */
	}				/* have system-map */
    }					/* valid arguments */

    return status;
}


/*
 * Adds a name-prefix to a unit-system.
 *
 * Arguments:
 *	system		Pointer to the unit-system.
 *	name		Pointer to the name-prefix (e.g., "mega").  May be freed
 *			upon return.
 *	value		The value of the prefix (e.g., 1e6).
 * Returns:
 *	UT_SUCCESS	Success.
 *	UT_BADSYSTEM	"system" is NULL.
 *	UT_BADID	"name" is NULL or empty.
 *	UT_BADVALUE	"value" is 0.
 *	UT_EXISTS	"name" already maps to a different value.
 *	UT_OS		Operating-system failure.  See "errno".
 */
utStatus
utAddNamePrefix(
    utSystem* const	system,
    const char* const	name,
    const double	value)
{
    return unitStatus = addPrefix(system, name, value, &systemToNameToValue,
	pseInsensitiveCompare);
}


/*
 * Adds a symbol-prefix to a unit-system.
 *
 * Arguments:
 *	system		Pointer to the unit-system.
 *	symbol		Pointer to the symbol-prefix (e.g., "M").  May be freed
 *			upon return.
 *	value		The value of the prefix (e.g., 1e6).
 * Returns:
 *	UT_SUCCESS	Success.
 *	UT_BADSYSTEM	"system" is NULL.
 *	UT_BADID	"symbol" is NULL or empty.
 *	UT_BADVALUE	"value" is 0.
 *	UT_EXISTS	"symbol" already maps to a different value.
 *	UT_OS		Operating-system failure.  See "errno".
 */
utStatus
utAddSymbolPrefix(
    utSystem* const	system,
    const char* const	symbol,
    const double	value)
{
    return unitStatus = addPrefix(system, symbol, value, &systemToSymbolToValue,
	pseSensitiveCompare);
}
