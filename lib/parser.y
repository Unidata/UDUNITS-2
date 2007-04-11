%{
/*
 * $Id: parser.y,v 1.9 2007/04/11 20:28:17 steve Exp $
 *
 * yacc(1)-based parser for decoding formatted unit specifications.
 */

/*LINTLIBRARY*/

#ifndef	_XOPEN_SOURCE
#   define _XOPEN_SOURCE 500
#endif

#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

#include "udunits2.h"

static ut_unit*		finalUnit;	/* fully-parsed specification */
static ut_system*	unitSystem;	/* The unit-system to use */
static char*		errorMessage;	/* last error-message */
static ut_encoding	_encoding;	/* encoding of string to be parsed */
static int		_restartScanner;	/* restart scanner? */
static size_t		_nchar;		/* number of parsed characters */


/*
 * Removes leading and trailing whitespace from a string.
 *
 * Arguments:
 *	string		NUL-terminated string.  Will be modified if it contains
 *                      whitespace.
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
 * Returns the number of successfully parsed characters.  If ut_parse() was
 * successful, then the returned number will equal the length of the string;
 * otherwise, the returned number will be the 0-based index of the character
 * that caused the parse to fail.
 *
 * Returns:
 *	The number of successfully parsed characters.
 */
size_t
ut_get_parse_length(void)
{
    return _nchar;
}


/*
 *  YACC error routine:
 */
void
uterror(
    char        	*s)
{
    static char*	nomem = "ut_error(): out of memory";

    if (errorMessage != NULL && errorMessage != nomem)
	free(errorMessage);

    errorMessage = strdup(s);

    if (errorMessage == NULL)
	errorMessage = nomem;
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
%token  	RAISE
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
		    finalUnit = ut_get_dimensionless_unit_one(unitSystem);
		    YYACCEPT;
		} |
		shift_exp {
		    finalUnit = $1;
		    YYACCEPT;
		} |
		error {
		    ut_set_status(UT_SYNTAX);
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
			YYABORT;
		} |
		product_exp SHIFT INT {
		    $$ = ut_offset($1, $3);

		    ut_free($1);

		    if ($$ == NULL)
			YYABORT;
		} |
		product_exp SHIFT timestamp {
		    $$ = ut_offset_by_time($1, $3);

		    ut_free($1);

		    if ($$ == NULL)
			YYABORT;
		}
		;

product_exp:	power_exp {
		    $$ = $1;
		} |
		product_exp power_exp	{
		    $$ = ut_multiply($1, $2);

		    ut_free($1);
		    ut_free($2);

		    if ($$ == NULL)
			YYABORT;
		} |
		product_exp MULTIPLY power_exp	{
		     $$ = ut_multiply($1, $3);

		    ut_free($1);
		    ut_free($3);

		    if ($$ == NULL)
			YYABORT;
		} |
		product_exp DIVIDE power_exp	{
		     $$ = ut_divide($1, $3);

		    ut_free($1);
		    ut_free($3);

		    if ($$ == NULL)
			YYABORT;
		}
		;

power_exp:	basic_exp {
		    $$ = $1;
		} |
		basic_exp INT {
		    $$ = ut_raise($1, $2);

		    ut_free($1);

		    if ($$ == NULL)
			YYABORT;
		} |
		basic_exp EXPONENT {
		    $$ = ut_raise($1, $2);

		    ut_free($1);

		    if ($$ == NULL)
			YYABORT;
		} |
		basic_exp RAISE INT {
		    $$ = ut_raise($1, $3);

		    ut_free($1);

		    if ($$ == NULL)
			YYABORT;
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

			unit = ut_get_unit_by_name(unitSystem, cp);

			if (unit != NULL)
			    break;

			unit = ut_get_unit_by_symbol(unitSystem, cp);

			if (unit != NULL)
			    break;

			if (utGetPrefixByName(unitSystem, cp, &value, &nchar)
				!= UT_SUCCESS) {
			    if (symbolPrefixSeen ||
				    utGetPrefixBySymbol(unitSystem, cp, &value,
					&nchar) != UT_SUCCESS)
				break;

			    symbolPrefixSeen = 1;
			}

			prefix *= value;
			cp += nchar;
		    }

		    free($1);

		    if (unit == NULL) {
			ut_set_status(UT_UNKNOWN);
			YYABORT;
		    }

		    $$ = ut_scale(prefix, unit);

		    ut_free(unit);

		    if ($$ == NULL)
			YYABORT;
		} |
		'(' shift_exp ')' {
		    $$ = $2;
		} |
		LOGREF product_exp ')' {
		    $$ = ut_log($1, $2);

		    ut_free($2);

		    if ($$ == NULL)
			YYABORT;
		} |
		number {
		    $$ = ut_scale($1, ut_get_dimensionless_unit_one(unitSystem));
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
		DATE CLOCK INT {
		    $$ = $1 + ($2 - ut_encode_clock($3/60, $3%60, 0));
		} |
		DATE CLOCK ID {
		    int	error = 0;

		    if (strcasecmp($3, "UTC") != 0 &&
			    strcasecmp($3, "GMT") != 0) {
			ut_set_status(UT_UNKNOWN);
			error = 1;
		    }

		    free($3);

		    if (!error) {
			$$ = $1 + $2;
		    }
		    else {
			YYABORT;
		    }
		} |
		TIMESTAMP {
		    $$ = $1;
		} |
		TIMESTAMP CLOCK {
		    $$ = $1 - $2;
		} |
		TIMESTAMP INT {
		    $$ = $1 - ut_encode_clock($2/60, $2%60, 0);
		} |
		TIMESTAMP ID {
		    int	error = 0;

		    if (strcasecmp($2, "UTC") != 0 &&
			    strcasecmp($2, "GMT") != 0) {
			ut_set_status(UT_UNKNOWN);
			error = 1;
		    }

		    free($2);

		    if (!error) {
			$$ = $1;
		    }
		    else {
			YYABORT;
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
#define yylen		utyylen
#define yydefred	utyydefred
#define yydgoto		utyydgoto
#define yysindex	utyysindex
#define yyrindex	utyyrindex
#define yygindex	utyygindex
#define yytable		utyytable
#define yycheck		utyycheck
#define yyname		utyyname
#define yyrule		utyyrule

#include "scanner.c"


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
 *			    UT_NULL_ARG		"system" or "string" is NULL.
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
    ut_system* const	system,
    const char* const	string,
    const ut_encoding	encoding)
{
    ut_unit*	unit = NULL;		/* failure */

    if (system == NULL || string == NULL) {
	ut_set_status(UT_NULL_ARG);
    }
    else {
	YY_BUFFER_STATE	buf;

	utrestart((FILE*)NULL);

	buf = ut_scan_string(string);
	unitSystem = system;
	_encoding = encoding;
	_restartScanner = 1;

#if YYDEBUG
	utdebug = 0;
	ut_flex_debug = 0;
#endif

	finalUnit = NULL;

	if (utparse() == 0) {
	    int	n = yy_c_buf_p  - buf->yy_ch_buf;

	    if (n >= strlen(string)) {
		unit = finalUnit;	/* success */
	    }
	    else {
		/*
		 * Parsing terminated before the end of the string.
		 */
		ut_free(finalUnit);

		/*
		 * The number of characters parsed is adjusted because
		 * the scanner accepted the terminating character.
		 */
		n--;
	    }

	    _nchar = n;
	}

	ut_delete_buffer(buf);
    }

    return unit;
}
