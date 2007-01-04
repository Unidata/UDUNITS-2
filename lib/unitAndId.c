/*
 * Searchable unit-and-identifier tree.
 *
 * $Id: unitAndId.c,v 1.5 2007/01/04 17:13:01 steve Exp $
 */

/*LINTLIBRARY*/

#ifndef	_XOPEN_SOURCE
#   define _XOPEN_SOURCE 500
#endif

#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "unitAndId.h"
#include "udunits2.h"


/*
 * Arguments:
 *	unit	The unit.  May be freed upon return.
 *	id	The identifier (name or symbol).  May be freed upon return.
 * Returns:
 *	NULL	Failure.  "utGetStatus()" will be
 *		    UT_NULL_ARG	"unit" or "id" is NULL.
 *		    UT_OS	Operating-system failure.  See "errno".
 *	else	Pointer to the new unit-and-identifier.
 */
UnitAndId*
uaiNew(
    const utUnit* const	unit,
    const char* const	id)
{
    UnitAndId*	entry = NULL;		/* failure */

    if (id == NULL || unit == NULL) {
	utHandleErrorMessage("uaiNew(): NULL argument");
	utSetStatus(UT_NULL_ARG);
    }
    else {
	entry = malloc(sizeof(UnitAndId));

	if (entry == NULL) {
	    utHandleErrorMessage(strerror(errno));
	    utHandleErrorMessage("Couldn't allocate %lu-byte data-structure",
		sizeof(UnitAndId));
	    utSetStatus(UT_OS);
	}
	else {
	    entry->id = strdup(id);

	    if (entry->id == NULL) {
		utHandleErrorMessage(strerror(errno));
		utHandleErrorMessage("Couldn't duplicate identifier");
		utSetStatus(UT_OS);
	    }
	    else {
		entry->unit = utClone(unit);

		if (entry->unit == NULL) {
		    assert(utGetStatus() != UT_SUCCESS);
		    free(entry->id);
		}
	    }

	    if (utGetStatus() != UT_SUCCESS) {
		free(entry);
		entry = NULL;
	    }
	}
    }

    return entry;
}


/*
 * Frees memory of a unit-and-identifier.
 *
 * Arguments:
 *	entry	Pointer to the unit-and-identifier or NULL.
 */
void
uaiFree(
    UnitAndId* const	entry)
{
    if (entry != NULL) {
	free(entry->id);
	utFree(entry->unit);
	free(entry);
    }
}
