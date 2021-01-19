/**
 * This file declares the API of a unit-to-identifier map for version 2 of the
 * Unidata UDUNITS package.
 *
 *  @file:  unitToIdMap.h
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

#ifndef UT_UNIT_TO_ID_MAP_H_INCLUDED
#define UT_UNIT_TO_ID_MAP_H_INCLUDED

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
EXTERNL void
utimFreeSystem(
    ut_system*	system);


#ifdef __cplusplus
}
#endif

#endif
