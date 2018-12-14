%{
/*
 * Copyright 2013 University Corporation for Atmospheric Research
 *
 * This file is part of the UDUNITS-2 package.  See the file COPYRIGHT
 * in the top-level source-directory of the package for copying and
 * redistribution conditions.
 */
/*
 * bison(1)-based parser for decoding formatted unit specifications.
 *
 * This module is thread-compatible but not thread-safe.  Multi-threaded
 * access must be externally synchronized.
 */

/*LINTLIBRARY*/

#ifndef	_XOPEN_SOURCE
#   define _XOPEN_SOURCE 500
#endif

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#ifndef _MSC_VER
#include <string.h>
#include <strings.h>
#endif

#include "udunits2.h"
#include "prefix.h"
#include "parser.h"
#include "scanner.h"

char *utGetYyCBufP(void);

static ut_unit*		_finalUnit;	/* fully-parsed specification */
static ut_system*	_unitSystem;	/* The unit-system to use */
static char*		_errorMessage;	/* last error-message */
static ut_encoding	_encoding;	/* encoding of string to be parsed */
int		_restartScanner;/* restart scanner? */
int		_isTime;        /* product_exp is time? */


/*
 * Removes leading and trailing whitespace from a string.
 *
 * Arguments:
 *	string		NUL-terminated string.  Will be modified if it
 *                      contains whitespace.
 *	encoding	The character-encoding of "string".
 * Returns:
 *      "string"
 */
char*
ut_trim(
    char* const	        string,
    const ut_encoding	encoding)
{
    static const char*	asciiSpace = " \t\n\r\f\v";
    static const char*	latin1Space = " \t\n\r\f\v\xa0";	/* add NBSP */
    const char*		whiteSpace;
    char*		start;
    char*		stop;
    size_t              len;

    whiteSpace =
	encoding == UT_LATIN1
	    ? latin1Space
	    : asciiSpace;

    start = string + strspn(string, whiteSpace);

    for (stop = start + strlen(start); stop > start; --stop)
	 if (strchr(whiteSpace, stop[-1]) == NULL)
	    break;

    len = stop - start;

    (void)memmove(string, start, len);

    string[len] = 0;

    ut_set_status(UT_SUCCESS);

    return start;
}


/*
 *  YACC error routine:
 */
void
uterror(
    char        	*s)
{
    static char*	nomem = "uterror(): out of memory";

    if (_errorMessage != NULL && _errorMessage != nomem)
	free(_errorMessage);

    _errorMessage = strdup(s);

    if (_errorMessage == NULL)
	_errorMessage = nomem;
}

/**
 * Parses an integer value into broken-down clock-time. The value is assumed to
 * have the form H[H[MM[SS]]].
 *
 * @param[in]  value   The integer value.
 * @param[out] hour    The hour field.
 * @param[out] minute  The minute field. Set to zero if appropriate.
 * @param[out] second  The second field. Set to zero if appropriate.
 */
static void to_clock(
    unsigned long       value,
    unsigned* const     hour,
    unsigned* const     minute,
    unsigned* const     second)
{
    if (value > 0)
        while (value < 10000)
            value *= 10;

    *hour = value / 10000;
    *minute = (value % 10000) / 100;
    *second = value % 100;
}

/**
 * Converts an integer value into a timezone offset as used by this package.
 *
 * @param[in]  value  The integer value. Must correspond to [+|-]H[H[MM]].
 * @param[out] time   The corresponding time as used by this package.
 * @retval     0      Success. "*time" is set.
 * @retval     -1     The integer value is invalid.
 */
static int timezone_to_time(
    const long    value,
    double* const time)
{
    unsigned hour, minute, second;

    if (value < -2400 || value > 2400)
        return -1;

    to_clock(value < 0 ? -value : value, &hour, &minute, &second);

    if (hour > 24 || minute >= 60)
        return -1;

    *time = value < 0
        ? ut_encode_clock(-hour, -minute, -second)
        : ut_encode_clock(hour, minute, second);

    return 0;
}

/**
 * Converts an integer value into a time as used by this package.
 *
 * @param[in]  value  The integer value. Must correspond to H[H[MM[SS]]].
 * @param[out] time   The corresponding time as used by this package.
 * @retval     0      Success. "*time" is set.
 * @retval     -1     The integer value is invalid.
 */
static int clock_to_time(
    const long    value,
    double* const time)
{
    unsigned hour, minute, second;

    if (value < 0)
        return -1;

    to_clock(value, &hour, &minute, &second);

    if (hour > 24 || minute >= 60 || second > 60) /* allow leap second */
        return -1;

    *time = ut_encode_clock(hour, minute, second);

    return 0;
}

/**
 * Indicates if a unit is a (non-offset) time unit.
 *
 * @param[in] unit      The unit to be checked.
 * @retval    0         If and only if the unit is not a time unit.
 */
static int isTime(
    const ut_unit* const unit)
{
    ut_status   prev = ut_get_status();
    ut_unit*    second = ut_get_unit_by_name(_unitSystem, "second");
    int         isTime = ut_are_convertible(unit, second);

    ut_free(second);
    ut_set_status(prev);
    return isTime;
}

%}

