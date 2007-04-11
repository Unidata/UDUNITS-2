/*
 * This module is thread-compatible but not thread-safe.  Multi-threaded
 * access must be externally synchronized.
 *
 * $Id: xml.c,v 1.7 2007/04/11 20:28:18 steve Exp $
 */

/*LINTLIBRARY*/

#ifndef	_XOPEN_SOURCE
#   define _XOPEN_SOURCE 500
#endif

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "expat.h"
#include "udunits2.h"

#define NAME_SIZE 128

typedef enum {
    INITIAL,
    UNIT_SYSTEM,
    PREFIX,
    VALUE,
    NAME,
    SYMBOL,
    UNIT,
    DIMENSIONLESS,
    DEF,
    SINGULAR,
    PLURAL,
    ALIASES
} ElementType;

static ElementType	elementStack[6] = {INITIAL};
static ElementType*	currElt = elementStack;
static const char*	_path;
static int		prefixAdded;
static ut_encoding	xmlEncoding;
static XML_Parser	parser = NULL;
static ut_system*	unitSystem = NULL;
static unsigned		skipDepth = 0;
static char*		text = NULL;
static size_t		nbytes = 0;
static double		value = 0;
static ut_encoding	encoding = UT_ASCII;
static ut_unit*		unit = NULL;
static int		isBase = 0;
static int		isDimensionless = 0;
static int		haveValue = 0;
static int		pluralNeeded = 1;
static char		singular[NAME_SIZE];
static int		singularSeen;


static void
clearText(void)
{
    if (text != NULL)
	*text = 0;

    nbytes = 0;
}


static void
accumulateText(
    void*		data,
    const char*		string,		/* input text in UTF-8 */
    int			len)
{
    char*	tmp = realloc(text, nbytes + len + 1);

    if (tmp == NULL) {
	ut_handle_error_message(strerror(errno));
	ut_handle_error_message("Couldn't reallocate %lu-byte text buffer",
	    nbytes+len+1);
	XML_StopParser(parser, 0);
    }
    else {
	text = tmp;

	if (encoding != UT_LATIN1) {
	    (void)strncpy(text + nbytes, string, len);

	    nbytes += len;
	}
	else {
	    int	i;

	    for (i = 0; i < len; ++i) {
		if (string[i] != '\xc2')
		    text[nbytes++] = string[i];
	    }
	}

	text[nbytes] = 0;
    }
}


static void
startUnitSystem(
    void*		data,
    const char**	atts)
{
    if (*currElt != INITIAL) {
	ut_handle_error_message("Wrong place for <unit-system> element");
	ut_set_status(UT_PARSE);
	XML_StopParser(parser, 0);
    }
    else {
	ut_free_system(unitSystem);

	unitSystem = ut_new_system();

	if (system == NULL) {
	    ut_handle_error_message("Couldn't create new unit-system");
	    XML_StopParser(parser, 0);
	}
    }
}


static void
endUnitSystem(
    void*		data)
{}


static void
startPrefix(
    void*		data,
    const char* const*	atts)
{
    if (*currElt != UNIT_SYSTEM) {
	ut_handle_error_message("Wrong place for <prefix> element");
    }
    else {
	prefixAdded = 0;
	haveValue = 0;
    }
}


static void
endPrefix(
    void*		data)
{
    if (!haveValue || !prefixAdded) {
	ut_handle_error_message("Prefix incompletely specified");
	XML_StopParser(parser, 0);
    }
    else {
	haveValue = 0;
	clearText();
    }
}


static void
startUnit(
    void*		data,
    const char**	atts)
{
    if (*currElt != UNIT_SYSTEM) {
	ut_handle_error_message("Wrong place for <unit> element");
	XML_StopParser(parser, 0);
    }
    else {
	ut_free(unit);
	unit = NULL;
	isBase = 0;
	isDimensionless = 0;
    }
}


static void
endUnit(
    void*		data)
{
    ut_free(unit);
    unit = NULL;
}


