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
#include <limits.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

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
static int decodeDate(
    const char* const   text,
    const char* const   format,
    double* const       date)
{
    int		year;
    int		month = 1;
    int		day = 1;

    int parsed = sscanf(text, format, &year, &month, &day);
    if (parsed < 1) {
        snprintf(yylval.error_msg, sizeof(yylval.error_msg), "Invalid date format: %s (Expected YYYY-MM-DD)", text);
        return ERR;
    }
    /* Range validation lives in ut_check_date() so the same rules apply
       whether a date enters through the parser or via direct API calls.
       Year 0 is unrestricted here; ut_encode_date silently normalizes it
       to year 1 via the Julian-day arithmetic (see GRAMMAR.md). */
    if (ut_check_date(year, month, day) != UT_SUCCESS) {
        /* ut_check_date already emitted the message; leave error_msg
           empty so the parser's ERR rule does not double-emit. */
        yylval.error_msg[0] = '\0';
        return ERR;
    }
    *date = ut_encode_date(year, month, day);
    return DATE;
}

/* ===== Shared scan/validate helpers ===== */

static int read_ndigits(const char** s, int n, int* out) {
    int v = 0;
    for (int i = 0; i < n; ++i) {
        char c = (*s)[i];
        if (c < '0' || c > '9') return 0;
        v = v*10 + (c - '0');
    }
    *out = v;
    *s += n;
    return 1;
}

static int read_1to2digits(const char* s, int* out, const char** endptr) {
    const char* p = s;
    if (p[0] < '0' || p[0] > '9') return 0;
    int v = p[0] - '0';
    p++;
    if (p[0] >= '0' && p[0] <= '9') {
        v = v*10 + (p[0]-'0');
        p++;
    }

    if (p[0] >= '0' && p[0] <= '9') {
        return 0;  // Third digit = overflow
    }

    *out = v;
    *endptr = p;
    return 1;
}

static int parse_fraction(const char** s, double* frac) {
    const char* p = *s;
    if (*p != '.') return 1; /* ok, no fraction */
    p++;
    if (*p < '0' || *p > '9') return 1;
    double scale = 1.0;
    double f = 0.0;
    while (*p >= '0' && *p <= '9') {
        f = f*10.0 + (*p - '0');
        scale *= 10.0;
        p++;
    }
    *frac = f/scale;
    *s = p;
    return 1;
}

/* Leap-second allowed only at 23:59:60[.frac] */
static int check_tod_range(int H, int M, double S, int had_seconds, char* error_msg) {
    if (H < 0 || H > 23) {
        snprintf(error_msg, UT_ERR_MSG_LEN, "Invalid hour: %d (must be 0-23)", H);
        return 0;
    }
    if (M < 0 || M > 59) {
        snprintf(error_msg, UT_ERR_MSG_LEN, "Invalid minute: %d (must be 0-59)", M);
        return 0;
    }
    if (!had_seconds) return 1; /* HH:MM ok */
    if (S < 0.0) {
        snprintf(error_msg, UT_ERR_MSG_LEN, "Invalid second: %.2f (must be non-negative)", S);
        return 0;
    }
    if (S < 60.0) return 1;
    /* S == 60 only if 23:59:60[.x] */
    if (S >= 61.0) {
        snprintf(error_msg, UT_ERR_MSG_LEN, "Invalid second: %.2f (must be less than 61)", S);
        return 0;
    }
    if (H != 23 || M != 59) {
        snprintf(error_msg, UT_ERR_MSG_LEN, "Leap second (60) only allowed at 23:59:60, not %02d:%02d:%.2f", H, M, S);
        return 0;
    }
    return 1;
}

/* TZ: sign*(HH[:MM] | HHMM); 0<=HH<=14, 0<=MM<=59; forbid negative zero */
static int check_tz_range(int sign, int H, int M) {
    // Defensive checks: H and M should be non-negative from parsing,
    // but we verify to be safe and consistent with validation elsewhere
    if (H < 0 || H > 14) return 0;
    if (H == 14 && M > 0) return 0;
    if (M < 0 || M > 59) return 0;
    if (sign < 0 && H == 0 && M == 0) return -1; /* negative zero sentinel */
    return 1;
}