%union {
    char*	id;			/* identifier */
    ut_unit*	unit;			/* "unit" structure */
    double	rval;			/* floating-point numerical value */
    long	ival;			/* integer numerical value */
}

%token  	ERR
%token		SHIFT
%token  	MULTIPLY
%token  	DIVIDE
%token  <ival>	INT
%token  <ival>	EXPONENT
%token  <rval>	REAL
%token  <id>	ID
%token	<rval>	DATE
%token	<rval>	CLOCK
%token	<rval>	TIMESTAMP
%token	<rval>	LOGREF

%type	<unit>	unit_spec
%type   <unit>	shift_exp
%type   <unit>	product_exp
%type   <unit>	power_exp
%type   <unit>	basic_exp
%type   <rval>	timestamp
%type   <rval>	number

%%

unit_spec:      /* nothing */ {
		    _finalUnit = ut_get_dimensionless_unit_one(_unitSystem);
		    YYACCEPT;
		} |
		shift_exp {
		    _finalUnit = $1;
		    YYACCEPT;
		} |
		error {
		    YYABORT;
		}
		;

shift_exp:	product_exp {
		    $$ = $1;
		} |
		product_exp SHIFT REAL {
		    $$ = ut_offset($1, $3);
		    ut_free($1);
		    if ($$ == NULL)
			YYERROR;
		} |
		product_exp SHIFT INT {
		    $$ = ut_offset($1, $3);
		    ut_free($1);
		    if ($$ == NULL)
			YYERROR;
		} |
		product_exp SHIFT timestamp {
		    $$ = ut_offset_by_time($1, $3);
		    ut_free($1);
		    if ($$ == NULL)
			YYERROR;
		} |
		product_exp SHIFT error {
		    ut_status	prev = ut_get_status();
		    ut_free($1);
		    ut_set_status(prev);
		    YYERROR;
		}
		;

product_exp:	power_exp {
		    $$ = $1;
                    _isTime = isTime($$);
		} |
		product_exp power_exp	{
		    $$ = ut_multiply($1, $2);
                    _isTime = isTime($$);
		    ut_free($1);
		    ut_free($2);
		    if ($$ == NULL)
			YYERROR;
		} |
		product_exp error	{
		    ut_status	prev = ut_get_status();
		    ut_free($1);
		    ut_set_status(prev);
		    YYERROR;
		} |
		product_exp MULTIPLY power_exp	{
		    $$ = ut_multiply($1, $3);
                    _isTime = isTime($$);
		    ut_free($1);
		    ut_free($3);
		    if ($$ == NULL)
			YYERROR;
		} |
		product_exp MULTIPLY error	{
		    ut_status	prev = ut_get_status();
		    ut_free($1);
		    ut_set_status(prev);
		    YYERROR;
		} |
		product_exp DIVIDE power_exp	{
		    $$ = ut_divide($1, $3);
                    _isTime = isTime($$);
		    ut_free($1);
		    ut_free($3);
		    if ($$ == NULL)
			YYERROR;
		} |
		product_exp DIVIDE error	{
		    ut_status	prev = ut_get_status();
		    ut_free($1);
		    ut_set_status(prev);
		    YYERROR;
		}
		;