static void
startBase(
    void*		data,
    const char**	atts)
{
    if (*currElt != UNIT) {
	ut_handle_error_message("Wrong place for <base> element");
	XML_StopParser(parser, 0);
    }
    else {
	if (isDimensionless) {
	    ut_handle_error_message(
		"<dimensionless> and <base> are mutually exclusive");
	    XML_StopParser(parser, 0);
	}
	if (unit != NULL) {
	    ut_handle_error_message(
		"<base> and <def> are mutually exclusive");
	    XML_StopParser(parser, 0);
	}
	else if (isBase) {
	    ut_handle_error_message("<base> element already seen");
	    XML_StopParser(parser, 0);
	}
    }
}


static void
endBase(
    void*		data)
{
    unit = ut_new_base_unit(unitSystem);

    if (unit == NULL) {
	ut_handle_error_message("Couldn't create new base unit");
	XML_StopParser(parser, 0);
    }
    else {
	isBase = 1;
    }
}


static void
startDimensionless(
    void*		data,
    const char**	atts)
{
    if (*currElt != UNIT) {
	ut_handle_error_message("Wrong place for <dimensionless> element");
	XML_StopParser(parser, 0);
    }
    else {
	if (isBase) {
	    ut_handle_error_message(
		"<dimensionless> and <base> are mutually exclusive");
	    XML_StopParser(parser, 0);
	}
	else if (unit != NULL) {
	    ut_handle_error_message(
		"<dimensionless> and <def> are mutually exclusive");
	    XML_StopParser(parser, 0);
	}
	else if (isDimensionless) {
	    ut_handle_error_message("<dimensionless> element already seen");
	    XML_StopParser(parser, 0);
	}
    }
}


static void
endDimensionless(
    void*		data)
{
    unit = ut_new_dimensionless_unit(unitSystem);

    if (unit == NULL) {
	ut_handle_error_message("Couldn't create new dimensionless unit");
	XML_StopParser(parser, 0);
    }
    else {
	isDimensionless = 1;
    }
}


static int
setEncoding(
    const char**	atts)
{
    encoding = xmlEncoding;

    for (; *atts != NULL; atts += 2) {
	const char*	name = atts[0];
	const char*	value = atts[1];

	if (strcasecmp(name, "encoding") == 0) {
	    if (strcasecmp(value, "ascii") == 0 || 
		    strcasecmp(value, "us-ascii") == 0) {
		encoding = UT_ASCII;
	    }
	    else if (strcasecmp(value, "latin1") == 0 ||
		    strcasecmp(value, "latin-1") == 0 ||
		    strcasecmp(value, "iso-8859-1") == 0) {
		encoding = UT_LATIN1;
	    }
	    else if (strcasecmp(value, "utf8") == 0 ||
		    strcasecmp(value, "utf-8") == 0) {
		encoding = UT_UTF8;
	    }
	    else {
		ut_handle_error_message("Unknown character encoding \"%s\"",
		    value);
		XML_StopParser(parser, 0);
		break;
	    }
	}
    }

    return *atts == NULL;
}


static void
startDef(
    void*		data,
    const char**	atts)
{
    if (*currElt != UNIT) {
	ut_handle_error_message("Wrong place for <def> element");
	XML_StopParser(parser, 0);
    }
    else if (isBase) {
	ut_handle_error_message(
	    "<base> and <def> are mutually exclusive");
	XML_StopParser(parser, 0);
    }
    else if (isDimensionless) {
	ut_handle_error_message(
	    "<dimensionless> and <def> are mutually exclusive");
	XML_StopParser(parser, 0);
    }
    else if (unit != NULL) {
	ut_handle_error_message("<def> element already seen");
	XML_StopParser(parser, 0);
    }
    else {
	clearText();
	XML_SetCharacterDataHandler(parser, accumulateText);
    }
}


