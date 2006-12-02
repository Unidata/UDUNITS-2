/*
 * lex(1) specification for tokens for the Unidata units package, UDUNITS2.
 *
 * $Id: scanner.l,v 1.2 2006/12/02 22:33:46 steve Exp $
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

%}

space			[ \t\r\f\v]
utf8_exp_0		\xe2\x81\xb0
utf8_exp_1		\xc2\xb1
utf8_exp_2		\xc2\xb2
utf8_exp_3		\xc2\xb3
utf8_exp_4		\xe2\x81\xb4
utf8_exp_5		\xe2\x81\xb5
utf8_exp_6		\xe2\x81\xb6
utf8_exp_7		\xe2\x81\xb7
utf8_exp_8		\xe2\x81\xb8
utf8_exp_9		\xe2\x81\xb9
utf8_exp_digit		{utf8_exp_0}|{utf8_exp_1}|{utf8_exp_2}|{utf8_exp_3}|{utf8_exp_4}|{utf8_exp_5}|{utf8_exp_6}|{utf8_exp_7}|{utf8_exp_8}|{utf8_exp_9}
utf8_exp_sign		\xe2\x81\xba|\xe2\x81\xbb
utf8_exponent		{utf8_exp_sign}?{utf8_exp_digit}+
int			[0-9]+
int_period		{int}\.
period_int		\.{int}
int_period_int		{int}\.{int}
mantissa		{int_period}|{period_int}|{int_period_int}
real_exp		[eE][+-]?[0-9]+
real			[+-]?({int}{real_exp}|{mantissa}{real_exp}?)
latin1_exponent		[¹²³]+
year			[+-]?[0-9]{1,4}
month			0?[1-9]|1[0-2]
day			0?[1-9]|[1-2][0-9]|30|31
hour			[+-]?[0-1]?[0-9]|2[0-3]
minute			[0-5]?[0-9]
second			({minute}|60)(\.[0-9]*)?
ascii_letter		[_a-zA-Z]
ascii_alphanum		[_a-zA-Z0-9]
ascii_id		%|{ascii_letter}({ascii_alphanum}*{ascii_letter})?
latin1_nbsp		\xa0
latin1_shy		\xad
latin1_degree		°
latin1_mu		µ
latin1_special		{latin1_nbsp}|{latin1_shy}|{latin1_degree}|{latin1_mu}
latin1_blk1		[À-Ö]
latin1_blk2		[Ø-ö]
latin1_blk3		[ø-\xff]
latin1_blk		{latin1_blk1}|{latin1_blk2}|{latin1_blk3}
latin1_letter		[_a-zA-Z]|{latin1_special}|{latin1_blk}
latin1_alphanum		{latin1_letter}|[0-9]
latin1_id		%|{latin1_letter}({latin1_alphanum}*{latin1_letter})?
utf8_ohm		\xe0\x96\xa9
utf8_special		\xc2{latin1_special}|{utf8_ohm}
utf8_blk		\xc2{latin1_blk}
utf8_letter		[_a-zA-Z]|{utf8_special}|{utf8_blk}
utf8_alphanum		{utf8_letter}|[0-9]
utf8_id			%|{utf8_letter}({utf8_alphanum}*{utf8_letter})?
broken_date		{year}-{month}(-{day})?
packed_date		{year}({month}{day}?)?
broken_clock		{hour}:{minute}(:{second})?
packed_clock		{hour}({minute}{second}?)?
broken_timestamp	{broken_date}({space}+{broken_clock})?
packed_timestamp	{packed_date}T{packed_clock}?
logref			\({space}*[Rr][Ee](:{space})?{space}*

%Start		latin1 utf8 ascii_exp latin1_exp utf8_exp

%%
    int	initial_condition;
    int	exp_condition;

    if (_encoding == UT_UTF8) {
	initial_condition = utf8;
	exp_condition = utf8_exp;
    }
    else if (_encoding == UT_LATIN1) {
	initial_condition = latin1;
	exp_condition = latin1_exp;
    }
    else {
	initial_condition = INITIAL;
	exp_condition = ascii_exp;
    }

    if (_restartScanner) {
	BEGIN(initial_condition);
	_restartScanner = 0;
    }

{space}*(@|after|AFTER|from|FROM|since|SINCE|ref|REF){space}* {
    BEGIN(initial_condition);
    return SHIFT;
}

{space}*(per|PER|"/"){space}* {
    BEGIN(initial_condition);
    return DIVIDE;
}

"-"|"."|"*"|{space}+ {
    BEGIN(initial_condition);
    return MULTIPLY;
}

<latin1,latin1_exp>"·" {
    BEGIN(initial_condition);
    return MULTIPLY;
}

<utf8,utf8_exp>\xc2\xb7 {
    BEGIN(initial_condition);
    return MULTIPLY;
}

"^"|"**" {
    BEGIN(exp_condition);
    return RAISE;
}

<latin1,latin1_exp>{latin1_exponent} {
    int	status = EXPONENT;
    int	exponent = 0;
    int	i;

    for (i = 0; i < yyleng; i++) {
	int	c = yytext[i];

	exponent *= 10;

	if (c == '¹') {
	    exponent += 1;
	}
	else if (c == '²') {
	    exponent += 2;
	}
	else if (c == '³') {
	    exponent += 3;
	}
	else {
	    status = ERR;
	    break;
	}
    }

    if (status == EXPONENT)
	yylval.ival = exponent;

    BEGIN(initial_condition);
    return status;
}

<utf8,utf8_exp>{utf8_exponent} {
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
	    {"\xe2\x81\xb0", 3},
	    {"\xc2\xb1",     2},
	    {"\xc2\xb2",     2},
	    {"\xc2\xb3",     2},
	    {"\xe2\x81\xb4", 3},
	    {"\xe2\x81\xb5", 3},
	    {"\xe2\x81\xb6", 3},
	    {"\xe2\x81\xb7", 3},
	    {"\xe2\x81\xb8", 3},
	    {"\xe2\x81\xb9", 3},
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

    BEGIN(initial_condition);
    return status;
}

{broken_date}{space}* {
    int		year;
    int		month;
    int		day = 1;

    (void) sscanf((char*)yytext, "%d-%d-%d", &year, &month, &day);

    yylval.rval	= utEncodeDate(year, month, day);

    BEGIN(initial_condition);
    return DATE;
}

{broken_clock}{space}* {
    int		hour;
    int		minute;
    double	second	= 0.0;

    (void) sscanf((char*)yytext, "%d:%d:%lf", &hour, &minute, &second);

    yylval.rval	= utEncodeClock(hour, minute, second);

    BEGIN(initial_condition);
    return CLOCK;
}

{packed_timestamp}{space}* {
    int		year;
    int		month = 1;
    int		day = 1;
    int		hour = 0;
    int		minute = 0;
    double	second	= 0.0;

    (void) sscanf((char*)yytext, "%4d%2d%2dT%2d%2d%lf", &year, &month, &day,
	&hour, &minute, &second);

    yylval.rval	= utEncodeTime(year, month, day, hour, minute, second);

    BEGIN(initial_condition);
    return TIMESTAMP;
}

{ascii_id} {
    yylval.id = strdup((char*)yytext);

    BEGIN(exp_condition);
    return ID;
}

<latin1>{latin1_id} {
    yylval.id = strdup((char*)yytext);

    BEGIN(latin1_exp);
    return ID;
}

[+-]?{int} {
    int		status;

    errno	= 0;
    yylval.ival = atol((char*)yytext);

    if (errno == 0) {
	status	= INT;
    } else {
	(void) fputs("udunits(3): Invalid integer\n", stderr);

	status	= ERR;
    }

    return status;
}

<INITIAL,latin1,utf8>{real} {
    int		status;

    errno	= 0;
    yylval.rval = atof((char*)yytext);

    if (errno == 0) {
	status	= REAL;
    } else {
	(void) fputs("udunits(3): Invalid real\n", stderr);

	status	= ERR;
    }

    return status;
}

(log|lg){space}*{logref} {
    yylval.rval = M_LOG10E;
    return LOGREF;
}

ln{space}*{logref} {
    yylval.rval = 1.0;
    return LOGREF;
}

lb{space}*{logref} {
    yylval.rval = M_LOG2E;
    return LOGREF;
}

. {
    BEGIN(initial_condition);
    return yytext[0];
}

%%