power_exp:	basic_exp {
		    $$ = $1;
		} |
		basic_exp INT {
		    $$ = ut_raise($1, $2);
		    ut_free($1);
		    if ($$ == NULL)
			YYERROR;
		} |
		basic_exp EXPONENT {
		    $$ = ut_raise($1, $2);
		    ut_free($1);
		    if ($$ == NULL)
			YYERROR;
		} |
		basic_exp error {
		    ut_status	prev = ut_get_status();
		    ut_free($1);
		    ut_set_status(prev);
		    YYERROR;
		}
		;

basic_exp:	ID {
		    double	prefix = 1;
		    ut_unit*	unit = NULL;
		    char*	cp = $1;
		    int		symbolPrefixSeen = 0;

		    while (*cp) {
			size_t	nchar;
			double	value;

			unit = ut_get_unit_by_name(_unitSystem, cp);

			if (unit != NULL)
			    break;

			unit = ut_get_unit_by_symbol(_unitSystem, cp);

			if (unit != NULL)
			    break;

			if (utGetPrefixByName(_unitSystem, cp, &value, &nchar)
				== UT_SUCCESS) {
			    prefix *= value;
			    cp += nchar;
			}
			else {
			    if (!symbolPrefixSeen &&
				    utGetPrefixBySymbol(_unitSystem, cp, &value,
					&nchar) == UT_SUCCESS) {
				symbolPrefixSeen = 1;
				prefix *= value;
				cp += nchar;
			    }
			    else {
				break;
			    }
			}
		    }

		    free($1);

		    if (unit == NULL) {
			ut_set_status(UT_UNKNOWN);
			YYERROR;
		    }

		    $$ = ut_scale(prefix, unit);

		    ut_free(unit);

		    if ($$ == NULL)
			YYERROR;
		} |
		'(' shift_exp ')' {
		    $$ = $2;
		} |
		'(' shift_exp error {
		    ut_status	status = ut_get_status();
		    ut_free($2);
		    ut_set_status(status);
		    YYERROR;
		} |
		LOGREF product_exp ')' {
		    $$ = ut_log($1, $2);
		    ut_free($2);
		    if ($$ == NULL)
			YYERROR;
		} |
		LOGREF product_exp error {
		    ut_status	status = ut_get_status();
		    ut_free($2);
		    ut_set_status(status);
		    YYERROR;
		} |
		number {
		    $$ = ut_scale($1,
                        ut_get_dimensionless_unit_one(_unitSystem));
		}
		;

number:		INT {
		    $$ = $1;
		} |
		REAL {
		    $$ = $1;
		}
		;

timestamp:	DATE {
		    $$ = $1;
		} |
		DATE CLOCK {
		    $$ = $1 + $2;
		} |
		DATE CLOCK CLOCK {
		    $$ = $1 + ($2 - $3);
		} |
		DATE CLOCK ID {
		    int	error = 0;

		    if (strcasecmp($3, "UTC") != 0 &&
			    strcasecmp($3, "GMT") != 0 &&
			    strcasecmp($3, "Z") != 0) {
			ut_set_status(UT_UNKNOWN);
			error = 1;
		    }

		    free($3);

		    if (!error) {
			$$ = $1 + $2;
		    }
		    else {
			YYERROR;
		    }
		} |
		TIMESTAMP {
		    $$ = $1;
		} |
		TIMESTAMP CLOCK {
		    $$ = $1 - $2;
		} |
		TIMESTAMP ID {
		    int	error = 0;

		    if (strcasecmp($2, "UTC") != 0 &&
			    strcasecmp($2, "GMT") != 0 &&
			    strcasecmp($2, "Z") != 0) {
			ut_set_status(UT_UNKNOWN);
			error = 1;
		    }

		    free($2);

		    if (!error) {
			$$ = $1;
		    }
		    else {
			YYERROR;
		    }
		}
		;

%%

#define yymaxdepth	utmaxdepth
#define yylval		utlval
#define yychar		utchar
#define yypact		utpact
#define yyr1		utr1
#define yyr2		utr2
#define yydef		utdef
#define yychk		utchk
#define yypgo		utpgo
#define yyact		utact
#define yyexca		utexca
#define yyerrflag	uterrflag
#define yynerrs		utnerrs
#define yyps		utps
#define yypv		utpv
#define yys		uts
#define yy_yys		utyys
#define yystate		utstate
#define yytmp		uttmp
#define yyv		utv
#define yy_yyv		utyyv
#define yyval		utval
#define yylloc		utlloc
#define yyreds		utreds
#define yytoks		uttoks
#define yylhs		utyylhs
#define yydefred	utyydefred
#define yydgoto		utyydgoto
#define yysindex	utyysindex
#define yyrindex	utyyrindex
#define yygindex	utyygindex
#define yytable		utyytable
#define yycheck		utyycheck
#define yyname		utyyname
#define yyrule		utyyrule