static void
endDef(
    void*		data)
{
    if (nbytes == 0) {
	ut_handle_error_message("Empty unit definition");
	XML_StopParser(parser, 0);
    }
    else {
	unit = ut_parse(unitSystem, text, xmlEncoding);

	if (unit == NULL) {
	    ut_handle_error_message("Couldn't parse unit specification \"%s\"",
		text);
	    XML_StopParser(parser, 0);
	}
    }
}


static int
xmlMapIdToUnit(
    const char*	id,
    ut_unit*	unit,
    int		isName)
{
    int		success = 0;		/* failure */
    ut_unit*	prev = ut_get_unit_by_name(unitSystem, id);

    if (prev == NULL)
	prev = ut_get_unit_by_symbol(unitSystem, id);

    if (prev != NULL) {
	char	buf[128];
	int	nchar = ut_format(prev, buf, sizeof(buf),
	    UT_ASCII | UT_DEFINITION | UT_NAMES);

	ut_handle_error_message(
	    "Duplicate definition for \"%s\" at \"%s\":%d", id, _path,
	    XML_GetCurrentLineNumber(parser));

	if (nchar < 0)
	    nchar = ut_format(prev, buf, sizeof(buf), UT_ASCII | UT_DEFINITION);

	if (nchar >= 0 && nchar < sizeof(buf)) {
	    buf[nchar] = 0;

	    ut_handle_error_message("Previous definition was \"%s\"", buf);
	}

        XML_StopParser(parser, 0);
    }
    else {
	/*
	 * Take prefixes into account for a prior definition by using ut_parse().
	 */
	prev = ut_parse(unitSystem, id, encoding);

	if ((isName ? ut_map_name_to_unit(id, unit) : ut_map_symbol_to_unit(id, unit))
		!= UT_SUCCESS) {
	    ut_handle_error_message("Couldn't map %s \"%s\" to unit",
		isName ? "name" : "symbol", id);
	    XML_StopParser(parser, 0);
	}
	else {
	    if (prev != NULL) {
		char	buf[128];
		int	nchar = ut_format(prev, buf, sizeof(buf),
		    UT_ASCII | UT_DEFINITION | UT_NAMES);

		if (nchar < 0)
		    nchar = ut_format(prev, buf, sizeof(buf),
			UT_ASCII | UT_DEFINITION);

		if (nchar < 0 || nchar >= sizeof(buf)) {
		    ut_handle_error_message("Definition of \"%s\" at \"%s\":%d "
			"overrides prefixed-unit", id, _path,
			XML_GetCurrentLineNumber(parser));
		}
		else {
		    buf[nchar] = 0;

		    ut_handle_error_message("Definition of \"%s\" at \"%s\":%d "
			"overrides prefixed-unit (%s)", id, _path,
                        XML_GetCurrentLineNumber(parser), buf);
		}
	    }

	    success = 1;
	}
    }

    return success;
}


static int
mapNameAndUnit(
    const char*	name,
    ut_encoding	encoding,
    ut_unit*	unit,
    const int	unitToName)
{
    int		success = 0;		/* failure */

    if (xmlMapIdToUnit(name, unit, 1)) {
	if (!unitToName) {
	    success = 1;
	}
	else {
	    if (ut_map_unit_to_name(unit, name, encoding) == UT_SUCCESS) {
		success = 1;
	    }
	    else {
		ut_handle_error_message("Couldn't map unit to name \"%s\"", name);
		XML_StopParser(parser, 0);
	    }
	}
    }

    return success;
}


static void
startName(
    void*		data,
    const char**	atts)
{
    if (setEncoding(atts)) {
	if (*currElt == PREFIX) {
	    if (!haveValue) {
		ut_handle_error_message("No previous <value> element");
		XML_StopParser(parser, 0);
	    }
	    else {
		clearText();
		XML_SetCharacterDataHandler(parser, accumulateText);
	    }
	}
	else if (*currElt == UNIT || *currElt == ALIASES) {
	    if (unit == NULL) {
		ut_handle_error_message(
		    "No previous <base>, <dimensionless>, or <def> element");
		XML_StopParser(parser, 0);
	    }
	    else {
		pluralNeeded  = 1;
		singularSeen = 0;
	    }
	}
	else {
	    ut_handle_error_message("Wrong place for <name> element");
	    XML_StopParser(parser, 0);
	}
    }
}


