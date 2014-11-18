/*
 * Copyright 2013 University Corporation for Atmospheric Research. All rights
 * reserved.
 *
 * This file is part of the UDUNITS-2 package.  See the file COPYRIGHT
 * in the top-level source-directory of the package for copying and
 * redistribution conditions.
 */
/*
 * This program prints definitions of units of physical quantities and converts
 * values between such units.
 */

#include "config.h"

#ifndef	_XOPEN_SOURCE
#   define _XOPEN_SOURCE 500
#endif

#ifdef _MSC_VER
#include "XGetOpt.h"
#endif

#include <errno.h>
#include <ctype.h>
#ifndef _MSC_VER
#include <libgen.h>
#endif
#include <limits.h>
#include <sys/types.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
#include <strings.h>
#include <unistd.h>
#endif
#include <udunits2.h>

#ifndef _POSIX_MAX_INPUT
#   define _POSIX_MAX_INPUT 255
#endif

static const char*      _cmdHave; /* command-line "have" unit specification */
static const char*      _cmdWant; /* command-line "want" unit specification */
static int		_reveal; /* reveal problems with unit database? */
static int		_encodingSet; /* is the character encoding set? */
static ut_encoding	_encoding; /* the character encoding to use */
static char             _progname[1024];
static const char*	_xmlPath = NULL; /* use default path */
static ut_system*	_unitSystem;
static double           _haveUnitAmount; /* amount of "have" unit */
static char		_haveUnitSpec[_POSIX_MAX_INPUT+1]; /* "have" unit minus
                                                              amount */
static char		_wantSpec[_POSIX_MAX_INPUT+1]; /* complete "want" unit
                                                          specification */
static ut_unit*		_haveUnit; /* "have" unit minus amount */
static ut_unit*		_wantUnit; /* complete "want" unit */
static int		_wantDefinition; /* "have" unit definition desired? */
static int		_formattingOptions = UT_DEFINITION;
static int		_exitStatus = EXIT_FAILURE;


static void
usage(void)
{
    (void)fprintf(stderr,
"Usage:\n"
"    %s -h\n"
"    %s [-A|-L|-U] [-r] [-H have] [-W want] [XML_file]\n"
"\n"
"where:\n"
"    -A         Use ASCII encoding (default).\n"
"    -L         Use ISO-8859-1 (ISO Latin-1) encoding.\n"
"    -U         Use UTF-8 encoding.\n"
"    -h         Help.  Print this message.\n"
"    -r         Reveal any problems in the database.\n"
"    -H have    Use \"have\" unit for conversion. Default is reply to prompt.\n"
"    -W want    Use \"want\" unit for conversion. Empty string requests\n"
"               definition of \"have\" unit. Default is reply to prompt.\n"
"    XML_file   XML database file. Default is \"%s\".\n",
        _progname, _progname, DEFAULT_UDUNITS2_XML_PATH);
}

/**
 * Prints an error-message to the standard error stream.
 *
 * @param format        The format for the error-message. It shouldn't have a
 *                      trailing newline.
 * @param ...           Arguments referenced by the format.
 */
static void
errMsg(
    const char* const   format,
    ...)
{
    (void)fprintf(stderr, "%s: ", _progname);
    {
        va_list     ap;

        va_start(ap, format);
        (void)vfprintf(stderr, format, ap);
        va_end(ap);
    }
    (void)fputc('\n', stderr);
}


