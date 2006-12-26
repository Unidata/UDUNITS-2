/*
 * This module is thread-compatible but not thread-safe.  Multi-threaded
 * access must be externally synchronized.
 *
 * $Id: xml.c,v 1.3 2006/12/26 22:42:01 steve Exp $
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

static const char*	_path;
static int		prefixAdded;
static utEncoding	xmlEncoding;
static XML_Parser	parser = NULL;
static utSystem*	unitSystem = NULL;
static unsigned		skipDepth = 0;
static char*		text = NULL;
static size_t		nbytes = 0;
static double		value = 0;
static utEncoding	encoding = UT_ASCII;
static utUnit*		unit = NULL;
static int		isBase = 0;
static int		isDimensionless = 0;
static int		haveValue = 0;
static int		pluralAdded = 0;
static char		singular[NAME_SIZE];
static int		singularSeen;

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
	utHandleErrorMessage(strerror(errno));
	utHandleErrorMessage("Couldn't reallocate %lu-byte text buffer",
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
	utHandleErrorMessage("Wrong place for <unit-system> element");
	utSetStatus(UT_XML);
	XML_StopParser(parser, 0);
    }
    else {
	utFreeSystem(unitSystem);

	unitSystem = utNewSystem();

	if (system == NULL) {
	    utHandleErrorMessage("Couldn't create new unit-system");
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
	utHandleErrorMessage("Wrong place for <prefix> element");
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
	utHandleErrorMessage("Prefix incompletely specified");
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
	utHandleErrorMessage("Wrong place for <unit> element");
	XML_StopParser(parser, 0);
    }
    else {
	utFree(unit);
	unit = NULL;
	isBase = 0;
	isDimensionless = 0;
    }
}


static void
endUnit(
    void*		data)
{
    utFree(unit);
    unit = NULL;
}


static void
startBase(
    void*		data,
    const char**	atts)
{
    if (*currElt != UNIT) {
	utHandleErrorMessage("Wrong place for <base> element");
	XML_StopParser(parser, 0);
    }
    else {
	if (isDimensionless) {
	    utHandleErrorMessage(
		"<dimensionless> and <base> are mutually exclusive");
	    XML_StopParser(parser, 0);
	}
	if (unit != NULL) {
	    utHandleErrorMessage(
		"<base> and <def> are mutually exclusive");
	    XML_StopParser(parser, 0);
	}
	else if (isBase) {
	    utHandleErrorMessage("<base> element already seen");
	    XML_StopParser(parser, 0);
	}
    }
}


static void
endBase(
    void*		data)
{
    unit = utNewBaseUnit(unitSystem);

    if (unit == NULL) {
	utHandleErrorMessage("Couldn't create new base unit");
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
	utHandleErrorMessage("Wrong place for <dimensionless> element");
	XML_StopParser(parser, 0);
    }
    else {
	if (isBase) {
	    utHandleErrorMessage(
		"<dimensionless> and <base> are mutually exclusive");
	    XML_StopParser(parser, 0);
	}
	else if (unit != NULL) {
	    utHandleErrorMessage(
		"<dimensionless> and <def> are mutually exclusive");
	    XML_StopParser(parser, 0);
	}
	else if (isDimensionless) {
	    utHandleErrorMessage("<dimensionless> element already seen");
	    XML_StopParser(parser, 0);
	}
    }
}


static void
endDimensionless(
    void*		data)
{
    unit = utNewDimensionlessUnit(unitSystem);

    if (unit == NULL) {
	utHandleErrorMessage("Couldn't create new dimensionless unit");
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
		utHandleErrorMessage("Unknown character encoding \"%s\"",
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
	utHandleErrorMessage("Wrong place for <def> element");
	XML_StopParser(parser, 0);
    }
    else if (isBase) {
	utHandleErrorMessage(
	    "<base> and <def> are mutually exclusive");
	XML_StopParser(parser, 0);
    }
    else if (isDimensionless) {
	utHandleErrorMessage(
	    "<dimensionless> and <def> are mutually exclusive");
	XML_StopParser(parser, 0);
    }
    else if (unit != NULL) {
	utHandleErrorMessage("<def> element already seen");
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
	utHandleErrorMessage("Empty unit definition");
	XML_StopParser(parser, 0);
    }
    else {
	unit = utParse(unitSystem, text, xmlEncoding);

	if (unit == NULL) {
	    utHandleErrorMessage("Couldn't parse unit specification \"%s\"",
		text);
	    XML_StopParser(parser, 0);
	}
    }
}


static int
xmlMapIdToUnit(
    const char*	id,
    utUnit*	unit,
    int		isName)
{
    int		success = 0;		/* failure */
    utUnit*	prev = utGetUnitByName(unitSystem, id);

    if (prev == NULL)
	prev = utGetUnitBySymbol(unitSystem, id);

    if (prev != NULL) {
	char	buf[128];
	int	nchar = utFormat(prev, buf, sizeof(buf),
	    UT_ASCII | UT_DEFINITION | UT_NAMES);

	if (nchar < 0)
	    nchar = utFormat(prev, buf, sizeof(buf), UT_ASCII | UT_DEFINITION);

	utHandleErrorMessage(
	    "Duplicate definition for \"%s\" at \"%s\":%d", id, _path,
	    XML_GetCurrentLineNumber(parser));

	if (nchar < 0 || nchar >= sizeof(buf)) {
	    utHandleErrorMessage("Previous definition remains in effect");
	}
	else {
	    buf[nchar] = 0;

	    utHandleErrorMessage("Previous definition (%s) remains in effect",
		buf);
	}
    }
    else {
	/*
	 * Take prefixes into account for a prior definition by using utParse().
	 */
	prev = utParse(unitSystem, id, encoding);

	if ((isName ? utMapNameToUnit(id, unit) : utMapSymbolToUnit(id, unit))
		!= UT_SUCCESS) {
	    utHandleErrorMessage("Couldn't map %s \"%s\" to unit",
		isName ? "name" : "symbol", id);
	    XML_StopParser(parser, 0);
	}
	else {
	    if (prev != NULL) {
		char	buf[128];
		int	nchar = utFormat(prev, buf, sizeof(buf),
		    UT_ASCII | UT_DEFINITION | UT_NAMES);

		if (nchar < 0)
		    nchar = utFormat(prev, buf, sizeof(buf),
			UT_ASCII | UT_DEFINITION);

		if (nchar < 0 || nchar >= sizeof(buf)) {
		    utHandleErrorMessage("Definition of \"%s\" at \"%s\":%d "
			"hides earlier one", id, _path,
			XML_GetCurrentLineNumber(parser));
		}
		else {
		    buf[nchar] = 0;

		    utHandleErrorMessage("Definition of \"%s\" at \"%s\":%d "
			"hides earlier one (%s)", id, _path,
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
    utEncoding	encoding,
    utUnit*	unit,
    const int	unitToName)
{
    int		success = 0;		/* failure */

    if (xmlMapIdToUnit(name, unit, 1)) {
	if (!unitToName) {
	    success = 1;
	}
	else {
	    if (utMapUnitToName(unit, name, encoding) == UT_SUCCESS) {
		success = 1;
	    }
	    else {
		utHandleErrorMessage("Couldn't map unit to name \"%s\"", name);
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
		utHandleErrorMessage("No previous <value> element");
		XML_StopParser(parser, 0);
	    }
	    else {
		clearText();
		XML_SetCharacterDataHandler(parser, accumulateText);
	    }
	}
	else if (*currElt == UNIT || *currElt == ALIASES) {
	    if (unit == NULL) {
		utHandleErrorMessage(
		    "No previous <base>, <dimensionless>, or <def> element");
		XML_StopParser(parser, 0);
	    }
	    else {
		pluralAdded = 0;
		singularSeen = 0;
	    }
	}
	else {
	    utHandleErrorMessage("Wrong place for <name> element");
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
	    utHandleErrorMessage("Singular form is too long");
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
    const utEncoding	encoding)
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
		utHandleErrorMessage("Name \"%s\" is too long", name);
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
	    utHandleErrorMessage("No previous <value> element");
	    XML_StopParser(parser, 0);
	}
	else {
	    if (utAddNamePrefix(unitSystem, text, value) != UT_SUCCESS) {
		utHandleErrorMessage(
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
	    utHandleErrorMessage("No <singular> element");
	    XML_StopParser(parser, 0);
	}
	else if (!pluralAdded) {
	    if (singular[0] != 0) {
		const char*	plural = formPlural(singular);

		if (plural == NULL) {
		    utHandleErrorMessage(
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
	utHandleErrorMessage("Wrong place for <singular> element");
	XML_StopParser(parser, 0);
    }
    else if (singularSeen) {
	utHandleErrorMessage("<singular> element already seen");
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
	utHandleErrorMessage("Name \"%s\" is too long", text);
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
	utHandleErrorMessage("Wrong place for <plural> element");
	XML_StopParser(parser, 0);
    }
    else if (pluralAdded) {
	utHandleErrorMessage("<plural> element already seen");
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
	utHandleErrorMessage("Empty <plural> element");
	XML_StopParser(parser, 0);
    }
    else if (nbytes >= NAME_SIZE) {
	utHandleErrorMessage("Name \"%s\" is too long", text);
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

	pluralAdded = 1;
    }

    clearText();
}


static void
startSymbol(
    void*		data,
    const char**	atts)
{
    if (setEncoding(atts)) {
	if (*currElt == PREFIX) {
	    if (!haveValue) {
		utHandleErrorMessage("No previous <value> element");
		XML_StopParser(parser, 0);
	    }
	    else {
		clearText();
		XML_SetCharacterDataHandler(parser, accumulateText);
	    }
	}
	else if (*currElt == UNIT || *currElt == ALIASES) {
	    if (unit == NULL) {
		utHandleErrorMessage(
		    "No previous <base>, <dimensionless>, or <def> element");
		XML_StopParser(parser, 0);
	    }
	    else {
		clearText();
		XML_SetCharacterDataHandler(parser, accumulateText);
	    }
	}
	else {
	    utHandleErrorMessage("Wrong place for <symbol> element");
	    XML_StopParser(parser, 0);
	}
    }
}


static void
endSymbol(
    void*		data)
{
    if (*currElt == PREFIX) {
	if (utAddSymbolPrefix(unitSystem, text, value) != UT_SUCCESS) {
	    utHandleErrorMessage(
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
		    utMapUnitToSymbol(unit, text, encoding) != UT_SUCCESS) {
		utHandleErrorMessage(
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
	utHandleErrorMessage("Wrong place for <value> element");
	XML_StopParser(parser, 0);
    }
    else if (haveValue) {
	utHandleErrorMessage("<value> element already seen");
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
	utHandleErrorMessage(strerror(errno));
	utHandleErrorMessage("Couldn't decode numeric prefix value \"%s\"", text);
	XML_StopParser(parser, 0);
    }
    else if (*endPtr != 0) {
	utHandleErrorMessage("Invalid numeric prefix value \"%s\"", text);
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
	utHandleErrorMessage("Wrong place for <aliases> element");
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
	    utHandleErrorMessage("Unknown element \"<%s>\"", name);
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
	utHandleErrorMessage("Unknown XML encoding \"%s\"", encoding);
	XML_StopParser(parser, 0);
    }
}


/*
 * Returns the unit-system corresponding to an XML file.  This is the usual way
 * that a client will obtain a unit-system.
 *
 * Arguments:
 *	path	The pathname of the XML file.
 * Returns:
 *	NULL	Failure.  "utGetStatus()" will be
 *		    UT_NULL_ARG	"path" is NULL.
 *		    UT_OS	Operating-system error.  See "errno".
 *		    UT_XML	XML parse error.
 *	else	Pointer to the unit-system defined by "path".
 */
utSystem*
utReadXml(
    const char* const	path)
{
    utSystem*	sys = NULL;		/* failure */

    utSetStatus(UT_SUCCESS);

    if (path == NULL) {
	utHandleErrorMessage("NULL path argument");
	utSetStatus(UT_NULL_ARG);
    }
    else {
	parser = XML_ParserCreate(NULL);

	if (parser == NULL) {
	    utHandleErrorMessage(strerror(errno));
	    utHandleErrorMessage("Couldn't create XML parser");
	    utSetStatus(UT_OS);
	}
	else {
	    int	fd = open(path, O_RDONLY);

	    if (fd == -1) {
		utHandleErrorMessage(strerror(errno));
	    }
	    else {
		int	error = 1;
		int	nbytes;

		_path = path;

		XML_SetXmlDeclHandler(parser, declareXml);
		XML_SetElementHandler(parser, startElement, endElement);

		do {
		    char	buf[BUFSIZ];	/* from <stdio.h> */

		    nbytes = read(fd, buf, sizeof(buf));

		    if (nbytes < 0) {
			utHandleErrorMessage(strerror(errno));
			utSetStatus(UT_OS);

			break;
		    }
		    else {
			if (XML_Parse(parser, buf, nbytes, nbytes == 0)
				!= XML_STATUS_OK) {
			    utHandleErrorMessage(
				XML_ErrorString(XML_GetErrorCode(parser)));
			    utSetStatus(UT_XML);

			    break;
			}
		    }
		} while (nbytes > 0);

		if (nbytes != 0) {
		    /*
		     * Parsing of the XML file terminated prematurely.
		     */
		    utHandleErrorMessage("Stopped at \"%s\":%d, column %d",
			path, XML_GetCurrentLineNumber(parser),
			XML_GetCurrentColumnNumber(parser));
		    XML_ParserFree(parser);
		}
		else {
		    utUnit*	second = utGetUnitByName(unitSystem, "second");

		    if (second != NULL) {
			if (utSetSecond(second) == UT_SUCCESS) {
			    error = 0;
			}
			else {
			    utHandleErrorMessage("Couldn't set \"second\" unit "
				"in unit-system");
			}

			utFree(second);
		    }
		}

		if (error) {
		    utFreeSystem(unitSystem);
		    unitSystem = NULL;
		}

		(void)close(fd);
	    }				/* "fd" open */
	}				/* "parser" allocated */

	sys = unitSystem;
	unitSystem = NULL;
    }

    return sys;
}