static const char*
formPlural(
    const char*	singular)
{
    static char	buf[NAME_SIZE];
    const char*	plural = NULL;		/* failure */

    if (singular != NULL) {
        int     length = strlen(singular);

	if (length + 3 >= sizeof(buf)) {
	    ut_handle_error_message("Singular form is too long");
	    XML_StopParser(parser, 0);
	}
	else if (length > 0) {
	    (void)strcpy(buf, singular);

	    if (length == 1) {
		(void)strcpy(buf+length, "s");
	    }
	    else {
		char    lastChar = singular[length-1];

		if (lastChar == 'y') {
		    char	penultimateChar = singular[length-2];

		    if (penultimateChar == 'a' || penultimateChar == 'e' ||
			    penultimateChar == 'i' || penultimateChar == 'o' ||
			    penultimateChar == 'u') {
			(void)strcpy(buf+length, "s");
		    }
		    else {
			(void)strcpy(buf+length-1, "ies");
		    }
		}
		else {
		    if (lastChar == 's' || lastChar == 'x' || lastChar == 'z' ||
			    (length >= 2 && (
				strcmp(singular+length-2, "ch") == 0 ||
				strcmp(singular+length-2, "sh") == 0))) {
			(void)strcpy(buf+length, "es");
		    }
		    else {
			(void)strcpy(buf+length, "s");
		    }
		}
	    }

	    plural = buf;
        }
    }

    return plural;
}


static const char*
embeddedNbspForm(
    const char*		name,
    const ut_encoding	encoding)
{
    const char*	newForm = NULL;		/* failure */

    if (strchr(name, '_') != NULL) {
	static char	buf[NAME_SIZE];
	const char*	replChars = NULL;
	int		replCharCount;

	if (encoding == UT_LATIN1) {
	    replChars = "\xa0";
	    replCharCount = 1;
	}
	else if (encoding == UT_UTF8) {
	    replChars = "\xc2\xa0";
	    replCharCount = 2;
	}

	if (replChars != NULL) {
	    int		n = 0;
	    const char*	cp;

	    for (cp = name; *cp; ++cp) {
		if (*cp != '_') {
		    if (n >= NAME_SIZE - 1)
			break;

		    buf[n++] = *cp;
		}
		else {
		    if (n >= NAME_SIZE - replCharCount)
			break;

		    (void)strncpy(buf+n, replChars, replCharCount);

		    n += replCharCount;
		}
	    }

	    if (*cp) {
		ut_handle_error_message("Name \"%s\" is too long", name);
		XML_StopParser(parser, 0);
	    }
	    else {
		buf[n] = 0;
		newForm = buf;
	    }
	}
    }

    return newForm;
}


static void
endName(
    void*		data)
{
    if (*currElt == PREFIX) {
	if (!haveValue) {
	    ut_handle_error_message("No previous <value> element");
	    XML_StopParser(parser, 0);
	}
	else {
	    if (ut_add_name_prefix(unitSystem, text, value) != UT_SUCCESS) {
		ut_handle_error_message(
		    "Couldn't map name-prefix \"%s\" to value %g", text, value);
		XML_StopParser(parser, 0);
	    }
	    else {
		prefixAdded = 1;
	    }
	}
    }
    else if (*currElt == UNIT || *currElt == ALIASES) {
	if (!singularSeen) {
	    ut_handle_error_message("No <singular> element");
	    XML_StopParser(parser, 0);
	}
	else if (pluralNeeded) {
	    if (singular[0] != 0) {
		const char*	plural = formPlural(singular);

		if (plural == NULL) {
		    ut_handle_error_message(
			"Couldn't form plural of \"%s\"", singular);
		    XML_StopParser(parser, 0);
		}
		/*
		 * NB: The unit has already been mapped to the singular name;
		 * consequently, it is not mapped to the plural name.
		 */
		else if (mapNameAndUnit(plural, encoding, unit, 0)) {
		    const char*	newForm = embeddedNbspForm(plural, encoding);

		    if (newForm != NULL)
			mapNameAndUnit(newForm, encoding, unit, 0);
		}
	    }				/* non-empty "singular" */
	}				/* plural form not seen */
    }					/* defining name for unit or alias */

    clearText();
}


