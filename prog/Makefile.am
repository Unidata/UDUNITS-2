# Copyright 2013 University Corporation for Atmospheric Research
#
# This file is part of the UDUNITS-2 package.  See the file COPYRIGHT
# in the top-level source-directory of the package for copying and
# redistribution conditions.
#
## Process this file with automake to produce Makefile.in
bin_PROGRAMS		= udunits2
LDADD			= ../lib/libudunits2.la
TEXINFO_TEX		= ../texinfo.tex
info_TEXINFOS		= udunits2prog.texi
AM_MAKEINFOFLAGS	= -I $(top_srcdir)
AM_MAKEINFOHTMLFLAGS	= --no-split -I $(top_srcdir)
AM_CPPFLAGS		= -I$(top_srcdir)/lib
MOSTLYCLEANFILES	= lint.log *.ln
LINTFLAGS		= -u
LINT			= lint
TAGS_FILES     	 	= $(top_srcdir)/lib/*.c $(top_srcdir)/lib/*.h
EXTRA_DIST 		= \
    CMakeLists.txt \
    udunits2prog.html \
    udunits2prog.pdf

.c.ln:
	$(LINT.c) $(AM_CPPFLAGS) $(CPPFLAGS) -c $<

.c.i:
	$(CPP) $(AM_CPPFLAGS) $(CPPFLAGS) $< >$@

debug:		udunits2
	UDUNITS2_XML_PATH=$(top_srcdir)/lib/udunits2.xml libtool \
	    --mode=execute gdb udunits2

check-local:	udunits2
	./udunits2 $(top_srcdir)/lib/udunits2.xml </dev/null
