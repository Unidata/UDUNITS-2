/**
 * This file implements a searchable unit-and-identifier tree for version 2 of
 * the Unidata UDUNITS package.
 *
 *  @file:  unitAndId.c
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

#include "unitAndId.h"
#include "udunits2.h"

#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>


/*
 * Arguments:
 *	unit	The unit.  May be freed upon return.
 *	id	The identifier (name or symbol).  May be freed upon return.
 * Returns:
 *	NULL	Failure.  "ut_get_status()" will be
 *		    UT_BAD_ARG	"unit" or "id" is NULL.
 *		    UT_OS	Operating-system failure.  See "errno".
 *	else	Pointer to the new unit-and-identifier.
 */
UnitAndId*
uaiNew(
    const ut_unit* const	unit,
    const char* const	        id)
{
    UnitAndId*	entry = NULL;		/* failure */

    if (id == NULL || unit == NULL) {
	ut_set_status(UT_BAD_ARG);
	ut_handle_error_message("uaiNew(): NULL argument");
    }
    else {
	entry = malloc(sizeof(UnitAndId));

	if (entry == NULL) {
	    ut_set_status(UT_OS);
	    ut_handle_error_message(strerror(errno));
	    ut_handle_error_message("Couldn't allocate %lu-byte data-structure",
		sizeof(UnitAndId));
	}
	else {
	    entry->id = strdup(id);

	    if (entry->id == NULL) {
		ut_set_status(UT_OS);
		ut_handle_error_message(strerror(errno));
		ut_handle_error_message("Couldn't duplicate identifier");
	    }
	    else {
		entry->unit = ut_clone(unit);

		if (entry->unit == NULL) {
		    assert(ut_get_status() != UT_SUCCESS);
		    free(entry->id);
		}
	    }

	    if (ut_get_status() != UT_SUCCESS) {
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
	ut_free(entry->unit);
	free(entry);
    }
}
