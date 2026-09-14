%{
/*
 * Copyright 2020 University Corporation for Atmospheric Research. All rights
 * reserved.
 *
 * This file is part of the UDUNITS-2 package.  See the file COPYRIGHT
 * in the top-level source-directory of the package for copying and
 * redistribution conditions.
 */
/*
 * bison(1)-based parser for decoding formatted unit specifications.
 *
 * This module is thread-compatible but not thread-safe: multi-threaded access
 * must be externally synchronized.
 */

/*LINTLIBRARY*/

#include "config.h"

#include "prefix.h"
#include "udunits2.h"

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
#include <strings.h>
#endif

extern int utlex (void);

/*
 *  YACC error routine. Defined in the post-%% section so it can inspect
 *  yychar/yylval (declared by bison's generated yyparse). When the lookahead
 *  is an ERR token carrying a scanner-supplied message, emit that message
 *  instead of the generic "syntax error".
 */
void uterror(const char *s);

/*
 * Size of the error-message buffer carried on the ERR token. Used by
 * scanner.l and by the parser's uterror() routine. Bumping this value
 * automatically resizes the union member below and every snprintf that
 * writes into it, provided callers use sizeof(yylval.error_msg) or
 * UT_ERR_MSG_LEN.
 */
#define UT_ERR_MSG_LEN 256

static ut_unit*		_finalUnit;	/* fully-parsed specification */
static ut_system*	_unitSystem;	/* The unit-system to use */
static ut_encoding	_encoding;	/* encoding of string to be parsed */
static int		_restartScanner;/* restart scanner? */
static int		_isTime;        /* product_exp is time? */


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
    char	error_msg[UT_ERR_MSG_LEN];	/* error message from lexer */
}

%token  <error_msg>	ERR
%token		SHIFT
%token  	MULTIPLY
%token  	DIVIDE
%token  <ival>	INT
%token  <ival>	EXPONENT
%token  <rval>	REAL
%token  <id>	ID
%token	<rval>	DATE
%token	<rval>	CLOCK
%token  <rval>  TZ_CLOCK
%token          Z_TOK
%token          GMT_TOK
%token          UTC_TOK
%token	<rval>	LOGREF

%type	<unit>	unit_spec
%type   <unit>	shift_exp
%type   <unit>	product_exp
%type   <unit>	power_exp
%type   <unit>	basic_exp
%type   <rval>	timestamp
%type   <rval>	number

/*
 * Free the identifier string carried by any <id> token that the parser
 * discards without reducing it through basic_exp:ID. This happens when a
 * complete specification is followed by trailing text that the scanner
 * tokenizes as an ID (e.g. "s since 2024-01-01 12:00 garbage"): the
 * timestamp reduces to a full shift_exp and the trailing ID becomes an
 * unreduced lookahead. Without this destructor the strdup() in the
 * scanner's <INITIAL,CLOCK_SEEN>{id} rule leaks.
 */
%destructor { free($$); } ID

/*
 * NOTE on parser conflicts.
 *
 * This grammar has 3 shift/reduce and 9 reduce/reduce conflicts. Both are
 * pre-existing (they predate the issue #124 datetime work) and are
 * intentionally accepted rather than refactored out:
 *
 *   - The 3 shift/reduce conflicts are all in error-recovery paths
 *     (`product_exp . error`, `'(' product_exp . error`) plus the
 *     juxtaposition ambiguity at `basic_exp . INT` — e.g. in "m 2" the
 *     parser must decide whether the INT is an exponent on the preceding
 *     basic_exp or a fresh number being multiplied in. Bison's default
 *     (shift) gives "m^2", which is the desired behavior.
 *
 *   - The 9 reduce/reduce conflicts are all inside `LOGREF product_exp
 *     error`, where the same error sequence can reduce via two different
 *     productions. Both paths produce the same user-visible "syntax
 *     error" — the conflict is harmless.
 *
 * We deliberately do not use %expect: bison treats it as also asserting
 * %expect-rr 0, and %expect-rr itself is GLR-only. Switching to a GLR
 * parser is a larger change than these warnings warrant. The build will
 * therefore continue to emit two conflict warnings on every build; a
 * future PR refactoring the error-recovery rules should bring both
 * counts down together.
 */

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

