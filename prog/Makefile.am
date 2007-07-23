## Process this file with automake to produce Makefile.in
bin_PROGRAMS	= udunits2
LDADD		= ../lib/libudunits2.la \
		  -lm
info_TEXINFOS = udunits2prog.texi
#html_DATA = udunits2prog.html
#pdf_DATA = udunits2prog.pdf
AM_MAKEINFOHTMLFLAGS = --no-split
AM_CPPFLAGS	= -I$(top_srcdir)/lib
MOSTLYCLEANFILES	= lint.log *.ln
LINTFLAGS	= -u
LINT		= lint
TAGS_FILES      = ../lib/*.c ../lib/*.h
EXTRA_DIST = udunits2prog.html udunits2prog.pdf

.c.ln:
	$(LINT.c) $(AM_CPPFLAGS) -c $<

.c.i:
	$(CPP) $(CPPFLAGS) $< >$@

debug:		udunits2
	libtool --mode=execute gdb udunits2