/* ===== Parsers for subforms ===== */

static int decodePackedDate(
    const char* const   text,
    double* const       date)
{
    const char* p = text;
    int sign = 1;
    int year = 0;
    int month = 1;
    int day = 1;

    // Handle optional sign
    if (*p == '+' || *p == '-') {
        if (*p == '-') sign = -1;
        p++;
    }

    // Count remaining digits
    int digit_count = 0;
    const char* q = p;
    while (*q >= '0' && *q <= '9') {
        digit_count++;
        q++;
    }

    // Skip trailing T or spaces
    while (*q == 'T' || *q == ' ' || *q == '\t') q++;

    // Should have consumed entire input
    if (*q != '\0') {
        snprintf(yylval.error_msg, sizeof(yylval.error_msg), "Invalid packed date format: %s", text);
        return ERR;
    }

    // Parse based on digit count
    if (digit_count >= 1 && digit_count <= 4) {
        // Y, YY, YYY, YYYY
        if (sscanf(p, "%d", &year) != 1) {
            snprintf(yylval.error_msg, sizeof(yylval.error_msg), "Invalid packed date: cannot parse year from %s", text);
            return ERR;
        }
        year *= sign;
    }
    else if (digit_count >= 5 && digit_count <= 6) {
        // YYYYM or YYYYMM
        if (sscanf(p, "%4d%d", &year, &month) != 2) {
            snprintf(yylval.error_msg, sizeof(yylval.error_msg), "Invalid packed date: cannot parse year-month from %s", text);
            return ERR;
        }
        year *= sign;
    }
    else if (digit_count >= 7 && digit_count <= 8) {
        // YYYYMMD or YYYYMMDD
        if (sscanf(p, "%4d%2d%d", &year, &month, &day) != 3) {
            snprintf(yylval.error_msg, sizeof(yylval.error_msg), "Invalid packed date: cannot parse full date from %s", text);
            return ERR;
        }
        year *= sign;
    }
    else {
        snprintf(yylval.error_msg, sizeof(yylval.error_msg), "Invalid packed date: wrong number of digits (%d)", digit_count);
        return ERR;
    }

    // Validate ranges via the public check function (single source of truth).
    if (ut_check_date(year, month, day) != UT_SUCCESS) {
        yylval.error_msg[0] = '\0';   /* ut_check_date already emitted */
        return ERR;
    }

    *date = ut_encode_date(year, month, day);
    return DATE;
}

/* TOD broken: HH:MM[:SS[.frac]] */
static int parse_tod_broken(const char* s, int* H,int* M,double* S,int* had_seconds,const char** endptr) {
    const char* p = s;
    int h,m; double sec = 0.0; *had_seconds = 0;
    if (!read_1to2digits(p, &h, &p)) return 0;
    if (*p++ != ':') return 0;
    if (!read_1to2digits(p, &m, &p)) return 0;
    if (*p == ':') {
        p++;
        int ss;
        /* <second>: [0-9]{1,2}, per GRAMMAR.md — accept 1 or 2 digits. */
        if (!read_1to2digits(p, &ss, &p)) return 0;
        sec = (double)ss;
        double frac = 0.0;
        if (!parse_fraction(&p, &frac)) return 0;
        sec += frac;
        *had_seconds = 1;
    }
    *H = h; *M = m; *S = sec;
    *endptr = p;
    return 1;
}

