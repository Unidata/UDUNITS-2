/**
 * This file implements error messaging for version 2 of the Unidata UDUNITS
 * package.
 *
 *  @file:  error.c
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

/*
 * This module is thread-compatible but not thread-safe.  Multi-threaded
 * access must be externally synchronized.
 */

/*LINTLIBRARY*/

#include "config.h"

#include "udunits2.h"

#include <stdarg.h>
#include <stdio.h>


/*
 * Writes an error-message to the standard-error stream when received and
 * appends a newline.  This is the initial error-message handler.
 *
 * Arguments:
 *	fmt	The format for the error-message.
 *	args	The arguments of "fmt".
 * Returns:
 *	<0	A output error was encountered.  See "errno".
 *	else	The number of bytes of "fmt" and "arg" written excluding any
 *		terminating NUL.
 */
int
ut_write_to_stderr(
    const char* const	fmt,
    va_list		args)
{
    int	nbytes = vfprintf(stderr, fmt, args);

    (void)fputc('\n', stderr);

    return nbytes;
}


/*
 * Does nothing with an error-message.
 *
 * Arguments:
 *	fmt	The format for the error-message.
 *	args	The arguments of "fmt".
 * Returns:
 *	0	Always.
 */
int
ut_ignore(
    const char* const	fmt,
    va_list		args)
{
    return 0;
}


static ut_error_message_handler	errorMessageHandler = ut_write_to_stderr;


/*
 * Returns the previously-installed error-message handler and optionally
 * installs a new handler.  The initial handler is "ut_write_to_stderr()".
 *
 * Arguments:
 *      handler		NULL or pointer to the error-message handler.  If NULL,
 *			then the handler is not changed.  The
 *			currently-installed handler can be obtained this way.
 * Returns:
 *	Pointer to the previously-installed error-message handler.
 */
ut_error_message_handler
ut_set_error_message_handler(
    ut_error_message_handler	handler)
{
    ut_error_message_handler	prev = errorMessageHandler;

    if (handler != NULL)
	errorMessageHandler = handler;

    return prev;
}


/*
 * Handles an error-message.
 *
 * Arguments:
 *	fmt	The format for the error-message.
 *	...	The arguments for "fmt".
 * Returns:
 *	<0	An output error was encountered.
 *	else	The number of bytes of "fmt" and "arg" written excluding any
 *		terminating NUL.
 */
int
ut_handle_error_message(
    const char* const	fmt,
    ...)
{
    int			nbytes;
    va_list		args;

    va_start(args, fmt);

    nbytes = errorMessageHandler(fmt, args);

    va_end(args);

    return nbytes;
}
