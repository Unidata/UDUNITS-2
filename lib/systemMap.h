/**
 * This file declares the API for mapping from unit systems to their associated
 * pointers for version 2 of the Unidata UDUNITS package.
 *
 *  @file:  systemMap.h
 * @author: Steven R. Emmerson
 *
 *    Copyright 2021 University Corporation for Atmospheric Research
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef UT_SYSTEM_MAP_H_INCLUDED
#define UT_SYSTEM_MAP_H_INCLUDED

#include "udunits2.h"

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