static int
decodeCommandLine(
    int                 argc,
    char* const*        argv)
{
    int		c;
    int		success = 0;

#ifndef _MSC_VER
    char* filename = basename(argv[0]);

    if (strlen(filename)+1 > sizeof(_progname))
        filename = "udunits2";

    (void)strcpy(_progname, filename);
#else
    {
        size_t len = strlen(argv[0]);
        char*  m_fname = (char*)malloc(sizeof(char)*(len+1));
        char*  m_ext = (char*)malloc(sizeof(char)*(len+1));

        _splitpath(argv[0], NULL, NULL, m_fname, m_ext);

        if (strlen(m_fname)+1 > sizeof(_progname))
            (void)strcpy(_progname, "udunits2");
        else
            (void)strcpy(_progname, m_fname);

        free(m_fname);
        free(m_ext);
    }
#endif

    while ((c = getopt(argc, argv, "ALUhrH:W:")) != -1) {
	switch (c) {
	    case 'A':
		_encoding = UT_ASCII;
		_encodingSet = 1;
		continue;
            case 'H':
                _cmdHave = optarg;
                continue;
            case 'W':
                _cmdWant = optarg;
                continue;
	    case 'L':
		_encoding = UT_LATIN1;
		_encodingSet = 1;
		continue;
	    case 'U':
		_encoding = UT_UTF8;
		_encodingSet = 1;
		continue;
	    case 'r':
		_reveal = 1;
		continue;
	    case 'h':
		_exitStatus = EXIT_SUCCESS;
		/*FALLTHROUGH*/
	    case '?':
		usage();
		break;
	    default:
		errMsg("Unknown option \"%c\"", c);
		usage();
	}

	break;
    }

    if (c == -1) {
        if (optind < argc)
            _xmlPath = argv[optind];

        success = 1;
    }

    return success;
}


/**
 * Returns a lower-case copy of a NUL-terminated string.
 *
 * @param string        The NUL-terminated string to be copied.
 * @retval NULL         The string couldn't be copied. An error-message is
 *                      printed to the standard error stream.
 * @return              A NUL-terminated lower-case copy of the string. The
 *                      caller should call free() on the string when it is no
 *                      longer needed.
 * @raise SIGSEGV       if "string" is NULL.
 */
static char*
duplower(
    const char*         string)
{
    char* const         copy = malloc(strlen(string)+1);

    if (copy == NULL) {
        errMsg("Couldn't copy string \"%s\": %s", string, strerror(errno));
    }
    else {
        char*   cp = copy;

        while (*cp++ = tolower(*string++))
            ; /* empty */
    }

    return copy;
}


/**
 * Sets the character encoding from a (case insensitive) string specification
 * of the encoding that might be embedded in a larger string.
 *
 * @param value         The string specification of the encoding. Contains
 *                      one of the substrings "ascii", "latin1", "latin?1",
 *                      "8859", "8859?1", "utf8", or "utf?8" (where the '?'
 *                      is one of the characters ' ', '-', '_', or '.'). If the
 *                      string contains more than one of these substrings, then
 *                      the result is unspecified.
 * @return              Whether or not the character encoding was set from the
 *                      given string specification. 0 means no; otherwise, yes.
 */
static int
setEncodingFromEmbeddedString(
    char*	value)
{
    char*       lowerValue = duplower(value);
    int         success = 0;

    if (lowerValue != NULL) {
        typedef struct {
            const char*       string;
            const ut_encoding encoding;
        } Entry;
        static const Entry entries[] = {
            {"ascii",   UT_ASCII},
            {"latin1",  UT_LATIN1},
            {"latin 1", UT_LATIN1},
            {"latin-1", UT_LATIN1},
            {"latin_1", UT_LATIN1},
            {"latin.1", UT_LATIN1},
            {"8859 1",  UT_LATIN1},
            {"8859-1",  UT_LATIN1},
            {"8859_1",  UT_LATIN1},
            {"8859.1",  UT_LATIN1},
            {"utf8",    UT_UTF8},
            {"utf 8",   UT_UTF8},
            {"utf-8",   UT_UTF8},
            {"utf_8",   UT_UTF8},
            {"utf.8",   UT_UTF8}
        };
        const Entry* entry;

        for (entry = entries;
                entry < entries + sizeof(entries)/sizeof(entries[0]);
                entry++) {
            if (strstr(lowerValue, entry->string) != NULL) {
                _encoding = entry->encoding;
                _encodingSet = 1;
                success = 1;
                break;
            }
        }

        free(lowerValue);
    }

    return success;
}


/**
 * Sets the character encoding from a (case insensitive but exact) string
 * specification of the encoding.
 *
 * @param value         The string specification of the encoding. One of
 *                      "c" or "posix".
 * @return              Whether or not the encoding was set. 0 means no;
 *                      otherwise, yes.
 */
static int
setEncodingFromExactString(
    char*	value)
{
    if (strcasecmp(value, "c") == 0 || strcasecmp(value, "posix") == 0) {
        _encoding = UT_ASCII;
        _encodingSet = 1;
        return 1;
    }

    return 0;
}


