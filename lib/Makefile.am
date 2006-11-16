## Process this file with automake to produce Makefile.in
lib_LTLIBRARIES = libudunits2.la
libudunits2_la_SOURCES = core.c converter.c formatter.c \
    idToUnitMap.c idToUnitMap.h \
    unitToIdMap.c unitToIdMap.h \
    unitAndId.c unitAndId.h \
    systemMap.c systemMap.h \
    prefix.c
include_HEADERS = units.h converter.h
AM_CTAGSFLAGS = -t
check_PROGRAMS = testUnits
testUnits_CPPFLAGS = -I/opt/include
testUnits_LDADD = libudunits2.la
testUnits_LDFLAGS = -L/opt/lib -lcunit -lm
TESTS = testUnits
MOSTLYCLEANFILES = lint.log *.ln

LINTFLAGS	= -u
LINT		= lint

.c.ln:
	$(LINT.c) -c $<

.c.i:
	$(CPP) $(CPPFLAGS) $< >$@