static void
startSingular(
    void*		data,
    const char**	atts)
{
    if (*currElt != NAME) {
	ut_handle_error_message("Wrong place for <singular> element");
	XML_StopParser(parser, 0);
    }
    else if (singularSeen) {
	ut_handle_error_message("<singular> element already seen");
	XML_StopParser(parser, 0);
    }
    else {
	clearText();
	XML_SetCharacterDataHandler(parser, accumulateText);
    }
}


static void
endSingular(
    void*		data)
{
    if (nbytes >= NAME_SIZE) {
	ut_handle_error_message("Name \"%s\" is too long", text);
	XML_StopParser(parser, 0);
    }
    else {
	if (mapNameAndUnit(text, encoding, unit, currElt[-1] == UNIT)) {
	    const char*	newForm = embeddedNbspForm(text, encoding);

	    if (newForm != NULL)
		mapNameAndUnit(newForm, encoding, unit, currElt[-1] == UNIT);
	}

	(void)strcpy(singular, text);
	singularSeen = 1;
    }

    clearText();
}


static void
startPlural(
    void*		data,
    const char**	atts)
{
    if (*currElt != NAME) {
	ut_handle_error_message("Wrong place for <plural> element");
	XML_StopParser(parser, 0);
    }
    else if (!pluralNeeded ) {
	ut_handle_error_message("<plural> or <noplural> element already seen");
	XML_StopParser(parser, 0);
    }
    else {
	clearText();
	XML_SetCharacterDataHandler(parser, accumulateText);
    }
}


static void
endPlural(
    void*		data)
{
    if (nbytes == 0) {
	ut_handle_error_message("Empty <plural> element");
	XML_StopParser(parser, 0);
    }
    else if (nbytes >= NAME_SIZE) {
	ut_handle_error_message("Name \"%s\" is too long", text);
	XML_StopParser(parser, 0);
    }
    else {
	/*
	 * NB: The unit has already been mapped to the singular name;
	 * consequently, it is not mapped to the plural name.
	 */
	if (mapNameAndUnit(text, encoding, unit, 0)) {
	    const char*	newForm = embeddedNbspForm(text, encoding);

	    if (newForm != NULL)
		mapNameAndUnit(newForm, encoding, unit, 0);
	}

	pluralNeeded  = 0;
    }

    clearText();
}


static void
startNoPlural(
    void*		data,
    const char**	atts)
{
    if (*currElt != NAME) {
	ut_handle_error_message("Wrong place for <noplural> element");
	XML_StopParser(parser, 0);
    }
    else if (!pluralNeeded) {
	ut_handle_error_message("<plural> or <noplural> element already seen");
	XML_StopParser(parser, 0);
    }
}


static void
endNoPlural(
    void*		data)
{
    pluralNeeded  = 0;
}


