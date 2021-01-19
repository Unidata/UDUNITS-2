#  @file:  Makefile.am
# @author: Steven R. Emmerson
#
#    Copyright 2021 University Corporation for Atmospheric Research
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# Process this file with automake to produce Makefile.in

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
    udunits2prog.pdf \
    XGetopt.c XGetopt.h

.c.ln:
	$(LINT.c) $(AM_CPPFLAGS) $(CPPFLAGS) -c $<

.c.i:
	$(CPP) $(AM_CPPFLAGS) $(CPPFLAGS) $< >$@

debug:		udunits2
	UDUNITS2_XML_PATH=$(top_srcdir)/lib/udunits2.xml libtool \
	    --mode=execute gdb udunits2

check-local:	udunits2
	./udunits2 $(top_srcdir)/lib/udunits2.xml </dev/null
