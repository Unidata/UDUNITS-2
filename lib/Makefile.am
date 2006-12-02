## Process this file with automake to produce Makefile.in
lib_LTLIBRARIES = libudunits2.la
libudunits2_la_SOURCES = core.c converter.c formatter.c \
    idToUnitMap.c idToUnitMap.h \
    unitToIdMap.c unitToIdMap.h \
    unitAndId.c unitAndId.h \
    systemMap.c systemMap.h \
    prefix.c \
    parser.y
include_HEADERS = units.h converter.h
AM_CPPFLAGS = -I$(srcdir)/expat
AM_CTAGSFLAGS = -t
BUILT_SOURCES = scanner.c
AM_YFLAGS = -p ut_
check_PROGRAMS = testUnits
testUnits_CPPFLAGS = -I/opt/include
testUnits_LDADD = libudunits2.la ../expat/libexpat.la
testUnits_LDFLAGS = -L/opt/lib -lcunit -lm
TESTS = testUnits
MOSTLYCLEANFILES = lint.log *.ln
CLEANFILES = scanner.c
TAGS_FILES = parser.c
LFLAGS = -Put_ @LFLAGS@

LINTFLAGS	= -u
LINT		= lint

SUFFIXES = .c .l

.c.ln:
	$(LINT.c) -c $<

.c.i:
	$(CPP) $(CPPFLAGS) $< >$@

debug:
	libtool --mode=execute gdb testUnits
