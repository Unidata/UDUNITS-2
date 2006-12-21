#ifndef UT_ID_TO_UNIT_MAP_H_INCLUDED
#define UT_ID_TO_UNIT_MAP_H_INCLUDED

#include "udunits2.h"


#ifdef __cplusplus
extern "C" {
#endif


/*
 * Frees resources associated with a unit-system.
 *
 * Arguments:
 *	system		Pointer to the unit-system to have its associated
 *			resources freed.
 */
void
itumFreeSystem(
    utSystem*	system);


#ifdef __cplusplus
}
#endif

#endif
