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

ACLOCAL_AMFLAGS		= -I m4
SUBDIRS			= lib prog
DIST_SUBDIRS		= lib prog
info_TEXINFOS		= udunits2.texi
TEXINFO_TEX	        = ./texinfo.tex
EXTRA_DIST = \
    CHANGE_LOG \
    CMakeLists.txt \
    config-cmake.h \
    LICENSE \
    udunits2.html \
    udunits2.pdf \
    version.texi
AM_MAKEINFOHTMLFLAGS 	= --no-split
RSYNC_FLAGS		= --rsh=ssh --rsync-path=/usr/bin/rsync
distName 		= $(PACKAGE)-$(VERSION)
distArchive 		= $(distName).tar.gz
CLEANFILES		= *~ *.info
MAINTAINERCLEANFILES	= *.gz build *~
DISTCLEANFILES		= *.log *.gz build *~
dist_doc_DATA		= CHANGE_LOG LICENSE README

ftp:	dist
	scp $(distArchive) ftp.unidata.ucar.edu:/web/ftp/pub/$(PACKAGE)/

web-update:	install install-html
	cd $(DESTDIR)$(htmldir) && \
            rsync $(RSYNC_FLAGS) -aCu --delete --delete-excluded \
                $(DESTDIR)$(datarootdir)/ \
                www.unidata.ucar.edu:/web/content/software/$(PACKAGE)/$(distName)
	ssh www.unidata.ucar.edu rm -f /web/content/software/$(PACKAGE)/$(PACKAGE)-current
	ssh www.unidata.ucar.edu ln -s $(distName) /web/content/software/$(PACKAGE)/$(PACKAGE)-current

download-update:
	cat /web/content/downloads/udunits/toc.xml \
	    | sed -e \
		's/"%current%" value=".*"/"%current%" value="$(VERSION)"/' \
		>toc.xml.new
	cp /web/content/downloads/$(PACKAGE)/toc.xml \
	    /web/content/downloads/$(PACKAGE)/toc.xml.old
	mv toc.xml.new /web/content/downloads/udunits/toc.xml

available:	ftp web-update download-update

release: 	ftp web-update

.PHONY:	hostchecks hostcheck remote-checks web-update ftp \
	download-update available release

$(srcdir)/version.texi:  $(srcdir)/stamp-vti
	@cp $(srcdir)/stamp-vti $@

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = @PACKAGE@.pc

CMakeLists.txt:	CHANGE_LOG
	versionId=`awk '{print $$1;exit}' CHANGE_LOG`; \
	if ! `echo "$$versionId" | egrep '^[0-9.]+$$' >/dev/null`; then \
	    echo "First word in file CHANGE_LOG isn't a version ID"; \
	    exit 1; \
	fi; \
	if test `sed -nr '/^PROJECT/{s/.* ([0-9.]+).*$$/\1/p;q}' CMakeLists.txt` \!= \
		$$versionId; then \
	    sed -r '/^PROJECT/s/ [0-9][0-9.]*/ '$$versionId'/' CMakeLists.txt \
		>CMakeLists.txt.tmp; \
	    mv CMakeLists.txt.tmp CMakeLists.txt; \
	else \
	    touch $@; \
	fi;