/**
 * Sets the character encoding from a (case insensitive) string specification
 * of the encoding.
 *
 * @param value         The string specification of the encoding or NULL. If
 *                      not NULL, then one of "c" or "posix", or a string that
 *                      contains one of the substrings "ascii", "latin1",
 *                      "latin?1", "8859", "8859?1", "utf8", or "utf?8" (where
 *                      the '?' is one of the characters ' ', '-', '_', or '.').
 *                      If the string contains multiple instances of the
 *                      substrings, then the result is unspecified.
 */
static void
setEncoding(
    char*	value)
{
    if (value != NULL) {
        if (!setEncodingFromExactString(value))
            (void)setEncodingFromEmbeddedString(value);
    }
}


static int
ensureEncodingSet()
{
    if (!_encodingSet) {
	setEncoding(getenv("LC_ALL"));

	if (!_encodingSet) {
	    setEncoding(getenv("LC_CTYPE"));

	    if (!_encodingSet) {
		setEncoding(getenv("LANG"));

		if (!_encodingSet) {
		    errMsg("Character encoding not specified and not settable "
                        "from environment variables LC_ALL, LC_CTYPE, or LANG. "
                        "Assuming ASCII encoding.");

		    setEncoding("ASCII");
		}
	    }
	}
    }

    if (_encodingSet)
	_formattingOptions |= _encoding;

    return _encodingSet;
}


static int
readXmlDatabase(void)
{
    int		success = 0;

    if (!_reveal)
        ut_set_error_message_handler(ut_ignore);

    _unitSystem = ut_read_xml(_xmlPath);

    ut_set_error_message_handler(ut_write_to_stderr);

    if (_unitSystem != NULL) {
        success = 1;
    }
    else {
        ut_status	status;

        errMsg("Couldn't initialize unit-system from database \"%s\": %s",
                ut_get_path_xml(_xmlPath, &status), strerror(errno));
    }

    return success;
}


/*
 * Prompt the user and get a specification.
 */
static int
getSpec(
    const char* const	prompt,
    char* const		spec,
    const size_t	size)
{
    int		nbytes = -1;		/* failure */

    if (fputs(prompt, stdout) == EOF) {
	errMsg("Couldn't write prompt: %s", strerror(errno));
    } else if (fgets(spec, size, stdin) == NULL) {
	putchar('\n');

	if (feof(stdin)) {
	    _exitStatus = EXIT_SUCCESS;
	} else {
	    errMsg("Couldn't read from standard input: %s", strerror(errno));
	}
    } else {
	/*
	 * Trim any whitespace from the specification.
	 */
	(void)ut_trim(spec, _encoding);

        nbytes = strlen(spec);
    }

    return nbytes;
}


static int
decodeInput(
    const char* const   input)
{
    int	success = 0;

    ut_free(_haveUnit);

    if (sscanf(input, "%lg %[^ \t\n]", &_haveUnitAmount, _haveUnitSpec) == 2) {
        _haveUnit = ut_parse(_unitSystem, _haveUnitSpec, _encoding);
        if (_haveUnit != NULL)
            success = 1;
    }

    if (!success) {
        _haveUnitAmount = 1;
        (void)strncpy(_haveUnitSpec, input, sizeof(_haveUnitSpec));
        _haveUnitSpec[sizeof(_haveUnitSpec)-1] = 0;
        _haveUnit = ut_parse(_unitSystem, _haveUnitSpec, _encoding);
        if (_haveUnit == NULL) {
            errMsg("Don't recognize \"%s\"", _haveUnitSpec);
        }
        else {
            success = 1;
        }
    }

    return success;
}


static int
getInputValue(void)
{
    int		success = 0;

    if (_cmdHave) {
        static int      initialized = 0;

        if (initialized) {
            if (_cmdWant == NULL) {
                /*
                 * Multiple, prompt-driven conversions desired.
                 */
                success = 1;
            }
            else {
                /*
                 * Single, previous, command-line-driven conversion desired.
                 */
                success = 0;
            	_exitStatus = EXIT_SUCCESS;
            }
        }
        else {
            success = decodeInput(_cmdHave);
            initialized = 1;
        }
    }
    else {
        for (;;) {
            char    buf[sizeof(_haveUnitSpec)];
            int	nbytes = getSpec("You have: ", buf, sizeof(buf));

            if (nbytes < 0)
                break;

            if (nbytes > 0) {
                success = decodeInput(buf);

                if (success)
                    break;
            }
        }
    }

    return success;
}


