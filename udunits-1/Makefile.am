# Copyright 2008, 2009 University Corporation for Atmospheric Research
#
# This file is part of the UDUNITS-2 package.  See the file LICENSE
# in the top-level source-directory of the package for copying and
# redistribution conditions.
#
## Process this file with automake to produce Makefile.in
noinst_LTLIBRARIES = libudunits1.la
libudunits1_la_SOURCES = udunits.c
if ENABLE_UDUNITS_1
include_HEADERS = udunits.h
else
noinst_HEADERS = udunits.h
endif
AM_CPPFLAGS    = -I$(top_srcdir)/lib
LDADD = $(top_builddir)/lib/libudunits2.la @LIBS_CUNIT@ -lm

if HAVE_CUNIT
check_PROGRAMS = testUnits
testUnits_CPPFLAGS = $(AM_CPPFLAGS) @CPPFLAGS_CUNIT@
TESTS_ENVIRONMENT = UDUNITS2_XML_PATH='$(top_srcdir)/lib/udunits2.xml'
TESTS = testUnits
endif

DISTCLEANFILES = *.log
MOSTLYCLEANFILES = lint.log *.ln *.i core core.[0-9]*
TAGS_FILES = ../lib/*.c ../lib/*.h

LINTFLAGS	= -u
LINT		= lint

.c.ln:
	$(LINT.c) $(AM_CPPFLAGS) $(CPPFLAGS) -c $<

.c.i:
	$(CPP) $(AM_CPPFLAGS) $(CPPFLAGS) $< >$@

if HAVE_CUNIT
debug:		testUnits
	UDUNITS2_XML_PATH='$(top_srcdir)/lib/udunits2.xml' libtool \
	    --mode=execute gdb testUnits
endif

install-info-am:
install-html-am:
install-pdf-am:
