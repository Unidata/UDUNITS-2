#ifndef UT_SYSTEM_MAP_H_INCLUDED
#define UT_SYSTEM_MAP_H_INCLUDED

#include "units.h"

typedef struct SystemMap	SystemMap;

#ifdef __cplusplus
extern "C" {
#endif


/*
 * Returns a new instance of a system-map.
 *
 * Arguments:
 *	compare	Function for comparing keys.
 * Returns:
 *	NULL	Operating-system failure.  See "errno".
 *	else	Pointer to the new map.
 */
SystemMap*
smNew();


/*
 * Returns the address of the pointer to which a unit-system maps.
 *
 * Arguments:
 *	map	Pointer to the system-map.
 *	system	Pointer to the unit-system.
 * Returns:
 *	NULL	There is no pointer associated with "system".
 *	else	Address of the pointer to which "system" maps.
 */
void**
smFind(
    const SystemMap* const	map,
    const void* const		system);


/*
 * Returns the address of the pointer to which a unit-system maps -- creating a
 * new entry if necessary.  If a new entry is created, then the pointer whose
 * address is returned will be NULL.
 *
 * Arguments:
 *	map	Pointer to the system-map.
 *	system	Pointer to the unit-system.
 * Returns:
 *	NULL	Operating system failure.  See "errno".
 *	else	Address of the pointer to which "system" maps.
 */
void**
smSearch(
    SystemMap* const	map,
    const void*		system);


/*
 * Removes the system-map entry that corresponds to a unit-system.
 *
 * Arguments:
 *	map	Pointer to the map.
 *	system	Pointer to the unit-system.
 */
void
smRemove(
    SystemMap* const	map,
    const void* const	system);


/*
 * Frees a system-map.  This function should be called when a system-map is no
 * longer needed.
 *
 * Arguments:
 *	map	Pointer to the system-map to be freed or NULL.  Use of "map"
 *		upon return results in undefined behavior.
 */
void
smFree(
    SystemMap* const	map);


#ifdef __cplusplus
}
#endif

#endif
