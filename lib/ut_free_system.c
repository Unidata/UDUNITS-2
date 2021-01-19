/**
 * This file implements a function to free a unit system for version 2 of the
 * Unidata UDUNITS package.
 *
 *  @file:  ut_free_system.c
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

/*LINTLIBRARY*/

#include "config.h"

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
