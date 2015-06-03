/*
 * Copyright 2014 University Corporation for Atmospheric Research
 *
 * This file is part of the UDUNITS-2 package.  See the file COPYRIGHT
 * in the top-level source-directory of the package for copying and
 * redistribution conditions.
 */
/*
 * lex(1) specification for tokens for the Unidata units package, UDUNITS2.
 */

%option noyywrap

%{

#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 * Decodes a date.
 *
 * @param[in]  text     The text specifying the date to be decoded.
 * @param[in]  format   The format to use for decoding. The order is year (int),
 *                      month (int), and day (int).
 * @param[out] date     The date corresponding to the input.
 * @retval     DATE     Success
 * @retval     ERR      Error
 */
static double decodeDate(
    const char* const   text,
    const char* const   format,
    double* const       date)
{
    int		year;
    int		month = 1;
    int		day = 1;

    (void)sscanf(text, format, &year, &month, &day);
    *date = ut_encode_date(year, month, day);
    return DATE;
}

/**
 * Decodes a clock-time.
 *
 * @param[in] text      The text specifying the clock-time to be decoded.
 * @param[in] format    The format to use for decoding. The order is hour (int),
 *                      minute (int), and second (double).
 * @return              The clock-time corresponding to the input.
 */
static double decodeClock(
    const char* const   text,
    const char* const   format)
{
    int         hour = 0;
    int         minute = 0;
    double      second = 0;

    (void)sscanf(text, format, &hour, &minute, &second);
    if (hour < 0) {
        minute = -minute;
        second = -second;
    }
    return ut_encode_clock(hour, minute, second);
}

/**
 * Decodes a real value.
 *
 * @param[in]  text     Text to be decoded.
 * @param[out] value    Decoded value.
 * @retval     REAL     Success.
 * @retval     ERR      Failure.
 */
static int decodeReal(
    const char* const text,
    double* const     value)
{
    errno = 0;
    *value = strtod(text, NULL);

    if (errno == 0)
        return REAL;

    ut_handle_error_message("Invalid real: \"%s\"", text);
    return ERR;
}

%}

