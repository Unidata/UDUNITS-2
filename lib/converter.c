/*
 * Value converters for the udunits(3) library.
 *
 * $Id: converter.c,v 1.3 2006/12/21 20:52:37 steve Exp $
 */

/*LINTLIBRARY*/

#ifndef	_XOPEN_SOURCE
#   define _XOPEN_SOURCE 500
#endif

#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "converter.h"		/* this module's API */

typedef struct {
    cvConverter*	(*clone)(cvConverter*);
    double		(*convertDouble)
	(const cvConverter*, double);
    float*		(*convertFloats)
	(const cvConverter*, const float*, size_t, float*);
    double*		(*convertDoubles)
	(const cvConverter*, const double*, size_t, double*);
    int			(*getExpression)
	(const cvConverter*, char* buf, size_t, const char*);
    void		(*free)(cvConverter*);
} ConverterOps;

typedef struct {
    ConverterOps*	ops;
} ReciprocalConverter;

typedef struct {
    ConverterOps*	ops;
    double		value;
} ScaleConverter;

typedef struct {
    ConverterOps*	ops;
    double		value;
} OffsetConverter;

typedef struct {
    ConverterOps*	ops;
    double		slope;
    double		intercept;
} GalileanConverter;

typedef struct {
    ConverterOps*	ops;
    double		logE;
} LogConverter;

typedef struct {
    ConverterOps*	ops;
    double		base;
} ExpConverter;

typedef struct {
    ConverterOps*	ops;
    cvConverter*	first;
    cvConverter*	second;
} CompositeConverter;

union cvConverter {
    ConverterOps*	ops;
    ScaleConverter	scale;
    OffsetConverter	offset;
    GalileanConverter	galilean;
    LogConverter	log;
    ExpConverter	exp;
    CompositeConverter	composite;
};

#define CV_CLONE(conv)		((conv)->ops->clone(conv))

#define IS_TRIVIAL(conv)	((conv)->ops == &trivialOps)
#define IS_RECIPROCAL(conv)	((conv)->ops == &reciprocalOps)
#define IS_SCALE(conv)		((conv)->ops == &scaleOps)
#define IS_OFFSET(conv)		((conv)->ops == &offsetOps)
#define CV_IS_GALILEAN(conv)	((conv)->ops == &galileanOps)
#define CV_IS_LOG(conv)		((conv)->ops == &logOps)


static void
nonFree(
    cvConverter* const conv)
{
}


static void
cvSimpleFree(
    cvConverter* const conv)
{
    free(conv);
}


static int
cvNeedsParentheses(
    const char* const	string)
{
    return strpbrk(string, " \t") != NULL &&
	(string[0] != '(' || string[strlen(string)-1] != ')');
}


/*******************************************************************************
 * Trivial Converter:
 ******************************************************************************/

static cvConverter*
trivialClone(
    cvConverter* const	conv)
{
    return cvGetTrivial();
}


static double
trivialConvertDouble(
    const cvConverter* const	conv,
    const double		value)
{
    return value;
}


static float*
trivialConvertFloats(
    const cvConverter* const	conv,
    const float* const		in,
    const size_t		count,
    float* 			out)
{
    if (in == NULL || out == NULL) {
	out = NULL;
    }
    else {
	(void)memmove(out, in, count*sizeof(float));
    }

    return out;
}


static double*
trivialConvertDoubles(
    const cvConverter* const	conv,
    const double* const		in,
    const size_t		count,
    double* 			out)
{
    if (in == NULL || out == NULL) {
	out = NULL;
    }
    else {
	(void)memmove(out, in, count*sizeof(double));
    }

    return out;
}


static int
trivialGetExpression(
    const cvConverter* const	conv,
    char* const			buf,
    const size_t		max,
    const char* const		variable)
{
    return snprintf(buf, max, "%s", variable);
}


static ConverterOps	trivialOps = {
    trivialClone,
    trivialConvertDouble,
    trivialConvertFloats,
    trivialConvertDoubles,
    trivialGetExpression,
    nonFree};

static cvConverter	trivialConverter = {&trivialOps};


/*******************************************************************************
 * Reciprocal Converter:
 ******************************************************************************/

static cvConverter*
reciprocalClone(
    cvConverter* const	conv)
{
    return cvGetInverse();
}

static double
reciprocalConvertDouble(
    const cvConverter* const	conv,
    const double		value)
{
    return 1.0 / value;
}


