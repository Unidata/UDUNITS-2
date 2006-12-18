## Process this file with automake to produce Makefile.in
lib_LTLIBRARIES = libudunits2.la
libudunits2_la_SOURCES = core.c converter.c formatter.c \
    idToUnitMap.c idToUnitMap.h \
    unitToIdMap.c unitToIdMap.h \
    unitAndId.c unitAndId.h \
    systemMap.c systemMap.h \
    prefix.c \
    parser.y \
    xml.c \
    error.c \
    utFreeSystem.c
BUILT_SOURCES = parser.c scanner.c
lex_prefix = ut_
LEX_OUTPUT_ROOT = lex.$(lex_prefix)
LFLAGS = -d -P$(lex_prefix)

libudunits2_la_LIBADD = ../expat/libexpat.la
include_HEADERS = units.h converter.h
AM_CPPFLAGS = -I$(top_srcdir)/expat/lib
AM_CTAGSFLAGS = -t
EXTRA_DIST = SI.xml scanner.l scanner.c
AM_YFLAGS = -t -p ut_

if HAVE_CUNIT
check_PROGRAMS = testUnits
testUnits_CPPFLAGS = -I/opt/include
testUnits_LDFLAGS = @LD_CUNIT@ -lm
testUnits_LDADD = libudunits2.la
TESTS = testUnits
endif

MOSTLYCLEANFILES = lint.log *.ln *.i
TAGS_FILES = parser.c

LINTFLAGS	= -u
LINT		= lint

SUFFIXES = .c .l

.c.ln:
	$(LINT.c) $(AM_CPPFLAGS) -c $<

.c.i:
	$(CPP) $(CPPFLAGS) $< >$@

if HAVE_CUNIT
debug:		testUnits
	libtool --mode=execute gdb testUnits
endif

parser.o: scanner.c