/* TOD packed: HH[MM[SS]][.frac] — at least HH; optional MM, then optional SS; fraction applies to SS if present else to HHMMSS interpretation */
static int parse_tod_packed(const char* s, int* H, int* M, double* S, int* had_seconds, const char** endptr) {
    const char* p = s;
    int len = 0; while (p[len] >= '0' && p[len] <= '9') len++;
    if (len < 1 || len > 6) return 0;

    int h=0, m=0, ss=0;
    const char* q = p;
    if (len <= 2) { /* H or HH */
        if (!read_1to2digits(q, &h, &q)) return 0;
        m = 0; ss = 0; *had_seconds = 0;
    } else if (len <= 4) { /* HHM or HHMM: fixed HH, flexible M */
        if (!read_ndigits(&q, 2, &h)) return 0;
        if (!read_1to2digits(q, &m, &q)) return 0;
        ss = 0; *had_seconds = 0;
    } else { /* HHMMS or HHMMSS: fixed HH, fixed MM, flexible S */
        if (!read_ndigits(&q, 2, &h)) return 0;
        if (!read_ndigits(&q, 2, &m)) return 0;
        if (!read_1to2digits(q, &ss, &q)) return 0;
        *had_seconds = 1;
    }
    if (m >= 60) return 0;
    if (*had_seconds && ss > 60) return 0;
    double sec = (double)ss;
    double frac = 0.0;
    const char* r = q;
    if (!parse_fraction(&r, &frac)) return 0;
    if (*had_seconds) sec += frac;
    else if (frac != 0.0) return 0;  /* reject fraction if len < 5 */

    *H = h; *M = m; *S = sec;
    *endptr = r;
    return 1;
}

/* TZ broken: (+|-)HH:MM */
static int parse_tz_broken(const char* text, int* sign, int* H, int* M, const char** endptr) {
    const char* p = text;

    // Parse sign
    if (*p == '+') {
        *sign = 1;
        p++;
    } else if (*p == '-') {
        *sign = -1;
        p++;
    } else {
        return 0;  // No sign
    }

    // Parse hour
    if (!read_1to2digits(p, H, &p)) return 0;

    // MUST have colon separator
    if (*p != ':') return 0;
    p++;

    // Parse minute (must be digits only, no decimal)
    if (!read_1to2digits(p, M, &p)) return 0;

    if (*p == '.') return 0;

    *endptr = p;
    return 1;
}

static int parse_tz_packed(const char* s, int* sign, int* H, int* M, const char** endptr) {
    const char* p = s;

    /* Read required sign */
    if (*p == '+') { *sign = +1; p++; }
    else if (*p == '-') { *sign = -1; p++; }
    else return 0;

    /* Measure digit length after sign */
    int len = 0;
    while (p[len] >= '0' && p[len] <= '9') len++;
    if (len < 1 || len > 4) return 0;

    int h = 0, m = 0;
    const char* q = p;

    if (len <= 2) {
        /* H or HH */
        if (!read_1to2digits(q, &h, &q)) return 0;
        m = 0;
    } else {
        /* HHM or HHMM: fixed HH, flexible M */
        if (!read_ndigits(&q, 2, &h)) return 0;
        if (!read_1to2digits(q, &m, &q)) return 0;
    }

    *H = h; *M = m;
    *endptr = q;
    return 1;
}

/* Time-of-day: broken or packed. Returns 1 on success, 0 on error; sets *seconds. */
static int decodeClockFlexible(const char* text, double* seconds, char* error_msg) {
    const char* p = text;
    int H=0, M=0, had_sec=0; double S=0.0;
    const char* end = NULL;
    int ok = 0;

    /* The form is unambiguous: presence of ':' means broken, otherwise packed.
       Do NOT fall back between the two — the packed parser will silently
       match a prefix like "9" of "9:5:3" and discard the rest. */
    if (strchr(text, ':')) {
        ok = parse_tod_broken(p, &H, &M, &S, &had_sec, &end);
    } else {
        ok = parse_tod_packed(p, &H, &M, &S, &had_sec, &end);
    }
    if (!ok) {
        snprintf(error_msg, UT_ERR_MSG_LEN, "Invalid time-of-day format: %s (expected HH:MM:SS or HHMMSS)", text);
        return 0;
    }

    /* The lexer rule appends optional trailing whitespace to the token; the
       sub-parsers don't consume it. Anything else past `end` is a leftover
       that the lexer should never have produced — defensive check. */
    while (*end == ' ' || *end == '\t' || *end == '\r' || *end == '\f' || *end == '\v') end++;
    if (*end != '\0') {
        snprintf(error_msg, UT_ERR_MSG_LEN, "Invalid time-of-day format: %s (trailing junk after parse)", text);
        return 0;
    }

    /* Range + leap-second - check_tod_range populates error_msg with specific error */
    if (!check_tod_range(H, M, S, had_sec, error_msg)) {
        return 0;
    }

    /* Convert to seconds since midnight */
    *seconds = ((double)H)*3600.0 + ((double)M)*60.0 + S;
    return 1;
}