static float*
reciprocalConvertFloats(
    const cvConverter* const	conv,
    const float* const		in,
    const size_t		count,
    float* 			out)
{
    if (in == NULL || out == NULL) {
	out = NULL;
    }
    else {
	size_t	i;

	if (in < out) {
	    for (i = count; i-- > 0;)
		out[i] = (float)(1.0f / in[i]);
	}
	else {
	    for (i = 0; i < count; i++)
		out[i] = (float)(1.0f / in[i]);
	}
    }

    return out;
}


static double*
reciprocalConvertDoubles(
    const cvConverter* const	conv,
    const double* const		in,
    const size_t		count,
    double* 			out)
{
    if (in == NULL || out == NULL) {
	out = NULL;
    }
    else {
	size_t	i;

	if (in < out) {
	    for (i = count; i-- > 0;)
		out[i] = 1.0 / in[i];
	}
	else {
	    for (i = 0; i < count; i++)
		out[i] = 1.0 / in[i];
	}
    }

    return out;
}


static int
reciprocalGetExpression(
    const cvConverter* const	conv,
    char* const			buf,
    const size_t		max,
    const char* const		variable)
{
    return
	cvNeedsParentheses(variable)
	? snprintf(buf, max, "1/(%s)", variable)
	: snprintf(buf, max, "1/%s", variable);
}


static ConverterOps	reciprocalOps = {
    reciprocalClone,
    reciprocalConvertDouble,
    reciprocalConvertFloats,
    reciprocalConvertDoubles,
    reciprocalGetExpression,
    nonFree};

static cvConverter	reciprocalConverter = {&reciprocalOps};


/*******************************************************************************
 * Scale Converter:
 ******************************************************************************/

static cvConverter*
scaleClone(
    cvConverter* const	conv)
{
    return cvGetScale(conv->scale.value);
}


static double
scaleConvertDouble(
    const cvConverter* const	conv,
    const double		value)
{
    return conv->scale.value * value;
}


static float*
scaleConvertFloats(
    const cvConverter* const	conv,
    const float* const		in,
    const size_t		count,
    float* 			out)
{
    if (conv == NULL || in == NULL || out == NULL) {
	out = NULL;
    }
    else {
	size_t	i;

	if (in < out) {
	    for (i = count; i-- > 0;)
		out[i] = (float)(conv->scale.value * in[i]);
	}
	else {
	    for (i = 0; i < count; i++)
		out[i] = (float)(conv->scale.value * in[i]);
	}
    }

    return out;
}


static double*
scaleConvertDoubles(
    const cvConverter* const	conv,
    const double* const		in,
    const size_t		count,
    double* 			out)
{
    if (conv == NULL || in == NULL || out == NULL) {
	out = NULL;
    }
    else {
	size_t	i;

	if (in < out) {
	    for (i = count; i-- > 0;)
		out[i] = conv->scale.value * in[i];
	}
	else {
	    for (i = 0; i < count; i++)
		out[i] = conv->scale.value * in[i];
	}
    }

    return out;
}


static int
scaleGetExpression(
    const cvConverter* const	conv,
    char* const			buf,
    const size_t		max,
    const char* const		variable)
{
    return
	cvNeedsParentheses(variable)
	? snprintf(buf, max, "%g*(%s)", conv->scale.value, variable)
	: snprintf(buf, max, "%g*%s", conv->scale.value, variable);
}


static ConverterOps	scaleOps = {
    scaleClone,
    scaleConvertDouble,
    scaleConvertFloats,
    scaleConvertDoubles,
    scaleGetExpression,
    cvSimpleFree};


/*******************************************************************************
 * Offset Converter:
 ******************************************************************************/

static cvConverter*
offsetClone(
    cvConverter* const	conv)
{
    return cvGetOffset(conv->offset.value);
}


static double
offsetConvertDouble(
    const cvConverter* const	conv,
    const double		value)
{
    return conv->offset.value + value;
}


static float*
offsetConvertFloats(
    const cvConverter* const	conv,
    const float* const		in,
    const size_t		count,
    float* 			out)
{
    if (conv == NULL || in == NULL || out == NULL) {
	out = NULL;
    }
    else {
	size_t	i;

	if (in < out) {
	    for (i = count; i-- > 0;)
		out[i] = (float)(conv->offset.value + in[i]);
	}
	else {
	    for (i = 0; i < count; i++)
		out[i] = (float)(conv->offset.value + in[i]);
	}
    }

    return out;
}


