/**
 * This file declares the API for a searchable unit-and-identifier tree for
 * version 2 of the Unidata UDUNITS package.
 *
 *  @file:  unitAndId.h
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

#ifndef UT_UNIT_SEARCH_NODE_H_INCLUDED
#define UT_UNIT_SEARCH_NODE_H_INCLUDED

#include "udunits2.h"

typedef struct {
    char*	id;
    ut_unit*	unit;
} UnitAndId;

#ifdef __cplusplus
extern "C" {
#endif


/*
 * Arguments:
 *	id	The identifier (name or symbol).  May be freed upon return.
 *	unit	The unit.  Must not be freed upon successful return until the
 *		returned unit-search-node is no longer needed.
 * Returns:
 *	NULL	"id" is NULL.
 *	NULL	"node" is NULL.
 *	NULL	Out of memory.
 *	else	Pointer to the new unit search node.
 */
UnitAndId*
uaiNew(
    const ut_unit* const	unit,
    const char* const	id);


void
uaiFree(
    UnitAndId* const	node);


#ifdef __cplusplus
}
#endif

#endif
