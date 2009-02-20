## Process this file with automake to produce Makefile.in
SUBDIRS	= expat lib prog test
info_TEXINFOS = udunits2.texi
udunits2_TEXINFOS = success.texi failure.texi make.texi
EXTRA_DIST = \
    udunits2.html \
    udunits2.pdf \
    ANNOUNCEMENT \
    CHANGE_LOG
AM_MAKEINFOHTMLFLAGS = --no-split
# DISTCHECK_CONFIGURE_FLAGS = 
RSYNC_FLAGS	= --rsh=ssh --rsync-path=/opt/bin/rsync

success.texi:	
	sort -u -t : -k 2,2 -k 4,4 -k 5,5 test/results.tab | \
	awk -F: ' \
	    BEGIN { \
		print "@multitable {@code{Linux 2.6.18-1.2257.fc5smp}} {@code{/opt/csw/gcc4/bin/gcc}} {@code{--disable-shared}}"; \
		print "@headitem O/S @tab Compiler @tab @code{configure} Option"; \
	    } \
	    $$6 == 1 { \
		printf "@item @code{%s} @tab @code{%s} @tab @code{%s}\n", \
		    $$2, $$4, $$5; \
	    } \
	    END { \
		print "@end multitable"; \
	    }' >success.texi

failure.texi:	
	sort -u -t : -k 2,2 -k 4,4 -k 5,5 test/results.tab | \
	awk -F: ' \
	    BEGIN { \
		print "@multitable {@code{Linux 2.6.18-1.2257.fc5smp}} {@code{/opt/SUNWspro/bin/c99}} {@code{--disable-shared}}"; \
		print "@headitem O/S @tab Compiler @tab @code{configure} Option"; \
	    } \
	    $$6 == 0 { \
		printf "@item @code{%s} @tab @code{%s} @tab @code{%s}\n", \
		    $$2, $$4, $$5; \
	    } \
	    END { \
		print "@end multitable"; \
	    }' >failure.texi

make.texi:	
	awk -F: '$$6 == 1 {printf "%s:%s\n", $$2, $$3}' test/results.tab | \
	sort -t : -u | \
	awk -F: ' \
	    BEGIN { \
		print "@multitable {@code{Linux 2.6.18-1.2257.fc5smp}} {@code{/usr/bin/posix/make}}"; \
		print "@headitem O/S @tab @code{make} Utility"; \
	    } \
	    { \
		printf "@item @code{%s} @tab @code{%s}\n", $$1, $$2; \
	    } \
	    END { \
		print "@end multitable"; \
	    }' >$@

remote-checks:	distcheck
	cd test && $(MAKE) $(AM_MAKEFLAGS) $@

ftp:	$(PACKAGE)-$(VERSION).tar.gz
	scp $(PACKAGE)-$(VERSION).tar.gz conan:/web/ftp/pub/$(PACKAGE)/

web-update:	install-html
	cd $(DESTDIR)$(htmldir) && \
	rsync $(RSYNC_FLAGS) -aCu --delete --delete-excluded \
	    *.html \
	    conan:/content/software/$(PACKAGE)/$(PACKAGE)-$(VERSION)

.PHONY:	hostchecks hostcheck remote-checks web-update

udunits2.info:	success.texi failure.texi make.texi
udunits2.html:	success.texi failure.texi make.texi
udunits2.pdf:	success.texi failure.texi make.texi

$(srcdir)/version.texi:  $(srcdir)/stamp-vti
	@cp $(srcdir)/stamp-vti $@