static double*
offsetConvertDoubles(
    const cvConverter* const	conv,
    const double* const		in,
    const size_t		count,
    double* 			out)
{
    if (conv == NULL || in == NULL || out == NULL) {
	out = NULL;
    }
    else {
	size_t	i;

	if (in < out) {
	    for (i = count; i-- > 0;)
		out[i] = conv->offset.value + in[i];
	}
	else {
	    for (i = 0; i < count; i++)
		out[i] = conv->offset.value + in[i];
	}
    }

    return out;
}


static int
offsetGetExpression(
    const cvConverter* const	conv,
    char* const			buf,
    const size_t		max,
    const char* const		variable)
{
    const int	oper = conv->offset.value < 0 ? '-' : '+';

    return
	cvNeedsParentheses(variable)
	    ? snprintf(buf, max, "(%s) %c %g", variable, oper, 
		fabs(conv->offset.value))
	    : snprintf(buf, max, "%s %c %g", variable, oper, 
		fabs(conv->offset.value));
}


static ConverterOps	offsetOps = {
    offsetClone,
    offsetConvertDouble,
    offsetConvertFloats,
    offsetConvertDoubles,
    offsetGetExpression,
    cvSimpleFree};


/*******************************************************************************
 * Galilean Converter:
 ******************************************************************************/

static cvConverter*
cvGalileanClone(
    cvConverter* const	conv)
{
    return cvGetGalilean(conv->galilean.slope, conv->galilean.intercept);
}


static double
galileanConvertDouble(
    const cvConverter* const	conv,
    const double		value)
{
    return conv->galilean.slope * value + conv->galilean.intercept;
}


static float*
galileanConvertFloats(
    const cvConverter* const	conv,
    const float* const		in,
    const size_t		count,
    float* 			out)
{
    if (conv == NULL || in == NULL || out == NULL) {
	out = NULL;
    }
    else {
	size_t	i;

	if (in < out) {
	    for (i = count; i-- > 0;)
		out[i] = (float)(conv->galilean.slope * in[i] +
		    conv->galilean.intercept);
	}
	else {
	    for (i = 0; i < count; i++)
		out[i] = (float)(conv->galilean.slope * in[i] +
		    conv->galilean.intercept);
	}
    }

    return out;
}


static double*
galileanConvertDoubles(
    const cvConverter* const	conv,
    const double* const		in,
    const size_t		count,
    double* 			out)
{
    if (conv == NULL || in == NULL || out == NULL) {
	out = NULL;
    }
    else {
	size_t	i;

	if (in < out) {
	    for (i = count; i-- > 0;)
		out[i] = conv->galilean.slope * in[i] + 
		    conv->galilean.intercept;
	}
	else {
	    for (i = 0; i < count; i++)
		out[i] = conv->galilean.slope * in[i] + 
		    conv->galilean.intercept;
	}
    }

    return out;
}


static int
galileanGetExpression(
    const cvConverter* const	conv,
    char* const			buf,
    const size_t		max,
    const char* const		variable)
{
    const int	oper = conv->galilean.intercept < 0 ? '-' : '+';

    return
	cvNeedsParentheses(variable)
	    ? snprintf(buf, max, "%g*(%s) %c %g", conv->galilean.slope,
		variable, oper, fabs(conv->galilean.intercept))
	    : snprintf(buf, max, "%g*%s %c %g", conv->galilean.slope, variable,
		oper, fabs(conv->galilean.intercept));
}


static ConverterOps	galileanOps = {
    cvGalileanClone,
    galileanConvertDouble,
    galileanConvertFloats,
    galileanConvertDoubles,
    galileanGetExpression,
    cvSimpleFree};


/*******************************************************************************
 * Logarithmic Converter:
 ******************************************************************************/

static cvConverter*
cvLogClone(
    cvConverter* const	conv)
{
    return cvGetLog(conv->log.logE);
}


static double
logConvertDouble(
    const cvConverter* const	conv,
    const double		value)
{
    return log(value) * conv->log.logE;
}


static float*
logConvertFloats(
    const cvConverter* const	conv,
    const float* const		in,
    const size_t		count,
    float* 			out)
{
    if (conv == NULL || in == NULL || out == NULL) {
	out = NULL;
    }
    else {
	size_t	i;

	if (in < out) {
	    for (i = count; i-- > 0;)
		out[i] = (float)(log(in[i]) * conv->log.logE);
	}
	else {
	    for (i = 0; i < count; i++)
		out[i] = (float)(log(in[i]) * conv->log.logE);
	}
    }

    return out;
}