static void
startSymbol(
    void*		data,
    const char**	atts)
{
    if (setEncoding(atts)) {
	if (*currElt == PREFIX) {
	    if (!haveValue) {
		ut_handle_error_message("No previous <value> element");
		XML_StopParser(parser, 0);
	    }
	    else {
		clearText();
		XML_SetCharacterDataHandler(parser, accumulateText);
	    }
	}
	else if (*currElt == UNIT || *currElt == ALIASES) {
	    if (unit == NULL) {
		ut_handle_error_message(
		    "No previous <base>, <dimensionless>, or <def> element");
		XML_StopParser(parser, 0);
	    }
	    else {
		clearText();
		XML_SetCharacterDataHandler(parser, accumulateText);
	    }
	}
	else {
	    ut_handle_error_message("Wrong place for <symbol> element");
	    XML_StopParser(parser, 0);
	}
    }
}


static void
endSymbol(
    void*		data)
{
    if (*currElt == PREFIX) {
	if (ut_add_symbol_prefix(unitSystem, text, value) != UT_SUCCESS) {
	    ut_handle_error_message(
		"Couldn't map symbol-prefix \"%s\" to value %g", text, value);
	    XML_StopParser(parser, 0);
	}
	else {
	    prefixAdded = 1;
	}
    }
    else if (*currElt == UNIT || *currElt == ALIASES) {
	if (xmlMapIdToUnit(text, unit, 0)) {
	    if (*currElt == UNIT &&
		    ut_map_unit_to_symbol(unit, text, encoding) != UT_SUCCESS) {
		ut_handle_error_message(
		    "Couldn't map unit to symbol \"%s\"", text);
		XML_StopParser(parser, 0);
	    }
	}
    }

    clearText();
}


static void
startValue(
    void*		data,
    const char**	atts)
{
    if (*currElt != PREFIX) {
	ut_handle_error_message("Wrong place for <value> element");
	XML_StopParser(parser, 0);
    }
    else if (haveValue) {
	ut_handle_error_message("<value> element already seen");
	XML_StopParser(parser, 0);
    }
    else {
	clearText();
	XML_SetCharacterDataHandler(parser, accumulateText);
    }
}


static void
endValue(
    void*		data)
{
    char*	endPtr;

    errno = 0;
    value = strtod(text, &endPtr);

    if (errno != 0) {
	ut_handle_error_message(strerror(errno));
	ut_handle_error_message("Couldn't decode numeric prefix value \"%s\"", text);
	XML_StopParser(parser, 0);
    }
    else if (*endPtr != 0) {
	ut_handle_error_message("Invalid numeric prefix value \"%s\"", text);
	XML_StopParser(parser, 0);
    }
    else {
	haveValue = 1;
    }
}


static void
startAliases(
    void*		data,
    const char**	atts)
{
    if (*currElt != UNIT) {
	ut_handle_error_message("Wrong place for <aliases> element");
	XML_StopParser(parser, 0);
    }
    else {
	clearText();
    }
}


static void
endAliases(
    void*		data)
{}


static void
startElement(
    void*		data,
    const XML_Char*	name,
    const XML_Char**	atts)
{
    if (skipDepth) {
	skipDepth++;
    }
    else {
	clearText();
	XML_SetCharacterDataHandler(parser, NULL);

	if (strcasecmp(name, "unit-system") == 0) {
	    startUnitSystem(data, atts);
	    *++currElt = UNIT_SYSTEM;
	}
	else if (strcasecmp(name, "prefix") == 0) {
	    startPrefix(data, atts);
	    *++currElt = PREFIX;
	}
	else if (strcasecmp(name, "unit") == 0) {
	    startUnit(data, atts);
	    *++currElt = UNIT;
	}
	else if (strcasecmp(name, "base") == 0) {
	    startBase(data, atts);
	    *++currElt = DIMENSIONLESS;
	}
	else if (strcasecmp(name, "dimensionless") == 0) {
	    startDimensionless(data, atts);
	    *++currElt = DIMENSIONLESS;
	}
	else if (strcasecmp(name, "def") == 0) {
	    startDef(data, atts);
	    *++currElt = DEF;
	}
	else if (strcasecmp(name, "value") == 0) {
	    startValue(data, atts);
	    *++currElt = VALUE;
	}
	else if (strcasecmp(name, "name") == 0) {
	    startName(data, atts);
	    *++currElt = NAME;
	}
	else if (strcasecmp(name, "singular") == 0) {
	    startSingular(data, atts);
	    *++currElt = SINGULAR;
	}
	else if (strcasecmp(name, "plural") == 0) {
	    startPlural(data, atts);
	    *++currElt = PLURAL;
	}
	else if (strcasecmp(name, "symbol") == 0) {
	    startSymbol(data, atts);
	    *++currElt = SYMBOL;
	}
	else if (strcasecmp(name, "aliases") == 0) {
	    startAliases(data, atts);
	    *++currElt = ALIASES;
	}
	else {
	    skipDepth = 1;
	}
    }
}