static int
decodeOutput(
    const char* const   buf)
{
    int         success = 0;
    size_t	nbytes = strlen(buf);

    if (nbytes == 0) {
        _wantDefinition = 1;
        success = 1;
    }
    else {
        ut_free(_wantUnit);

        _wantDefinition = 0;
        _wantUnit = ut_parse(_unitSystem, buf, _encoding);

        if (_wantUnit == NULL) {
            errMsg("Don't recognize \"%s\"", buf);
        }
        else {
            success = 1;
        }
    }

    return success;
}


static int
getOutputRequest(void)
{
    int		success = 0;

    if (_cmdWant) {
        static int      initialized = 0;

        if (initialized) {
            success = 1;
        }
        else {
            (void)strncpy(_wantSpec, _cmdWant, sizeof(_wantSpec));
            _wantSpec[sizeof(_wantSpec)-1] = 0;

            success = decodeOutput(_wantSpec);
            initialized = 1;
        }
    }
    else {
        for (;;) {
            int	nbytes = getSpec("You want: ", _wantSpec, sizeof(_wantSpec));

            if (nbytes < 0)
                break;

            success = decodeOutput(_wantSpec);

            if (success)
                break;
        }
    }

    return success;
}


static int
handleRequest(void)
{
    int		success = 0;

    if (getInputValue()) {
	if (getOutputRequest()) {
	    if (_wantDefinition) {
                char	        buf[256];
                ut_unit*        unit = ut_scale(_haveUnitAmount, _haveUnit);
                int	        nbytes = ut_format(unit, buf, sizeof(buf),
                        _formattingOptions);

                if (nbytes >= sizeof(buf)) {
                    errMsg("Resulting unit specification is too long");
                }
                else if (nbytes >= 0) {
                    buf[nbytes] = 0;

                    (void)printf("    %s\n", buf);
                }

                ut_free(unit);
	    }
	    else if (!ut_are_convertible(_wantUnit, _haveUnit)) {
		errMsg("Units are not convertible");
	    }
	    else {
		cv_converter*	conv = ut_get_converter(_haveUnit, _wantUnit);

		if (conv == NULL) {
		    errMsg("Couldn't get unit converter");
		}
		else {
                    char        haveExp[_POSIX_MAX_INPUT+1];
                    char        exp[_POSIX_MAX_INPUT+1];
                    char        whiteSpace[] = " \t\n\r\f\v\xa0";
		    int	        needsParens =
                        strpbrk(_wantSpec, whiteSpace) != NULL;
                    int         n;

		    (void)printf(
			needsParens
			    ? "    %g %s = %g (%s)\n"
			    : "    %g %s = %g %s\n",
                        _haveUnitAmount,
			_haveUnitSpec,
			cv_convert_double(conv, _haveUnitAmount),
                        _wantSpec);

                    (void)sprintf(haveExp,
                        strpbrk(_haveUnitSpec, whiteSpace) ||
                                strpbrk(_haveUnitSpec, "/")
                            ? "(x/(%s))"
                            : "(x/%s)",
                        _haveUnitSpec);

                    n = cv_get_expression(conv, exp, sizeof(exp), haveExp);

                    if (n >= 0)
                        (void)printf(
                            strpbrk(_wantSpec, whiteSpace) ||
                                    strpbrk(_wantSpec, "/")
                                ?  "    x/(%s) = %*s\n"
                                :  "    x/%s = %*s\n",
                        _wantSpec, n, exp);

                    cv_free(conv);
		}
	    }

	    success = 1;
	}
    }

    return success;
}


int
main(
    const int		argc,
    char**	argv)
{
    if (decodeCommandLine(argc, argv)) {
    	if (ensureEncodingSet()) {
            if (readXmlDatabase()) {
                while (handleRequest())
                    ; /* EMPTY */
            }
    	}
    }

    return _exitStatus;
}