/*
 * Converts a string in the Latin-1 character set (ISO 8859-1) to the UTF-8
 * character set.
 *
 * Arguments:
 *      latin1String    Pointer to the string to be converted.  May be freed
 *                      upon return.
 * Returns:
 *      NULL            Failure.  ut_handle_error_message() was called.
 *      else            Pointer to UTF-8 representation of "string".  Must not
 *                      be freed.  Subsequent calls may overwrite.
 */
static const char*
latin1ToUtf8(
    const char* const   latin1String)
{
    static char*                utf8String = NULL;
    static size_t               bufSize = 0;
    size_t                      size;
    const unsigned char*        in;
    unsigned char*              out;

    assert(latin1String != NULL);

    size = 2 * strlen(latin1String) + 1;

    if (size > bufSize) {
        char*   buf = realloc(utf8String, size);

        if (buf != NULL) {
            utf8String = buf;
            bufSize = size;
        }
        else {
            ut_handle_error_message("Couldn't allocate %ld-byte buffer: %s",
                (unsigned long)size, strerror(errno));
            return NULL;
        }
    }

    if (utf8String) {
        for (in = (const unsigned char*)latin1String,
                out = (unsigned char*)utf8String; *in; ++in) {
#           define IS_ASCII(c) (((c) & 0x80) == 0)

            if (IS_ASCII(*in)) {
                *out++ = *in;
            }
            else {
                *out++ = 0xC0 | ((0xC0 & *in) >> 6);
                *out++ = 0x80 | (0x3F & *in);
            }
        }

        *out = 0;
    }

    return utf8String;
}


/*
 * Returns the binary representation of a unit corresponding to a string
 * representation.
 *
 * Arguments:
 *	system		Pointer to the unit-system in which the parsing will
 *			occur.
 *	string		The string to be parsed (e.g., "millimeters").  There
 *			should be no leading or trailing whitespace in the
 *			string.  See ut_trim().
 *	encoding	The encoding of "string".
 * Returns:
 *	NULL		Failure.  "ut_get_status()" will be one of
 *			    UT_BAD_ARG		"system" or "string" is NULL.
 *			    UT_SYNTAX		"string" contained a syntax
 *						error.
 *			    UT_UNKNOWN		"string" contained an unknown
 *						identifier.
 *			    UT_OS		Operating-system failure.  See
 *						"errno".
 *	else		Pointer to the unit corresponding to "string".
 */
ut_unit*
ut_parse(
    const ut_system* const	system,
    const char* const		string,
    ut_encoding			encoding)
{
    ut_unit*	unit = NULL;		/* failure */

    if (system == NULL || string == NULL) {
	ut_set_status(UT_BAD_ARG);
    }
    else {
        const char*     utf8String;

        if (encoding != UT_LATIN1) {
            utf8String = string;
        }
        else {
            utf8String = latin1ToUtf8(string);
            encoding = UT_UTF8;

            if (utf8String == NULL)
                ut_set_status(UT_OS);
        }

        if (utf8String != NULL) {
            YY_BUFFER_STATE	buf = ut_scan_string(utf8String);

            _unitSystem = (ut_system*)system;
            _encoding = encoding;
            _restartScanner = 1;

#if YYDEBUG
            utdebug = 0;
            ut_flex_debug = 0;
#endif

            _finalUnit = NULL;

            if (utparse() == 0) {
                int     status;
                /* Offset from the beginning of the buffer. */
                const int n = utGetYyCBufP() - buf->yy_ch_buf;

                if (n >= strlen(utf8String)) {
                    unit = _finalUnit;	/* success */
                    status = UT_SUCCESS;
                }
                else {
                    /*
                     * Parsing terminated before the end of the string.
                     */
                    ut_free(_finalUnit);
                    status = UT_SYNTAX;
                }

                ut_set_status(status);
            }

            ut_delete_buffer(buf);
        }                               /* utf8String != NULL */
    }                                   /* valid arguments */

    return unit;
}