static void
endElement(
    void*		data,
    const XML_Char*	name)
{
    if (skipDepth != 0) {
	--skipDepth;
    }
    else if (strcasecmp(name, "unit-system") == 0) {
	endUnitSystem(data);
	currElt = elementStack;
    }
    else {
	--currElt;

	if (strcasecmp(name, "prefix") == 0) {
	    endPrefix(data);
	}
	else if (strcasecmp(name, "unit") == 0) {
	    endUnit(data);
	}
	else if (strcasecmp(name, "base") == 0) {
	    endBase(data);
	}
	else if (strcasecmp(name, "dimensionless") == 0) {
	    endDimensionless(data);
	}
	else if (strcasecmp(name, "def") == 0) {
	    endDef(data);
	}
	else if (strcasecmp(name, "value") == 0) {
	    endValue(data);
	}
	else if (strcasecmp(name, "name") == 0) {
	    endName(data);
	}
	else if (strcasecmp(name, "singular") == 0) {
	    endSingular(data);
	}
	else if (strcasecmp(name, "plural") == 0) {
	    endPlural(data);
	}
	else if (strcasecmp(name, "symbol") == 0) {
	    endSymbol(data);
	}
	else if (strcasecmp(name, "aliases") == 0) {
	    endAliases(data);
	}
	else {
	    ut_handle_error_message("Unknown element \"<%s>\"", name);
	    XML_StopParser(parser, 0);
	}

	XML_SetCharacterDataHandler(parser, NULL);
    }
}


static void
declareXml(
    void*	data,
    const char*	version,
    const char*	encoding,
    int		standalone)
{
    if (strcasecmp(encoding, "US-ASCII") == 0) {
	xmlEncoding = UT_ASCII;
    }
    else if (strcasecmp(encoding, "ISO-8859-1") == 0) {
	xmlEncoding = UT_LATIN1;
    }
    else if (strcasecmp(encoding, "UTF-8") == 0) {
	xmlEncoding = UT_UTF8;
    }
    else {
	ut_handle_error_message("Unknown XML encoding \"%s\"", encoding);
	XML_StopParser(parser, 0);
    }
}


/*
 * Returns the unit-system corresponding to an XML file.  This is the usual way
 * that a client will obtain a unit-system.
 *
 * Arguments:
 *	path	The pathname of the XML file or NULL.  If NULL, then the
 *		pathname specified by the environment variable UDUNITS2_XML_PATH
 *		is used if set; otherwise, the compile-time pathname of the
 *		installed, default, unit database is used.
 * Returns:
 *	NULL	Failure.  "ut_get_status()" will be
 *		    UT_OPEN_ARG		"path" is non-NULL but file couldn't be
 *					opened.  See "errno" for reason.
 *		    UT_OPEN_ENV		"path" is NULL and environment variable
 *					UDUNITS2_XML_PATH is set but file
 *					couldn't be opened.  See "errno" for
 *					reason.
 *		    UT_OPEN_DEFAULT	"path" is NULL, environment variable
 *					UDUNITS2_XML_PATH is unset, and the
 *					installed, default, unit database
 *					couldn't be opened.  See "errno" for
 *					reason.
 *		    UT_PARSE		Couldn't parse unit database.
 *		    UT_OS		Operating-system error.  See "errno".
 *	else	Pointer to the unit-system defined by "path".
 */