/* Timezone offset: broken or packed. Returns 1 on success, 0 on error; sets *seconds (signed). */
static int decodeTzOffsetFlexible(const char* text, double* seconds, char* error_msg) {
    const char* p = text;
    int sign=0, H=0, M=0; const char* end = NULL;
    int ok = 0;

    /* Same reasoning as decodeClockFlexible: presence of ':' picks broken. */
    if (strchr(text, ':')) {
        ok = parse_tz_broken(p, &sign, &H, &M, &end);
    } else {
        ok = parse_tz_packed(p, &sign, &H, &M, &end);
    }
    if (!ok) {
        snprintf(error_msg, UT_ERR_MSG_LEN, "Invalid timezone offset: %s (expected ±HH:MM or ±HHMM)", text);
        return 0;
    }

    /* Defensive trailing-junk check (lexer shouldn't produce one). */
    if (*end != '\0') {
        snprintf(error_msg, UT_ERR_MSG_LEN, "Invalid timezone offset: %s (trailing junk after parse)", text);
        return 0;
    }

    int rng = check_tz_range(sign, H, M);
    if (rng == -1) {
        snprintf(error_msg, UT_ERR_MSG_LEN, "Invalid timezone offset: -00:00 not allowed");
        return 0;
    }
    if (rng == 0) {
        snprintf(error_msg, UT_ERR_MSG_LEN, "Invalid timezone offset: %+03d:%02d (must be ±14:00 or less)",
                 sign * H, M);
        return 0;
    }

    *seconds = (double)sign * (H*3600.0 + M*60.0);
    return 1;
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

    snprintf(yylval.error_msg, sizeof(yylval.error_msg), "Invalid real: \"%s\"", text);
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
year_broken		[+-]?[0-9]{1,7}
month			[0-9]{1,2}
day			    [0-9]{1,2}
tod_hour		[0-9]{1,2}
tz_hour			[+-][0-9]{1,2}
minute			[0-9]{1,2}
second			[0-9]{1,2}(\.[0-9]*)?
middot                  \xc2\xb7
utf8_exp_digit	        \xc2\xb9|\xc2\xb2|\xc2\xb3|\xe2\x81\xb0|\xe2\x81\xb4|\xe2\x81\xb5|\xe2\x81\xb6|\xe2\x81\xb7|\xe2\x81\xb8|\xe2\x81\xb9
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
utf8_2bytes_no_super    \xc2[\x80-\xb0\xb4-\xb5\xba-\xbf]|[\xc8-\xdf]{utf8_cont}
utf8_3bytes_no_super    [\xe0-\xe1]{utf8_cont}{utf8_cont}|\xe2[\x80\x82-\xbf]{utf8_cont}|\xe2\x81[\x80-\xaf\xbc-\xbf]|[\xe3-\xef]{utf8_cont}{utf8_cont}
letter  		[_a-zA-Z]|{latin1}|{utf8_2bytes_no_super}|{utf8_3bytes_no_super}
alphanum		{letter}|[0-9]
id			%|'|\"|{letter}({alphanum}*{letter})?
packed_date		{year}({month}{day}?)?
broken_clock		{tod_hour}:{minute}(:{second})?
packed_clock		{tod_hour}({minute}{second}?)?
broken_tz_clock		{tz_hour}:{minute}
packed_tz_clock		{tz_hour}{minute}?
logref			\({space}*[Rr][Ee](:{space})?{space}*
after                   [Aa][Ff][Tt][Ee][Rr]
from                    [Ff][Rr][Oo][Mm]
since                   [Ss][Ii][Nn][Cc][Ee]
ref                     [Rr][Ee][Ff]
per                     [Pp][Ee][Rr]
nanspell     ([nN][aA][nN](\([^()]*\))?)
infspell     ([iI][nN][fF]([iI][nN][iI][tT][yY])?)
idchar       [A-Za-z0-9_]
%Start		ID_SEEN SHIFT_SEEN DATE_SEEN CLOCK_SEEN

%%
    if (_restartScanner) {
	BEGIN INITIAL;
	_restartScanner = 0;
    }

<INITIAL,SHIFT_SEEN>{sign}?{nanspell}{idchar} { yyless(0);}
<INITIAL,SHIFT_SEEN>{sign}?{infspell}{idchar} { yyless(0);}

<INITIAL,SHIFT_SEEN>{sign}?{nanspell} {
    snprintf(yylval.error_msg, sizeof(yylval.error_msg),
             "NaN is not allowed in unit expressions");
    return ERR;
}
<INITIAL,SHIFT_SEEN>{sign}?{infspell} {
    snprintf(yylval.error_msg, sizeof(yylval.error_msg),
             "Infinity is not allowed in unit expressions");
    return ERR;
}

<INITIAL,ID_SEEN>{space}*(@|{after}|{from}|{ref}|{since}){space}* {
    BEGIN SHIFT_SEEN;
    return SHIFT;
}

<INITIAL,ID_SEEN>({space}+{per}{space}+|{space}*"/"{space}*) {
    BEGIN INITIAL;
    return DIVIDE;
}

<INITIAL,ID_SEEN>"-"|"."|"*"|{middot}|{space}+ {
    BEGIN INITIAL;
    return MULTIPLY;
}

    /*
     * Exponent rules. The grammar caps integer exponents at two digits
     * with no leading zeros: the magnitude must satisfy |n| <= 99 and
     * the digit sequence must be either "0" or [1-9][0-9]?.
     *
     * Two rules are needed: an accept rule for the canonical form, and
     * a reject rule that matches any other ^N / **N sequence and emits
     * a detailed error. Flex picks the longer match (then the first
     * listed on ties), so:
     *   "^99"  -> both match 3 chars -> accept rule wins (listed first)
     *   "^100" -> accept matches "^10", reject matches "^100" -> reject
     *   "^012" -> accept matches "^0",  reject matches "^012" -> reject
     */
<INITIAL,ID_SEEN>("^"|"**")[+-]?(0|[1-9][0-9]?) {
    int     status;

    if (sscanf(yytext, "%*[*^]%ld", &yylval.ival) != 1) {
        /*
         * Defensive: the regex already constrains the input to a
         * valid signed integer, so this branch should be unreachable.
         */
        snprintf(yylval.error_msg, sizeof(yylval.error_msg),
                 "Invalid exponent integer in '%s'", yytext);
        status = ERR;
    } else {
        status = EXPONENT;
    }

    return status;
}

<INITIAL,ID_SEEN>("^"|"**")[+-]?[0-9]+ {
    /*
     * Anything that isn't the canonical [+-]?(0|[1-9][0-9]?) form:
     * too many digits (|n| >= 100) or leading zeros.
     */
    snprintf(yylval.error_msg, sizeof(yylval.error_msg),
             "Invalid exponent in \"%s\": magnitude must be <= 99, "
             "no leading zeros", yytext);
    return ERR;
}

<INITIAL,ID_SEEN>{utf8_exponent} {
    int		status = EXPONENT;
    int		exponent = 0;
    int		sign = 1;
    int		digit_count = 0;
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
	    snprintf(yylval.error_msg, sizeof(yylval.error_msg),
	             "Invalid UTF-8 exponent character in \"%s\"", yytext);
	    status = ERR;
	    break;
	}

	++digit_count;
    }

    /*
     * Cap at |exp| <= 99 with no leading zeros — same rule as the
     * "^N" / "**N" exponent above. digit_count > 2 catches the
     * magnitude cap; digit_count == 2 with exponent < 10 catches
     * a leading-zero two-digit exponent (e.g. "⁰⁹" → exponent=9).
     */
    if (status == EXPONENT &&
        (digit_count > 2 || (digit_count == 2 && exponent < 10))) {
        snprintf(yylval.error_msg, sizeof(yylval.error_msg),
                 "Invalid UTF-8 exponent in \"%s\": magnitude must be "
                 "<= 99, no leading zeros", yytext);
        status = ERR;
    }

    if (status == EXPONENT)
	yylval.ival = sign * exponent;

    BEGIN INITIAL;
    return status;
}

