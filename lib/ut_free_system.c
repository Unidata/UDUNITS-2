/*
 * $Id: ut_free_system.c,v 1.1 2007/04/11 20:28:18 steve Exp $
 */

/*LINTLIBRARY*/

#ifndef	_XOPEN_SOURCE
#   define _XOPEN_SOURCE 500
#endif

#include "udunits2.h"
#include "idToUnitMap.h"
#include "unitToIdMap.h"

extern void coreFreeSystem(ut_system* system);


/*
 * Frees a unit-system.  All unit-to-identifier and identifier-to-unit mappings
 * will be removed.
 *
 * Arguments:
 *	system		Pointer to the unit-system to be freed.  Use of "system"
 *			upon return results in undefined behavior.
 */
void
ut_free_system(
    ut_system*	system)
{
    if (system != NULL) {
	itumFreeSystem(system);
	utimFreeSystem(system);
	coreFreeSystem(system);
    }
}