timestamp:      DATE {
                    $$ = $1;
                } |
                DATE CLOCK {
                    $$ = $1 + $2;
                } |
                DATE CLOCK TZ_CLOCK {
                    $$ = $1 + ($2 - $3);
                } |
                DATE CLOCK Z_TOK {
                    $$ = $1 + $2;
                } |
                DATE CLOCK GMT_TOK {
                    $$ = $1 + $2;
                } |
                DATE CLOCK UTC_TOK {
                    $$ = $1 + $2;
                } |
                DATE Z_TOK {
                    $$ = $1;
                } |
                ERR {
                    /* Date parsing error. Some lexer paths emit the
                       message themselves (e.g. via ut_check_date) and
                       leave $1 empty to signal "do not re-emit". */
                    if ($1[0] != '\0') ut_handle_error_message("%s", $1);
                    YYERROR;
                } |
                DATE ERR {
                    /* Clock parsing error (see ERR rule above). */
                    if ($2[0] != '\0') ut_handle_error_message("%s", $2);
                    YYERROR;
                } |
                DATE CLOCK ERR {
                    /* Timezone offset parsing error (see ERR rule above). */
                    if ($3[0] != '\0') ut_handle_error_message("%s", $3);
                    YYERROR;
                }
                /*
                 * Note: no `DATE error` / `DATE CLOCK error` productions.
                 * They added two undeclared shift/reduce conflicts (the
                 * parser couldn't tell whether `DATE . error` should reduce
                 * `DATE` into a complete timestamp first or shift the
                 * error into an inline rule). Removing them changes nothing
                 * user-visible: errors after DATE/DATE-CLOCK now fall
                 * through to the outer `product_exp SHIFT error` catcher,
                 * which produces the same "syntax error" message.
                 */
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

#include "scanner.c"


/*
 *  YACC error routine.
 *
 *  Bison calls this with "syntax error" when the current lookahead (yychar)
 *  has no valid action in the current parser state. The scanner attaches a
 *  detailed message to yylval.error_msg for ERR tokens that diagnose
 *  specific lexical problems (integer overflow, invalid date components,
 *  disallowed NaN/Inf, etc.). When such an ERR is unconsumed by any
 *  grammar production — i.e. it falls through to default error recovery —
 *  this routine emits the scanner's detailed message instead of the
 *  bison-supplied generic string.
 *
 *  Productions that consume ERR explicitly (see timestamp:) emit the
 *  message inline and then invoke YYERROR, which does not call yyerror.
 *  Those paths are unaffected.
 */
void uterror(const char *s)
{
    if (yychar == ERR && yylval.error_msg[0] != '\0') {
        ut_handle_error_message("%s", yylval.error_msg);
    } else {
        ut_handle_error_message("%s", s);
    }
}


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
                int       status;
                ptrdiff_t n = yy_c_buf_p  - buf->yy_ch_buf;

                if (n >= strlen(utf8String)) {
                    unit = _finalUnit;	/* success */
                    status = UT_SUCCESS;
                }
                else {
                    /*
                     * Parsing terminated before the end of the string.
                     */
					size_t consumed = (size_t)n;
					const char* leftover = utf8String + consumed;

					/*
					 * Truncate leftover text for display (~50 chars).
					 * %.47s is a byte-count cap, so we must not stop in
					 * the middle of a UTF-8 multi-byte sequence; walk
					 * back to the nearest lead byte (a non-continuation
					 * byte, i.e. (c & 0xC0) != 0x80).
					 */
					char leftover_snippet[64];
					size_t leftover_len = strlen(leftover);
					if (leftover_len > 50) {
					    size_t cut = 47;
					    while (cut > 0 &&
					           ((unsigned char)leftover[cut] & 0xC0) == 0x80) {
					        --cut;
					    }
					    snprintf(leftover_snippet, sizeof(leftover_snippet),
					             "%.*s...", (int)cut, leftover);
					} else {
					    snprintf(leftover_snippet, sizeof(leftover_snippet),
					             "%s", leftover);
					}

					ut_handle_error_message(
						"Unexpected text after unit specification: \"%s\"",
						leftover_snippet);

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