<SHIFT_SEEN>{year_broken}-{month}-{day}(T|{space}*) {
    BEGIN DATE_SEEN;
    return decodeDate((char*)yytext, "%d-%d-%d", &yylval.rval);
}

<SHIFT_SEEN>{year_broken}-[0-9]{2}[0-9]+ {
    snprintf(yylval.error_msg, sizeof(yylval.error_msg),
             "Too many digits after '-' in broken date \"%s\" "
             "(use YYYY-MM or YYYY-MM-DD)", yytext);
    return ERR;
}

<SHIFT_SEEN>{year_broken}-{month}(T|{space}*) {
    BEGIN DATE_SEEN;
    return decodeDate((char*)yytext, "%d-%d", &yylval.rval);
}

<SHIFT_SEEN>{packed_date}(T|{space}*) {
    if (_isTime) {
        BEGIN DATE_SEEN;
        return decodePackedDate((char*)yytext, &yylval.rval);
    }
    else {
        BEGIN INITIAL;
        return decodeReal((char*)yytext, &yylval.rval);
    }
}

<SHIFT_SEEN>[+-]?[0-9]{8,}-[0-9] {
    snprintf(yylval.error_msg, sizeof(yylval.error_msg),
             "Invalid date: year has too many digits (max 7 digits allowed in broken format)");
    return ERR;
}

