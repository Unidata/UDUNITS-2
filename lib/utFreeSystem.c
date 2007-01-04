/*
 * $Id: utFreeSystem.c,v 1.1 2007/01/04 17:15:40 steve Exp $
 */

/*LINTLIBRARY*/

#ifndef	_XOPEN_SOURCE
#   define _XOPEN_SOURCE 500
#endif

#include "udunits2.h"
#include "idToUnitMap.h"
#include "unitToIdMap.h"

extern void coreFreeSystem(utSystem* system);


/*
 * Frees a unit-system.  All unit-to-identifier and identifier-to-unit mappings
 * will be removed.
 *
 * Arguments:
 *	system		Pointer to the unit-system to be freed.  Use of "system"
 *			upon return results in undefined behavior.
 */
void
utFreeSystem(
    utSystem*	system)
{
    if (system != NULL) {
	itumFreeSystem(system);
	utimFreeSystem(system);
	coreFreeSystem(system);
    }
}