static double*
logConvertDoubles(
    const cvConverter* const	conv,
    const double* const		in,
    const size_t		count,
    double* 			out)
{
    if (conv == NULL || in == NULL || out == NULL) {
	out = NULL;
    }
    else {
	size_t	i;

	if (in < out) {
	    for (i = count; i-- > 0;)
		out[i] = log(in[i]) * conv->log.logE;
	}
	else {
	    for (i = 0; i < count; i++)
		out[i] = log(in[i]) * conv->log.logE;
	}
    }

    return out;
}


static int
logGetExpression(
    const cvConverter* const	conv,
    char* const			buf,
    const size_t		max,
    const char* const		variable)
{
    const int	isUnity = conv->log.logE == 1.0;

    return 
	cvNeedsParentheses(variable)
	    ? (isUnity
		? snprintf(buf, max, "ln((%s)/%g)", variable,
		    conv->offset.value)
		: snprintf(buf, max, "%g*ln((%s)/%g)", conv->log.logE, variable,
		    conv->offset.value))
	    : (isUnity
		? snprintf(buf, max, "ln(%s/%g)", variable,
		    conv->offset.value)
		: snprintf(buf, max, "%g*ln(%s/%g)", conv->log.logE, variable,
		    conv->offset.value));
}


static ConverterOps	logOps = {
    cvLogClone,
    logConvertDouble,
    logConvertFloats,
    logConvertDoubles,
    logGetExpression,
    cvSimpleFree};


/*******************************************************************************
 * Exponential Converter:
 ******************************************************************************/

static cvConverter*
expClone(
    cvConverter* const	conv)
{
    return cvGetPow(conv->exp.base);
}


static double
expConvertDouble(
    const cvConverter* const	conv,
    const double		value)
{
    return pow(conv->exp.base, value);
}

static float*
expConvertFloats(
    const cvConverter* const	conv,
    const float* const		in,
    const size_t		count,
    float* 			out)
{
    if (conv == NULL || in == NULL || out == NULL) {
	out = NULL;
    }
    else {
	size_t	i;

	if (in < out) {
	    for (i = count; i-- > 0;)
		out[i] = (float)(pow(conv->exp.base, in[i]));
	}
	else {
	    for (i = 0; i < count; i++)
		out[i] = (float)(pow(conv->exp.base, in[i]));
	}
    }

    return out;
}


static double*
expConvertDoubles(
    const cvConverter* const	conv,
    const double* const		in,
    const size_t		count,
    double* 			out)
{
    if (conv == NULL || in == NULL || out == NULL) {
	out = NULL;
    }
    else {
	size_t	i;

	if (in < out) {
	    for (i = count; i-- > 0;)
		out[i] = pow(conv->exp.base, in[i]);
	}
	else {
	    for (i = 0; i < count; i++)
		out[i] = pow(conv->exp.base, in[i]);
	}
    }

    return out;
}


static int
expGetExpression(
    const cvConverter* const	conv,
    char* const			buf,
    const size_t		max,
    const char* const		variable)
{
    const int	isUnity = conv->exp.base == 1.0;

    return 
	cvNeedsParentheses(variable)
	    ? snprintf(buf, max, "pow(%g, (%s))", conv->exp.base, variable)
	    : snprintf(buf, max, "pow(%g, %s)", conv->exp.base, variable);
}


static ConverterOps	expOps = {
    expClone,
    expConvertDouble,
    expConvertFloats,
    expConvertDoubles,
    expGetExpression,
    cvSimpleFree};


/*******************************************************************************
 * Composite Converter:
 ******************************************************************************/

static cvConverter*
compositeClone(
    cvConverter* const	conv)
{
    return cvCombine(conv->composite.first, conv->composite.second);
}


static double
compositeConvertDouble(
    const cvConverter* const	conv,
    const double		value)
{
    return
	cvConvertDouble(conv->composite.second,
	    cvConvertDouble(((CompositeConverter*)conv)->first, value));
}


