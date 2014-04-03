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
RSYNC_FLAGS		= --rsh=ssh --rsync-path=/opt/bin/rsync
distName 		= $(PACKAGE)-$(VERSION)
distArchive 		= $(distName).tar.gz
DISTCLEANFILES		= *.log $(distArchive)
dist_doc_DATA		= CHANGE_LOG COPYRIGHT README

remote-checks:	dist
	cd test && $(MAKE) $(AM_MAKEFLAGS) $@
	$(MAKE) udunits2.html

ftp:	dist
	-git commit -a
	git tag -f v$(VERSION)
	cp $(distArchive) /web/ftp/pub/$(PACKAGE)/
	mailx -s '$(distName) now available' -a CHANGE_LOG \
	    ed@unidata.ucar.edu </dev/null

web-update:	install install-html
	cd $(DESTDIR)$(htmldir) \
	&& rsync $(RSYNC_FLAGS) -aCu --delete --delete-excluded \
	    *.html \
	    /web/content/software/$(PACKAGE)/$(distName)
	cd $(DESTDIR)$(pkgdatadir) \
	&& rsync $(RSYNC_FLAGS) -aCu --delete \
	    *.xml \
	    conan:/content/software/$(PACKAGE)/$(distName)
	echo 'cd /content/software/$(PACKAGE) && \
	    rm $(PACKAGE)-2 && ln -s $(distName) $(PACKAGE)-2' | \
	    ssh conan

download-update:
	cat /web/content/downloads/udunits/toc.xml \
	    | sed -e \
		's/"%current%" value=".*"/"%current%" value="$(VERSION)"/' \
		>toc.xml.new
	cp /web/content/downloads/$(PACKAGE)/toc.xml \
	    /web/content/downloads/$(PACKAGE)/toc.xml.old
	mv toc.xml.new /web/content/downloads/udunits/toc.xml

available:	ftp web-update download-update

.PHONY:	hostchecks hostcheck remote-checks web-update ftp \
	download-update available

udunits2.info:	version.texi COPYRIGHT
udunits2.html:	version.texi COPYRIGHT
udunits2.pdf:	version.texi COPYRIGHT

$(srcdir)/version.texi:  $(srcdir)/stamp-vti
	@cp $(srcdir)/stamp-vti $@