<SHIFT_SEEN>[0-9]{4}\.[0-9]{1,2}\.[0-9]{1,2} {
    snprintf(yylval.error_msg, sizeof(yylval.error_msg),
             "Invalid date separator: use '-' not '.' (expected YYYY-MM-DD format)");
    return ERR;
}

<DATE_SEEN>{broken_clock}{space}*    |
<DATE_SEEN>{packed_clock}{space}*    {
    double sec = 0.0;
    if (!decodeClockFlexible((const char*)yytext, &sec, yylval.error_msg)) {
        return ERR;
    }
    yylval.rval = sec;
    BEGIN(CLOCK_SEEN);
    return CLOCK;
}

<CLOCK_SEEN>{broken_tz_clock}{space}*    |
<CLOCK_SEEN>{packed_tz_clock}{space}*    {
    double off = 0.0;
    if (!decodeTzOffsetFlexible((const char*)yytext, &off, yylval.error_msg)) {
        return ERR;
    }
    yylval.rval = off;
    BEGIN INITIAL;
    return TZ_CLOCK;
}

<CLOCK_SEEN>Z {
    BEGIN INITIAL;
    return Z_TOK;
}

<CLOCK_SEEN>GMT {
    BEGIN INITIAL;
    return GMT_TOK;
}

<CLOCK_SEEN>UTC {
    BEGIN INITIAL;
    return UTC_TOK;
}

<CLOCK_SEEN>[+-][0-9]{1,2}\.[0-9]+ {
    snprintf(yylval.error_msg, sizeof(yylval.error_msg),
             "Invalid timezone separator: use ':' not '.' (expected +HH:MM format)");
    return ERR;
}

<CLOCK_SEEN>[+-][0-9]{1,2}: {
    snprintf(yylval.error_msg, sizeof(yylval.error_msg),
             "Incomplete timezone offset (expected minutes after ':')");
    return ERR;
}

<CLOCK_SEEN>[A-Za-z]+ {
    snprintf(yylval.error_msg, sizeof(yylval.error_msg), "Unknown timezone identifier '%s' (expected Z, GMT, UTC, or numeric offset like +05:30)", yytext);
    return ERR;
}

<CLOCK_SEEN>[0-9]+ {
    snprintf(yylval.error_msg, sizeof(yylval.error_msg), "Timezone offset must include sign (use +%s or -%s)", yytext, yytext);
    return ERR;
}

<DATE_SEEN>Z {
    BEGIN INITIAL;
    return Z_TOK;
}

<DATE_SEEN>GMT {
    snprintf(yylval.error_msg, sizeof(yylval.error_msg), "GMT timezone requires a time (e.g., '2024-01-01 00:00GMT')");
    return ERR;
}

<DATE_SEEN>UTC {
    snprintf(yylval.error_msg, sizeof(yylval.error_msg), "UTC timezone requires a time (e.g., '2024-01-01 00:00UTC')");
    return ERR;
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
        snprintf(yylval.error_msg, sizeof(yylval.error_msg),
             "Integer overflow or invalid integer '%s'", yytext);
        status = ERR;
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
