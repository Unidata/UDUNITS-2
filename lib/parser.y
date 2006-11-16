%{
/*
 * $Id: parser.y,v 1.1 2006/11/16 20:21:06 steve Exp $
 *
 * yacc(1)-based parser for decoding formatted unit specifications.
 */

/*LINTLIBRARY*/

#include <stdio.h>
#include <string.h>

#include "units.h"
#include "scanner.h"

int	UtLineno;		/* input-file line index */
int	UnitNotFound;		/* parser didn't find unit */
utUnit	*FinalUnit;		/* fully-parsed specification */


/*
 * Parses a string representation of a unit and returns the corresponding unit.
 *
 * Arguments:
 *	system		Pointer to the unit-system in which the parsing will
 *			occur.
 *	string		The string to be parsed (e.g., "millimeters").  There
 *			should be no leading or trailing whitespace in the
 *			string.
 *	encoding	The encoding of "string".
 *	nchar		NULL or pointer to storage for the number of characters
 *			at the beginning of "string" that correspond to the
 *			returned unit.
 * Returns:
 *	NULL	Failure.  "utGetStatus()" will be
 *		    UT_BADARG	"string" is NULL.
 *		    UT_PARSE	"string" couldn't be parsed into a known unit.
 *	else	Pointer to the unit corresponding to the first "*nchar"
 *		characters of "string".
 */
utUnit*
utParse(
    utSystem* const	system,
    const char* const	string,
    const utEncoding	encoding,
    int* const		nchar)
{
    unitStatus = UT_PARSE;

    return NULL;
}

%}

%union {
    double	rval;			/* floating-point numerical value */
    long	ival;			/* integer numerical value */
    const char*	name;			/* name of a quantity */
    utUnit	unit;			/* "unit" structure */
}

%token  <ival>	INT
%token  <ival>	ERR
%token	<ival>	SHIFT
%token  <ival>	SPACE
%token  <ival>	MULTIPLY
%token  <ival>	DIVIDE
%token  <ival>	EXPONENT
%token  <rval>	REAL
%token  <name>	NAME
%token	<rval>	DATE
%token	<rval>	TIME
%token	<rval>	ZONE

%type   <rval>	number_exp
%type   <rval>	value_exp
%type   <rval>	timestamp
%type   <rval>	time_exp
%type   <unit>	unit_exp
%type   <unit>	power_exp
%type   <unit>	factor_exp
%type   <unit>	quant_exp
%type   <unit>	origin_exp
%type	<unit>	unit_spec

%%

unit_spec:        /* nothing */			{
			YYACCEPT;
		  }
		| origin_exp			{
			(void)utCopy(&$1, FinalUnit);
			YYACCEPT;
		  }
		| error				{
			yyerrok;
			yyclearin;
			YYABORT;
		  }
		;

origin_exp:	  unit_exp			{
			(void)utCopy(&$1, &$$);
		  }
		| unit_exp SHIFT value_exp	{
			if (utIsTime(&$1)) {
			    /*
			     * The shift amount is divided by the unit scale
			     * factor in the following because the shift amount
			     * must be in the units of the first argument (e.g.
			     * 0.555556 kelvins for the fahrenheit unit) and a
			     * timestamp isn't necessarily so proportioned.
			    (void)utShift(&$1, $3, &$$);
			     */
			    (void)utShift(&$1, $3/$1.factor, &$$);
			} else {
			    (void) utShift(&$1, $3, &$$);
			}
		  }
		| unit_exp SHIFT timestamp	{
			if (utIsTime(&$1)) {
			    (void)utShift(&$1, $3/$1.factor, &$$);
			} else {
			    UnitNotFound	= 1;
			    YYERROR;
			}
		  }
		    ;

unit_exp:	  power_exp			{
			(void)utCopy(&$1, &$$);
		  }
		| unit_exp power_exp	{
			(void)utMultiply(&$1, &$2, &$$);
		  }
		| unit_exp MULTIPLY power_exp	{
			(void)utMultiply(&$1, &$3, &$$);
		  }
		| unit_exp DIVIDE power_exp	{
			(void)utDivide(&$1, &$3, &$$);
		  }
		;

power_exp:	  factor_exp			{
			(void)utCopy(&$1, &$$);
		  }
		| power_exp INT			{
			(void)utRaise(&$1, $2, &$$);
		  }
		| power_exp EXPONENT INT	{
			(void)utRaise(&$1, $3, &$$);
		  }
		;

factor_exp:	  number_exp			{
			utUnit	unit;
			(void)utScale(utClear(&unit), $1, &$$);
		  }
		| quant_exp			{
			(void)utCopy(&$1, &$$);
		  }
		| '(' origin_exp ')'		{
			(void)utCopy(&$2, &$$);
		  }
		;

quant_exp:	  NAME                          {
			utUnit     unit;
			if (utFind($1, &unit) != 0) {
			    UnitNotFound	= 1;
			    YYERROR;
			}
			(void)utCopy(&unit, &$$);
		  }
		| NAME INT			{
			utUnit     unit;
			if (utFind($1, &unit) != 0) {
			    UnitNotFound	= 1;
			    YYERROR;
			}
			(void)utRaise(&unit, $2, &$$);
		  }
		;

value_exp:	  number_exp			{ $$ = $1; }
		| '(' value_exp ')'		{ $$ = $2; }
		;

number_exp:	  INT				{ $$ = $1; }
		| REAL				{ $$ = $1; }
		;

timestamp:	  time_exp			{ $$ = $1; }
		| '(' timestamp ')'		{ $$ = $2; }
		;

time_exp:	  DATE 				{ $$ = $1; }
		| DATE TIME 			{ $$ = $1 + $2; }
		| DATE TIME ZONE 		{ $$ = $1 + ($2 - $3); }
		;

%%
