/**
 * This file handles the status of the last operation by the UDUNITS2(3)
 * library.
 *
 *  @file:  status.c
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

static ut_status		_status = UT_SUCCESS;


/*
 * Returns the status of the last operation by the units module.  This function
 * will not change the status.
 */
ut_status
ut_get_status()
{
    return _status;
}


/*
 * Sets the status of the units module.  This function would not normally be
 * called by the user unless they were doing their own parsing or formatting.
 *
 * Arguments:
 *	status	The status of the units module.
 */
void
ut_set_status(
    const ut_status	status)
{
    _status = status;
}