space			[ \t\r\f\v]
sign                    [+-]
int			[0-9]+
int_period		{int}\.
period_int		\.{int}
int_period_int		{int}\.{int}
mantissa		{int_period}|{period_int}|{int_period_int}
real_exp		[eE][+-]?[0-9]+
real			[+-]?({int}{real_exp}|{mantissa}{real_exp}?)
year			[+-]?[0-9]{1,4}
month			0?[1-9]|1[0-2]
day			0?[1-9]|[1-2][0-9]|30|31
hour			[+-]?[0-1]?[0-9]|2[0-3]
minute			[0-5]?[0-9]
second			({minute}|60)(\.[0-9]*)?
middot                  \xc2\xb7
utf8_exp_digit	        \xc2(\xb9|\xb2|\xb3)|\xe2\x81(\xb0|[\xb4-\xb9])
utf8_exp_sign		\xe2\x81\xba|\xe2\x81\xbb
utf8_exponent		{utf8_exp_sign}?{utf8_exp_digit}+
nbsp                    \xc2\xa0
shy                     \xc2\xad
degree                  \xc2\xb0
mu                      \xc2\xb5
blk1                    \xc3([\x80-\x96])
blk2                    \xc3([\x98-\xB6])
blk3                    \xc3([\xB8-\xBF])
latin1		        {nbsp}|{shy}|{degree}|{mu}|{blk1}|{blk2}|{blk3}
utf8_cont               [\x80-\xbf]
utf8_2bytes             [\xc8-\xdf]{utf8_cont}
utf8_3bytes             [\xe0-\xef]{utf8_cont}{utf8_cont}
letter  		[_a-zA-Z]|{latin1}|{utf8_2bytes}|{utf8_3bytes}
alphanum		{letter}|[0-9]
id			%|'|\"|{letter}({alphanum}*{letter})?
broken_date		{year}-{month}(-{day})?
packed_date		{year}({month}{day}?)?
broken_clock		{hour}:{minute}(:{second})?
packed_clock		{hour}({minute}{second}?)?
broken_timestamp	{broken_date}({space}+{broken_clock})?
packed_timestamp	{packed_date}T{packed_clock}?
logref			\({space}*[Rr][Ee](:{space})?{space}*
after                   [Aa][Ff][Tt][Ee][Rr]
from                    [Ff][Rr][Oo][Mm]
since                   [Ss][Ii][Nn][Cc][Ee]
ref                     [Rr][Ee][Ff]
per                     [Pp][Ee][Rr]

%Start		ID_SEEN SHIFT_SEEN DATE_SEEN CLOCK_SEEN

%%
    if (_restartScanner) {
	BEGIN INITIAL;
	_restartScanner = 0;
    }

<INITIAL,ID_SEEN>{space}*(@|{after}|{from}|{ref}|{since}){space}* {
    BEGIN SHIFT_SEEN;
    return SHIFT;
}

<INITIAL,ID_SEEN>{space}*({per}|"/"){space}* {
    BEGIN INITIAL;
    return DIVIDE;
}

<INITIAL,ID_SEEN>"-"|"."|"*"|{middot}|{space}+ {
    BEGIN INITIAL;
    return MULTIPLY;
}

<INITIAL,ID_SEEN>("^"|"**")[+-]?{int} {
    int		status;

    if (sscanf(yytext, "%*[*^]%ld", &yylval.ival) != 1) {
        ut_handle_error_message("Invalid integer\n", stderr);

	status	= ERR;
    }
    else {
	status	= EXPONENT;
    }

    return status;
}

<INITIAL,ID_SEEN>{utf8_exponent} {
    int		status = EXPONENT;
    int		exponent = 0;
    int		sign = 1;
    char*	cp = yytext;

    if (strncmp(cp, "\xe2\x81\xba", 3) == 0) {
	cp += 3;
    }
    else if (strncmp(cp, "\xe2\x81\xbb", 3) == 0) {
	sign = -1;
	cp += 3;
    }

    while (cp < yytext + yyleng) {
	int	j;
	static struct {
	    const char*	string;
	    const int	len;
	} utf8_exponents[] = {
	    {"\xe2\x81\xb0", 3},        /* 0 */
	    {"\xc2\xb9",     2},        /* 1 */
	    {"\xc2\xb2",     2},        /* 2 */
	    {"\xc2\xb3",     2},        /* 3 */
	    {"\xe2\x81\xb4", 3},        /* 4 */
	    {"\xe2\x81\xb5", 3},        /* 5 */
	    {"\xe2\x81\xb6", 3},        /* 6 */
	    {"\xe2\x81\xb7", 3},        /* 7 */
	    {"\xe2\x81\xb8", 3},        /* 8 */
	    {"\xe2\x81\xb9", 3},        /* 9 */
	};

	exponent *= 10;

	for (j = 0; j < 10; j++) {
	    int	len = utf8_exponents[j].len;

	    if (strncmp(cp, utf8_exponents[j].string, len) == 0) {
		exponent += j;
		cp += len;
		break;
	    }
	}

	if (j >= 10) {
	    status = ERR;
	    break;
	}
    }

    if (status == EXPONENT)
	yylval.ival = sign * exponent;

    BEGIN INITIAL;
    return status;
}

<SHIFT_SEEN>{broken_date}(T|{space}*) {
    BEGIN DATE_SEEN;
    return decodeDate((char*)yytext, "%d-%d-%d", &yylval.rval);
}

<SHIFT_SEEN>{packed_date}(T|{space}*) {
    if (_isTime) {
        BEGIN DATE_SEEN;
        return decodeDate((char*)yytext, "%4d%2d%2d", &yylval.rval);
    }
    else {
        BEGIN INITIAL;
        return decodeReal((char*)yytext, &yylval.rval);
    }
}

<DATE_SEEN>{broken_clock}{space}* {
    yylval.rval = decodeClock((char*)yytext, "%d:%d:%lf");
    BEGIN CLOCK_SEEN;
    return CLOCK;
}

<DATE_SEEN>{packed_clock}{space}* {
    yylval.rval = decodeClock((char*)yytext, "%2d%2d%lf");
    BEGIN CLOCK_SEEN;
    return CLOCK;
}

<CLOCK_SEEN>{sign}?{int}:{int} {
    yylval.rval	= decodeClock((char*)yytext, "%d:%d");
    BEGIN INITIAL;
    return CLOCK;
}

<CLOCK_SEEN>{sign}{int} {
    yylval.rval	= (yyleng <= 3)
                        ? decodeClock((char*)yytext, "%d")
                        : (yyleng == 4)
                            ? decodeClock((char*)yytext, "%2d%d")
                            : decodeClock((char*)yytext, "%3d%d");
    BEGIN INITIAL;
    return CLOCK;
}

<CLOCK_SEEN>{int} {
    yylval.rval	= (yyleng <= 2)
                        ? decodeClock((char*)yytext, "%d")
                        : (yyleng == 3)
                            ? decodeClock((char*)yytext, "%1d%d")
                            : decodeClock((char*)yytext, "%2d%d");
    BEGIN INITIAL;
    return CLOCK;
}

<INITIAL,SHIFT_SEEN>{real} {
    BEGIN INITIAL;
    return decodeReal((char*)yytext, &yylval.rval);
}

<INITIAL,ID_SEEN,SHIFT_SEEN>[+-]?{int} {
    int		status;

    errno	= 0;
    yylval.ival = atol((char*)yytext);

    if (errno == 0) {
	status	= INT;
    } else {
        ut_handle_error_message("Invalid integer\n", stderr);

	status	= ERR;
    }

    BEGIN INITIAL;
    return status;
}

(log|lg){space}*{logref} {
    yylval.rval = 10;
    return LOGREF;
}

ln{space}*{logref} {
    yylval.rval = M_E;
    return LOGREF;
}

lb{space}*{logref} {
    yylval.rval = 2;
    return LOGREF;
}

<INITIAL,CLOCK_SEEN>{id} {
    yylval.id = strdup((char*)yytext);

    BEGIN ID_SEEN;
    return ID;
}

. {
    BEGIN INITIAL;
    return yytext[0];
}

%%
