/*
 * $Id: converter.h,v 1.5 2007/01/04 17:13:01 steve Exp $
 *
 * Public header-file for the Unidata units(3) library.
 */

#ifndef CV_CONVERTER_H_INCLUDED
#define CV_CONVERTER_H_INCLUDED

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef union cvConverter	cvConverter;

/*
 * Returns the trivial converter (i.e., y = x).
 * When finished with the converter, the client should pass the converter to
 * cvFree().
 * RETURNS:
 *	The trivial converter.
 */
cvConverter*
cvGetTrivial(void);

/*
 * Returns the reciprocal converter (i.e., y = 1/x).
 * When finished with the converter, the client should pass the converter to
 * cvFree().
 * RETURNS:
 *	The reciprocal converter.
 */
cvConverter*
cvGetInverse(void);

/*
 * Returns a scaling converter (i.e., y = ax).
 * When finished with the converter, the client should pass the converter to
 * cvFree().
 * RETURNS:
 *	The scaling converter.
 */
cvConverter*
cvGetScale(
    const double	slope);

/*
 * Returns a converter that adds a number to values (i.e., y = x + b).
 * When finished with the converter, the client should pass the converter to
 * cvFree().
 * ARGUMENTS:
 *	intercept	The number to be added.
 * RETURNS:
 *	NULL	Necessary memory couldn't be allocated.
 *	else	A converter that adds the given number to values.
 */
cvConverter*
cvGetOffset(
    const double	intercept);

/*
 * Returns a Galilean converter (i.e., y = ax + b).
 * When finished with the converter, the client should pass the converter to
 * cvFree().
 * ARGUMENTS:
 *	slope		The number by which to multiply values.
 *	intercept	The number to be added.
 * RETURNS:
 *	NULL	Necessary memory couldn't be allocated.
 *	else	A Galilean converter corresponding to the inputs.
 */
cvConverter*
cvGetGalilean(
    const double	slope,
    const double	intercept);

/*
 * Returns a logarithmic converter (i.e., y = log(x) in some base).
 * When finished with the converter, the client should pass the converter to
 * cvFree().
 * ARGUMENTS:
 *	logE		The logarithm of "e" in the base of the desired 
 *			logarithm.  Must be positive.
 * RETURNS:
 *	NULL		The logarithm of "e" is not positive or necessary
 *			memory couldn't be allocated.
 *	else		A logarithmic converter corresponding to the inputs.
 */
cvConverter*
cvGetLog(
    const double	logE);

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
    const double	base);

/*
 * Returns a converter corresponding to the sequential application of two
 * other converters.
 * ARGUMENTS:
 *	first	The converter to be applied first.
 *	second	The converter to be applied second.
 * RETURNS:
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
    cvConverter* const	second);

/*
 * Frees resources associated with a converter.
 * ARGUMENTS:
 *	conv	The converter to have its resources freed or NULL.
 */
void
cvFree(
    cvConverter* const	conv);

/*
 * Converts a float.
 * ARGUMENTS:
 *	converter	The converter.
 *	value		The value to be converted.
 * RETURNS:
 *	The converted value.
 */
float
cvConvertFloat(
    const cvConverter*	converter,
    const float		value);

/*
 * Converts a double.
 * ARGUMENTS:
 *	converter	The converter.
 *	value		The value to be converted.
 * RETURNS:
 *	The converted value.
 */
double
cvConvertDouble(
    const cvConverter*	converter,
    const double	value);

/*
 * Converts an array of floats.
 * ARGUMENTS:
 *	converter	The converter.
 *	in		The values to be converted.
 *	count		The number of values to be converted.
 *	out		The output array for the converted values.  May
 *			be the same array as "in" or overlap it.
 * RETURNS:
 *	NULL	"out" is NULL.
 *	else	A pointer to the output array.
 */
float*
cvConvertFloats(
    const cvConverter*	converter,
    const float* const	in,
    const size_t	count,
    float*		out);

/*
 * Converts an array of doubles.
 * ARGUMENTS:
 *	converter	The converter.
 *	in		The values to be converted.
 *	count		The number of values to be converted.
 *	out		The output array for the converted values.  May
 *			be the same array as "in" or overlap it.
 * RETURNS:
 *	NULL	"out" is NULL.
 *	else	A pointer to the output array.
 */
double*
cvConvertDoubles(
    const cvConverter*	converter,
    const double* const	in,
    const size_t	count,
    double*		out);

/*
 * Returns a string representation of a converter.
 * ARGUMENTS:
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
    const char* const		variable);

#ifdef __cplusplus
}
#endif

#endif
