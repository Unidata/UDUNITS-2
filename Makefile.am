# Copyright 2013 University Corporation for Atmospheric Research
#
# This file is part of the UDUNITS-2 package.  See the file COPYRIGHT
# in the top-level source-directory of the package for copying and
# redistribution conditions.
#
## Process this file with automake to produce Makefile.in
ACLOCAL_AMFLAGS		= -I m4
SUBDIRS			= lib prog
DIST_SUBDIRS		= lib prog
info_TEXINFOS		= udunits2.texi
EXTRA_DIST = \
    ANNOUNCEMENT \
    CHANGE_LOG \
    CMakeLists.txt \
    config.h.cmake \
    COPYRIGHT \
    udunits2.html \
    udunits2.pdf
AM_MAKEINFOHTMLFLAGS 	= --no-split
RSYNC_FLAGS		= --rsh=ssh --rsync-path=/usr/bin/rsync
distName 		= $(PACKAGE)-$(VERSION)
distArchive 		= $(distName).tar.gz
DISTCLEANFILES		= *.log $(distArchive)
dist_doc_DATA		= CHANGE_LOG COPYRIGHT README

verify_on_master_branch:
	git branch | grep '^\* *master$$' >/dev/null

dist:	verify_on_master_branch

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
	download-update available release verify_on_master_branch

udunits2.info:	version.texi COPYRIGHT
udunits2.html:	version.texi COPYRIGHT
udunits2.pdf:	version.texi COPYRIGHT

$(srcdir)/version.texi:  $(srcdir)/stamp-vti
	@cp $(srcdir)/stamp-vti $@