ut_system*
ut_read_xml(
    const char*	path)
{
    ut_system*	sys = NULL;		/* failure */
    int		fd;

    ut_set_status(UT_SUCCESS);

    if (path != NULL) {
	fd = open(path, O_RDONLY);

	if (fd == -1) {
	    ut_handle_error_message(strerror(errno));
	    ut_handle_error_message(
		"ut_read_xml(): Couldn't open argument-specified database \"%s\"",
		path);
	    ut_set_status(UT_OPEN_ARG);
	}
    }
    else {
	path = getenv("UDUNITS2_XML_PATH");

	if (path != NULL) {
	    fd = open(path, O_RDONLY);

	    if (fd == -1) {
		ut_handle_error_message(strerror(errno));
		ut_handle_error_message(
		    "ut_read_xml(): Couldn't open UDUNITS2_XML_PATH-specified "
			"database \"%s\"", path);
		ut_set_status(UT_OPEN_ENV);
	    }
	}
	else {
	    path = DEFAULT_UDUNITS2_XML_PATH;
	    fd = open(path, O_RDONLY);

	    if (fd == -1) {
		ut_handle_error_message(strerror(errno));
		ut_handle_error_message(
		    "ut_read_xml(): Couldn't open installed, default database "
			"\"%s\"", path);
		ut_set_status(UT_OPEN_DEFAULT);
	    }
	}
    }

    if (fd != -1) {
	_path = path;
	parser = XML_ParserCreate(NULL);

	if (parser == NULL) {
	    ut_handle_error_message(strerror(errno));
	    ut_handle_error_message("Couldn't create XML parser");
	    ut_set_status(UT_OS);
	}
	else {
	    int	error = 1;
	    int	nbytes;

	    _path = path;
            currElt = elementStack;
            prefixAdded = 0;
            unitSystem = NULL;
            skipDepth = 0;
            encoding = UT_ASCII;
            isBase = 0;
            isDimensionless = 0;
            haveValue = 0;
            pluralNeeded = 1;
            singularSeen = 0;

	    XML_SetXmlDeclHandler(parser, declareXml);
	    XML_SetElementHandler(parser, startElement, endElement);

	    do {
		char	buf[BUFSIZ];	/* from <stdio.h> */

		nbytes = read(fd, buf, sizeof(buf));

		if (nbytes < 0) {
		    ut_handle_error_message(strerror(errno));
		    ut_set_status(UT_OS);

		    break;
		}
		else {
		    if (XML_Parse(parser, buf, nbytes, nbytes == 0)
			    != XML_STATUS_OK) {
			ut_handle_error_message(
			    XML_ErrorString(XML_GetErrorCode(parser)));
			ut_set_status(UT_PARSE);

			break;
		    }
		}
	    } while (nbytes > 0);

	    if (nbytes != 0) {
		/*
		 * Parsing of the XML file terminated prematurely.
		 */
		ut_handle_error_message("Stopped at \"%s\":%d, column %d",
		    path, XML_GetCurrentLineNumber(parser),
		    XML_GetCurrentColumnNumber(parser));
		XML_ParserFree(parser);
	    }
	    else {
		ut_unit*	second = ut_get_unit_by_name(unitSystem, "second");

		if (second != NULL) {
		    if (ut_set_second(second) == UT_SUCCESS) {
			error = 0;
		    }
		    else {
			ut_handle_error_message("Couldn't set \"second\" unit "
			    "in unit-system");
		    }

		    ut_free(second);
		}
	    }

	    if (error) {
                ut_status        status = ut_get_status();

		ut_free_system(unitSystem);
		unitSystem = NULL;

                ut_set_status(status);
	    }
	}				/* "parser" allocated */

	sys = unitSystem;
	unitSystem = NULL;

	(void)close(fd);
    }					/* "fd" open */

    return sys;
}
