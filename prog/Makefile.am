## Process this file with automake to produce Makefile.in
bin_PROGRAMS	= udunits2
LDADD		= ../lib/libudunits2.la \
		  ../expat/libexpat.la \
		  -lm
info_TEXINFOS = udunits2prog.texi
AM_MAKEINFOHTMLFLAGS = --no-split
AM_CPPFLAGS	= -I$(top_srcdir)/lib
MOSTLYCLEANFILES	= lint.log *.ln
LINTFLAGS	= -u
LINT		= lint
TAGS_FILES      = ../lib/*.c ../lib/*.h

.c.ln:
	$(LINT.c) $(AM_CPPFLAGS) -c $<

.c.i:
	$(CPP) $(CPPFLAGS) $< >$@

debug:		udunits2
	libtool --mode=execute gdb udunits2