static float*
compositeConvertFloats(
    const cvConverter* const	conv,
    const float* const		in,
    const size_t		count,
    float* 			out)
{
    if (conv == NULL || in == NULL || out == NULL) {
	out = NULL;
    }
    else {
	out =
	    cvConvertFloats(
		conv->composite.second,
		cvConvertFloats(conv->composite.first, in, count, out),
		count,
		out);
    }

    return out;
}


static double*
compositeConvertDoubles(
    const cvConverter* const	conv,
    const double* const		in,
    const size_t		count,
    double* 			out)
{
    if (conv == NULL || in == NULL || out == NULL) {
	out = NULL;
    }
    else {
	out = 
	    cvConvertDoubles(
		conv->composite.second,
		cvConvertDoubles(conv->composite.first, in, count, out),
		count,
		out);
    }

    return out;
}


static void
compositeFree(
    cvConverter* const	conv)
{
    cvFree(conv->composite.first);
    cvFree(conv->composite.second);
    free(conv);
}


static int
compositeGetExpression(
    const cvConverter* const	conv,
    char* const			buf,
    const size_t		max,
    const char* const		variable)
{
    char	tmpBuf[132];
    int		nchar = cvGetExpression(conv->composite.first, buf, max,
	variable);

    if (nchar >= 0) {
	int	n;

	buf[max-1] = 0;

	if (cvNeedsParentheses(buf)) {
	    nchar = snprintf(tmpBuf, sizeof(tmpBuf), "(%s)", buf);
	}
	else {
	    (void)strncpy(tmpBuf, buf, sizeof(tmpBuf));

	    tmpBuf[sizeof(tmpBuf)-1] = 0;
	}

	n = cvGetExpression(conv->composite.second, buf, max, tmpBuf);

	nchar = n >= 0 ? nchar + n : n;
    }

    return nchar;
}


static ConverterOps	compositeOps = {
    compositeClone,
    compositeConvertDouble,
    compositeConvertFloats,
    compositeConvertDoubles,
    compositeGetExpression,
    compositeFree};


/*******************************************************************************
 * Public API:
 ******************************************************************************/

/*
 * Returns the trivial converter (i.e., y = x).
 * When finished with the converter, the client should pass the converter to
 * cvFree().
 *
 * Returns:
 *	The trivial converter.
 */
cvConverter*
cvGetTrivial()
{
    return &trivialConverter;
}


/*
 * Returns the reciprocal converter (i.e., y = 1/x).
 * When finished with the converter, the client should pass the converter to
 * cvFree().
 *
 * Returns:
 *	The reciprocal converter.
 */
cvConverter*
cvGetInverse()
{
    return &reciprocalConverter;
}


/*
 * Returns a converter that multiplies values by a number (i.e., y = ax).
 * When finished with the converter, the client should pass the converter to
 * cvFree().
 *
 * Arguments:
 *	slope	The number by which to multiply values.
 * Returns:
 *	NULL	Necessary memory couldn't be allocated.
 *	else	A converter that will multiply values by the given number.
 */
cvConverter*
cvGetScale(
    const double	slope)
{
    cvConverter*	conv;

    if (slope == 1) {
	conv = &trivialConverter;
    }
    else {
	conv = malloc(sizeof(ScaleConverter));

	if (conv != NULL) {
	    conv->ops = &scaleOps;
	    conv->scale.value = slope;
	}
    }

    return conv;
}


/*
 * Returns a converter that adds a number to values (i.e., y = x + b).
 * When finished with the converter, the client should pass the converter to
 * cvFree().
 *
 * Arguments:
 *	offset	The number to be added.
 * Returns:
 *	NULL	Necessary memory couldn't be allocated.
 *	else	A converter that adds the given number to values.
 */
cvConverter*
cvGetOffset(
    const double	offset)
{
    cvConverter*	conv;

    if (offset == 0) {
	conv = &trivialConverter;
    }
    else {
	conv = malloc(sizeof(OffsetConverter));

	if (conv != NULL) {
	    conv->ops = &offsetOps;
	    conv->offset.value = offset;
	}
    }

    return conv;
}


/*
 * Returns a Galilean converter (i.e., y = ax + b).
 * When finished with the converter, the client should pass the converter to
 * cvFree().
 *
 * Arguments:
 *	slope	The number by which to multiply values.
 *	offset	The number to be added.
 * Returns:
 *	NULL	Necessary memory couldn't be allocated.
 *	else	A Galilean converter corresponding to the inputs.
 */
