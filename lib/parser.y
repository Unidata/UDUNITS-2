%{
/*
 * $Id: parser.y,v 1.6 2006/12/21 20:52:37 steve Exp $
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

static utUnit*		finalUnit;	/* fully-parsed specification */
static utSystem*	unitSystem;	/* The unit-system to use */
static char*		errorMessage;	/* last error-message */
static utEncoding	_encoding;	/* encoding of string to be parsed */
static int		_restartScanner;	/* restart scanner? */
static size_t		_nchar;		/* number of parsed characters */


/*
 * Returns a string with leading and trailing whitespace removed.  The returned
 * string should be freed when it is no longer needed.
 *
 * Arguments:
 *	string		NUL-terminated string.  May be freed upon return.
 *	encoding	The character-encoding of "string".
 * Returns:
 *	NULL	Failure.  "utStatus" will be:
 *		    UT_OS	Operating-system failure.  See "errno".
 *	else	Pointer to the string with leading and trailing whitespace
 *		removed.
 */
char*
utTrim(
    const char* const	string,
    const utEncoding	encoding)
{
    static const char*	asciiSpace = " \t\n\r\f\v";
    static const char*	latin1Space = " \t\n\r\f\v\xa0";	/* add NBSP */
    const char*		whiteSpace;
    const char*		start;
    const char*		stop;
    size_t		nchar;
    char*		newString;

    whiteSpace =
	encoding == UT_LATIN1
	    ? latin1Space
	    : asciiSpace;

    start = string + strspn(string, whiteSpace);

    for (stop = string + strlen(string); stop > string; --stop)
	 if (strchr(whiteSpace, stop[-1]) == NULL)
	    break;

    nchar = stop - start;
    newString = malloc(nchar + 1);

    if (newString == NULL) {
	utHandleErrorMessage(strerror(errno));
	utHandleErrorMessage("Couldn't allocate %lu-byte string-buffer",
	    nchar+1);
	utStatus = UT_OS;
    }
    else {
	strncpy(newString, start, nchar);
	newString[nchar] = 0;
	utStatus = UT_SUCCESS;
    }

    return newString;
}


/*
 * Returns the number of successfully parsed characters.  If utParse() was
 * successful, then the returned number will equal the length of the string;
 * otherwise, the returned number will be the 0-based index of the character
 * that caused the parse to fail.
 *
 * Returns:
 *	The number of successfully parsed characters.
 */
size_t
utGetParseLength(void)
{
    return _nchar;
}


/*
 *  YACC error routine:
 */
void
ut_error(
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
    utUnit*	unit;			/* "unit" structure */
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
		    finalUnit = utGetDimensionlessUnitOne(unitSystem);
		    YYACCEPT;
		} |
		shift_exp {
		    finalUnit = $1;
		    YYACCEPT;
		} |
		error {
		    utStatus = UT_SYNTAX;
		    YYABORT;
		}
		;

shift_exp:	product_exp {
		    $$ = $1;
		} |
		product_exp SHIFT REAL {
		    $$ = utOffset($1, $3);

		    utFree($1);

		    if ($$ == NULL)
			YYABORT;
		} |
		product_exp SHIFT INT {
		    $$ = utOffset($1, $3);

		    utFree($1);

		    if ($$ == NULL)
			YYABORT;
		} |
		product_exp SHIFT timestamp {
		    $$ = utOffsetByScalarTime($1, $3);

		    utFree($1);

		    if ($$ == NULL)
			YYABORT;
		}
		;

product_exp:	power_exp {
		    $$ = $1;
		} |
		product_exp power_exp	{
		    $$ = utMultiply($1, $2);

		    utFree($1);
		    utFree($2);

		    if ($$ == NULL)
			YYABORT;
		} |
		product_exp MULTIPLY power_exp	{
		     $$ = utMultiply($1, $3);

		    utFree($1);
		    utFree($3);

		    if ($$ == NULL)
			YYABORT;
		} |
		product_exp DIVIDE power_exp	{
		     $$ = utDivide($1, $3);

		    utFree($1);
		    utFree($3);

		    if ($$ == NULL)
			YYABORT;
		}
		;

power_exp:	basic_exp {
		    $$ = $1;
		} |
		basic_exp INT {
		    $$ = utRaise($1, $2);

		    utFree($1);

		    if ($$ == NULL)
			YYABORT;
		} |
		basic_exp EXPONENT {
		    $$ = utRaise($1, $2);

		    utFree($1);

		    if ($$ == NULL)
			YYABORT;
		} |
		basic_exp RAISE INT {
		    $$ = utRaise($1, $3);

		    utFree($1);

		    if ($$ == NULL)
			YYABORT;
		}
		;

basic_exp:	ID {
		    double	prefix = 1;
		    utUnit*	unit = NULL;
		    char*	cp = $1;
		    int		symbolPrefixSeen = 0;

		    while (*cp) {
			size_t	nchar;
			double	value;

			unit = utGetUnitByName(unitSystem, cp);

			if (unit != NULL)
			    break;

			unit = utGetUnitBySymbol(unitSystem, cp);

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
			utStatus = UT_UNKNOWN;
			YYABORT;
		    }

		    $$ = utScale(prefix, unit);

		    utFree(unit);

		    if ($$ == NULL)
			YYABORT;
		} |
		'(' shift_exp ')' {
		    $$ = $2;
		} |
		LOGREF product_exp ')' {
		    $$ = utLog($1, $2);

		    utFree($2);

		    if ($$ == NULL)
			YYABORT;
		} |
		number {
		    $$ = utScale($1, utGetDimensionlessUnitOne(unitSystem));
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
		    $$ = $1 + ($2 - utEncodeClock($3/60, $3%60, 0));
		} |
		DATE CLOCK ID {
		    int	error = 0;

		    if (strcasecmp($3, "UTC") != 0 &&
			    strcasecmp($3, "GMT") != 0) {
			utStatus = UT_UNKNOWN;
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
		    $$ = $1 - utEncodeClock($2/60, $2%60, 0);
		} |
		TIMESTAMP ID {
		    int	error = 0;

		    if (strcasecmp($2, "UTC") != 0 &&
			    strcasecmp($2, "GMT") != 0) {
			utStatus = UT_UNKNOWN;
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

#define yymaxdepth	ut_maxdepth
#define yylval		ut_lval
#define yychar		ut_char
#define yypact		ut_pact
#define yyr1		ut_r1
#define yyr2		ut_r2
#define yydef		ut_def
#define yychk		ut_chk
#define yypgo		ut_pgo
#define yyact		ut_act
#define yyexca		ut_exca
#define yyerrflag	ut_errflag
#define yynerrs		ut_nerrs
#define yyps		ut_ps
#define yypv		ut_pv
#define yys		ut_s
#define yy_yys		ut_yys
#define yystate		ut_state
#define yytmp		ut_tmp
#define yyv		ut_v
#define yy_yyv		ut_yyv
#define yyval		ut_val
#define yylloc		ut_lloc
#define yyreds		ut_reds
#define yytoks		ut_toks
#define yylhs		ut_yylhs
#define yylen		ut_yylen
#define yydefred	ut_yydefred
#define yydgoto		ut_yydgoto
#define yysindex	ut_yysindex
#define yyrindex	ut_yyrindex
#define yygindex	ut_yygindex
#define yytable		ut_yytable
#define yycheck		ut_yycheck
#define yyname		ut_yyname
#define yyrule		ut_yyrule

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
 *			string.  See utTrim().
 *	encoding	The encoding of "string".
 * Returns:
 *	NULL		Failure.  "utGetStatus()" will be one of
 *			    UT_NULL_ARG		"system" or "string" is NULL.
 *			    UT_SYNTAX		"string" contained a syntax
 *						error.
 *			    UT_UNKNOWN		"string" contained an unknown
 *						identifier.
 *			    UT_OS		Operating-system failure.  See
 *						"errno".
 *	else		Pointer to the unit corresponding to "string".
 */
utUnit*
utParse(
    utSystem* const	system,
    const char* const	string,
    const utEncoding	encoding)
{
    utUnit*	unit = NULL;		/* failure */

    if (system == NULL || string == NULL) {
	utStatus = UT_NULL_ARG;
    }
    else {
	YY_BUFFER_STATE	buf;

	ut_restart((FILE*)NULL);

	buf = ut__scan_string(string);
	unitSystem = system;
	_encoding = encoding;
	_restartScanner = 1;

#if YYDEBUG
	ut_debug = 0;
	ut__flex_debug = 0;
#endif

	finalUnit = NULL;

	if (ut_parse() == 0) {
	    int	n = yy_c_buf_p  - buf->yy_ch_buf;

	    if (n >= strlen(string)) {
		unit = finalUnit;	/* success */
	    }
	    else {
		/*
		 * Parsing terminated before the end of the string.
		 */
		utFree(finalUnit);

		/*
		 * The number of characters parsed is adjusted because
		 * the scanner accepted the terminating character.
		 */
		n--;
	    }

	    _nchar = n;
	}

	ut__delete_buffer(buf);
    }

    return unit;
}
