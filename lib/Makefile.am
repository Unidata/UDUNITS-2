## Process this file with automake to produce Makefile.in
#docdir = $(datadir)/../doc/udunits2
#htmldir = $(docdir)
#pdfdir = $(docdir)
SUBDIRS	= xmlTests
lib_LTLIBRARIES = libudunits2.la
libudunits2_la_SOURCES = unitcore.c converter.c formatter.c \
    idToUnitMap.c idToUnitMap.h \
    unitToIdMap.c unitToIdMap.h \
    unitAndId.c unitAndId.h \
    systemMap.c systemMap.h \
    prefix.c \
    parser.y \
    status.c \
    xml.c \
    error.c \
    utFreeSystem.c
BUILT_SOURCES = parser.c scanner.c
data_DATA = SI.xml
info_TEXINFOS = udunits2lib.texi
html_DATA = udunits2lib.html
pdf_DATA = udunits2lib.pdf
AM_MAKEINFOHTMLFLAGS = --no-split
libudunits2_la_CPPFLAGS = \
	-DUDUNITS2_XML_PATH='"@datadir@/SI.xml"' \
	-I$(top_srcdir)/expat

lex_prefix = ut_
LEX_OUTPUT_ROOT = lex.$(lex_prefix)
LFLAGS = -d -P$(lex_prefix)

LDADD = libudunits2.la @LIBS_CUNIT@ -lm @LIBS_COVERAGE@
AM_LDFLAGS = @CFLAGS_COVERAGE@ @LIBS_COVERAGE@
libudunits2_la_CFLAGS = @CFLAGS_COVERAGE@
libudunits2_la_LIBADD = ../expat/libexpat.la -lm
include_HEADERS = udunits2.h converter.h
EXTRA_DIST = scanner.l scanner.c SI.xml udunits2lib.html udunits2lib.pdf
AM_YFLAGS = -t -p ut_

if HAVE_CUNIT
check_PROGRAMS = testUnits
testUnits_CPPFLAGS = @CPPFLAGS_CUNIT@
TESTS = testUnits
endif

MOSTLYCLEANFILES = lint.log *.ln *.i core core.[0-9]* *.gcov *.gcda *.gcno
TAGS_FILES = parser.c ../expat/*.c ../expat/*.h

LINTFLAGS	= -u
LINT		= lint

SUFFIXES = .c .l

.c.ln:
	$(LINT.c) $(libudunits2_la_CPPFLAGS) $(CPPFLAGS) -c $<

.c.i:
	$(CPP) $(libudunits2_la_CPPFLAGS) $(CPPFLAGS) $< >$@

.c.gcov:
	gcov -o .libs/libudunits2_la-$<.o $<.c

if HAVE_CUNIT
debug:		testUnits
	libtool --mode=execute gdb testUnits
endif

coverage:	testUnits
	rm -f .libs/*.gcda
	./testUnits
	rm -f *.gcov
	gcov -o .libs/libudunits2_la-converter.o converter.c
	gcov -o .libs/libudunits2_la-unitcore.o unitcore.c
	gcov -o .libs/libudunits2_la-error.o error.c
	gcov -o .libs/libudunits2_la-formatter.o formatter.c
	gcov -o .libs/libudunits2_la-idToUnitMap.o idToUnitMap.c
	gcov -o .libs/libudunits2_la-parser.o parser.c
	gcov -o .libs/libudunits2_la-prefix.o prefix.c
	gcov -o .libs/libudunits2_la-systemMap.o systemMap.c
	gcov -o .libs/libudunits2_la-unitAndId.o unitAndId.c
	gcov -o .libs/libudunits2_la-unitToIdMap.o unitToIdMap.c
	gcov -o .libs/libudunits2_la-utFreeSystem.o utFreeSystem.c
	gcov -o .libs/libudunits2_la-xml.o xml.c

parser.o: scanner.c