cvConverter*
cvGetGalilean(
    const double	slope,
    const double	intercept)
{
    cvConverter*	conv;

    if (slope == 1) {
	conv = cvGetOffset(intercept);
    }
    else if (intercept == 0) {
	conv = cvGetScale(slope);
    }
    else {
	cvConverter*	tmp = malloc(sizeof(GalileanConverter));

	if (tmp != NULL) {
	    tmp->ops = &galileanOps;
	    tmp->galilean.slope = slope;
	    tmp->galilean.intercept = intercept;
	    conv = tmp;
	}
    }

    return conv;
}


/*
 * Returns a logarithmic converter (i.e., y = log(x/x0) in some base).
 * When finished with the converter, the client should pass the converter to
 * cvFree().
 *
 * Arguments:
 *	logE		The logarithm of "e" in the base of the desired 
 *			logarithm.  Must be positive.
 * Returns:
 *	NULL		"logE" is invalid or necessary memory couldn't be
 *			allocated.
 *	else		A logarithmic converter corresponding to the inputs.
 */
cvConverter*
cvGetLog(
    const double	logE)
{
    cvConverter*	conv;

    if (logE <= 0) {
	conv = NULL;
    }
    else {
	conv = malloc(sizeof(LogConverter));

	if (conv != NULL) {
	    conv->ops = &logOps;
	    conv->log.logE = logE;
	}
    }

    return conv;
}


/*
 * Returns an exponential converter (i.e., y = pow(b, x) in some base "b").
 * When finished with the converter, the client should pass the converter to
 * cvFree().
 *
 * Arguments:
 *	base		The desired base.  Must be positive.
 * Returns:
 *	NULL		"base" is invalid or necessary memory couldn't be
 *			allocated.
 *	else		An exponential converter corresponding to the inputs.
 */
cvConverter*
cvGetPow(
    const double	base)
{
    cvConverter*	conv;

    if (base <= 0) {
	conv = NULL;
    }
    else {
	conv = malloc(sizeof(ExpConverter));

	if (conv != NULL) {
	    conv->ops = &expOps;
	    conv->exp.base = base;
	}
    }

    return conv;
}


/*
 * Returns a converter corresponding to the sequential application of two
 * other converters.  The returned converter should be passed to cfFree() when
 * it is no longer needed.
 *
 * Arguments:
 *	first	The converter to be applied first.  May be passed to cvFree()
 *		upon return.
 *	second	The converter to be applied second.  May be passed to cvFree()
 *		upon return.
 * Returns:
 *	NULL	Either "first" or "second" is NULL or necessary memory couldn't
 *		be allocated.
 *      else    A converter corresponding to the sequential application of the
 *              given converters.  If one of the input converters is the trivial
 *              converter, then the returned converter will be the other input
 *              converter.
 */
cvConverter*
cvCombine(
    cvConverter* const	first,
    cvConverter* const	second)
{
    cvConverter*	conv;

    if (first == NULL || second == NULL) {
	conv = NULL;
    }
    else if (IS_TRIVIAL(first)) {
	conv = CV_CLONE(second);
    }
    else if (IS_TRIVIAL(second)) {
	conv = CV_CLONE(first);
    }
    else {
	conv = NULL;

	if (IS_RECIPROCAL(first)) {
	    if (IS_RECIPROCAL(second)) {
		conv = cvGetTrivial();
	    }
	}
	else if (IS_SCALE(first)) {
	    if (IS_SCALE(second)) {
		conv = cvGetScale(first->scale.value * second->scale.value);
	    }
	    else if (IS_OFFSET(second)) {
		conv = cvGetGalilean(first->scale.value, second->offset.value);
	    }
	    else if (CV_IS_GALILEAN(second)) {
		conv = cvGetGalilean(
		    first->scale.value * second->galilean.slope, 
		    second->galilean.intercept);
	    }
	    else if (CV_IS_LOG(second)) {
		conv = cvGetLog(first->scale.value * second->log.logE);
	    }
	}
	else if (IS_OFFSET(first)) {
	    if (IS_SCALE(second)) {
		conv = cvGetGalilean(second->scale.value, 
		    first->offset.value * second->scale.value);
	    }
	    else if (IS_OFFSET(second)) {
		conv = cvGetOffset(first->offset.value + second->offset.value);
	    }
	    else if (CV_IS_GALILEAN(second)) {
		conv = cvGetGalilean(second->galilean.slope, 
		    first->offset.value * second->galilean.slope +
			second->galilean.intercept);
	    }
	}
	else if (CV_IS_GALILEAN(first)) {
	    if (IS_SCALE(second)) {
		conv = cvGetGalilean(
		    second->scale.value * first->galilean.slope,
		    second->scale.value * first->galilean.intercept);
	    }
	    else if (IS_OFFSET(second)) {
		conv = cvGetGalilean(first->galilean.slope,
		    first->galilean.intercept + second->offset.value);
	    }
	    else if (CV_IS_GALILEAN(second)) {
		conv = cvGetGalilean(
		    second->galilean.slope * first->galilean.slope,
		    second->galilean.slope * first->galilean.intercept +
			second->galilean.intercept);
	    }
	}
	else if (CV_IS_LOG(first)) {
	    if (IS_SCALE(second))
		conv = cvGetLog(first->log.logE * second->scale.value);
	}

	if (conv == NULL) {
	    /*
	     * General case: create a composite converter.
	     */
	    cvConverter*	tmp = malloc(sizeof(CompositeConverter));

	    if (tmp != NULL) {
		tmp->ops = &compositeOps;
		tmp->composite.first = CV_CLONE(first);
		tmp->composite.second = CV_CLONE(second);
		conv = tmp;
	    }
	}
    }

    return conv;
}


