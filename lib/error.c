/*
 * This module is thread-compatible but not thread-safe.  Multi-threaded
 * access must be externally synchronized.
 *
 * $Id: error.c,v 1.1 2007/01/04 17:15:40 steve Exp $
 */

/*LINTLIBRARY*/

#ifndef	_XOPEN_SOURCE
#   define _XOPEN_SOURCE 500
#endif

#include <stdarg.h>
#include <stdio.h>

#include "udunits2.h"


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
utWriteToStderr(
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
utIgnore(
    const char* const	fmt,
    va_list		args)
{
    return 0;
}


static utErrorMessageHandler	errorMessageHandler = utWriteToStderr;


/*
 * Returns the previously-installed error-message handler and optionally
 * installs a new handler.  The initial handler is "utWriteToStderr()".
 *
 * Arguments:
 *      handler		NULL or pointer to the error-message handler.  If NULL,
 *			then the handler is not changed.  The 
 *			currently-installed handler can be obtained this way.
 * Returns:
 *	Pointer to the previously-installed error-message handler.
 */
utErrorMessageHandler
utSetErrorMessageHandler(
    utErrorMessageHandler	handler)
{
    utErrorMessageHandler	prev = errorMessageHandler;

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
utHandleErrorMessage(
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