/*
 * Frees resources associated with a converter.  Use of the converter argument
 * subsequent to this function may result in undefined behavior.
 *
 * Arguments:
 *	conv	The converter to have its resources freed or NULL.  The
 *		converter must have been returned by this module.
 */
void
cvFree(
    const cvConverter* const	conv)
{
    conv->ops->free((cvConverter*)conv);
}


/*
 * Converts a float.
 *
 * Arguments:
 *	converter	Pointer to the converter.
 *	value		The value to be converted.
 * Returns:
 *	The converted value.
 */
float
cvConvertFloat(
    const cvConverter*	converter,
    const float		value)
{
    return (float)converter->ops->convertDouble(converter, value);
}


/*
 * Converts a double.
 *
 * Arguments:
 *	converter	Pointer to the converter.
 *	value		The value to be converted.
 * Returns:
 *	The converted value.
 */
double
cvConvertDouble(
    const cvConverter*	converter,
    const double	value)
{
    return converter->ops->convertDouble(converter, value);
}


/*
 * Converts an array of floats.
 *
 * Arguments:
 *	converter	Pointer to the converter.
 *	in		Pointer to the values to be converted.  The array may
 *			overlap "out".
 *	count		The number of values to be converted.
 *	out		Pointer to the output array for the converted values.
 *			The array may overlap "in".
 * Returns:
 *	NULL		"converter", "in", or "out" is NULL.
 *	else		Pointer to the output array, "out".
 */
float*
cvConvertFloats(
    const cvConverter*	converter,
    const float* const	in,
    const size_t	count,
    float*		out)
{
    if (converter == NULL || in == NULL || out == NULL) {
	out = NULL;
    }
    else {
        out = converter->ops->convertFloats(converter, in, count, out);
    }

    return out;
}


/*
 * Converts an array of doubles.
 *
 * Arguments:
 *	converter	Pointer to the converter.
 *	in		Pointer to the values to be converted.  The array may
 *			overlap "out".
 *	count		The number of values to be converted.
 *	out		Pointer to the output array for the converted values.
 *			The array may overlap "in".
 * Returns:
 *	NULL		"converter", "in", or "out" is NULL.
 *	else		Pointer to the output array, "out".
 */
double*
cvConvertDoubles(
    const cvConverter*	converter,
    const double* const	in,
    const size_t	count,
    double*		out)
{
    if (converter == NULL || in == NULL || out == NULL) {
	out = NULL;
    }
    else {
    	out = converter->ops->convertDoubles(converter, in, count, out);
    }

    return out;
}


/*
 * Returns a string expression representation of a converter.
 *
 * Arguments:
 *	conv		The converter.
 *	buf		The buffer into which to write the expression.
 *	max		The size of the buffer.
 *	variable	The string to be used as the input value for the
 *			converter.
 * RETURNS
 *	<0	An error was encountered.
 *	else	The number of bytes formatted excluding the terminating null.
 */
int
cvGetExpression(
    const cvConverter* const	conv,
    char* const			buf,
    size_t			max,
    const char* const		variable)
{
    return conv->ops->getExpression(conv, buf, max, variable);
}
