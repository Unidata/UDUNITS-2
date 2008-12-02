## Process this file with automake to produce Makefile.in
SUBDIRS	= expat lib prog
info_TEXINFOS = udunits2.texi
udunits2_TEXINFOS = success.texi failure.texi make.texi
EXTRA_DIST = \
    udunits2.html \
    udunits2.pdf \
    results.tab \
    success.texi \
    failure.texi \
    make.texi \
    ANNOUNCEMENT \
    CHANGE_LOG
AM_MAKEINFOHTMLFLAGS = --no-split
# DISTCHECK_CONFIGURE_FLAGS = 
RSYNC_FLAGS	= --rsh=ssh --rsync-path=/opt/bin/rsync

# The following rule is executed on the remote system.
hostcheck:
	-test -d /tmp/$(distdir) && \
	    (chmod -R +w /tmp/$(distdir) && rm -rf /tmp/$(distdir))
	gunzip -c $(DIST_ARCHIVES) | \
	    (mkdir /tmp/$(distdir) && cd /tmp/$(distdir) && pax -r)
	if cd /tmp/$(distdir)/$(distdir); then \
	    eval ./configure --prefix=/tmp/$(distdir) --enable-debug \
		$(SHARED_OPT) CC='$(CC)'; \
	    status=$$?; \
	    if test $$status -eq 0; then \
		$(MAKE) all check install install-html \
		    install-pdf uninstall; \
		status=$$?; \
	    fi; \
	    cd /tmp && rm -rf /tmp/$(distdir); \
	    exit $$status; \
	fi

# The following rule starts a build process on a remote system.
.host.check:
	rm -f $*.results
	IFS=':' read uname shell <$<; \
	sed -n '2,$$p' $< | \
	while IFS=':' read make cc; do \
	    if ssh $* "$$shell \"cd $$PWD && $$make hostcheck MAKE="$$make" CC='$$cc'\"" </dev/null; then \
		echo "$$make:$$cc::1" >>$*.results; \
	    else \
		echo $$make:$$cc::0 >>$*.results; \
		if ssh $* "$$shell \"cd $$PWD && $$make hostcheck MAKE="$$make" CC='$$cc' SHARED_OPT=--disable-shared \"" </dev/null; then \
		    echo $$make:$$cc:--disable-shared:1 >>$*.results; \
		else \
		    echo $$make:$$cc:--disable-shared:0 >>$*.results; \
		fi; \
	    fi; \
	done
	touch redo-results

# The following rule starts multiple, parallel, asynchronous builds.
hostchecks:	$(DIST_ARCHIVES)
	rm -f $@ results.tab
	hosts=`ls *.host | sed 's/.host//'`; \
	for host in $$hosts; do \
	    $(MAKE) -s $$host.check >$$host.log 2>&1 & pid="$$!"; \
	    echo $$host $$pid >>$@; \
	    echo "Building on $$host"; \
	done; \
	status=0; \
	while read host pid; do \
	    echo "Waiting on $$host"; \
	    wait $$pid; \
	    stat=$$?; \
	    if test $$stat -ne 0; then \
		echo 1>&2 \
		    "Build on \"$$host\" exited with status $$stat.  " \
		    "See file \"$$host.log\"."; \
		status=$$(($$status+1)); \
	    fi; \
	done <$@; \
	echo "Done waiting"; \
	exit $$status

results.tab:
	ls *.results | sed 's/.results//' | \
	while read host; do \
	    IFS=':' read uname shell <$$host.host || break; \
	    id=`ssh $$host "$$uname" </dev/null` || break; \
	    sed -n "s/^/$$host:$$id:/p" $$host.results || break; \
	done >$@

success.texi:	results.tab
	sort -u -t : -k 2,2 -k 4,4 -k 5,5 results.tab | \
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

failure.texi:	results.tab
	sort -u -t : -k 2,2 -k 4,4 -k 5,5 results.tab | \
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

make.texi:	results.tab
	awk -F: '$$6 == 1 {printf "%s:%s\n", $$2, $$3}' results.tab | \
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

remote-checks:	distcheck hostchecks results.tab

web-update:	install-html
	cd $(DESTDIR)$(htmldir) && \
	rsync $(RSYNC_FLAGS) -aCu --delete --delete-excluded \
	    *.html \
	    conan:/content/software/$(PACKAGE)/$(PACKAGE_TARNAME)-$(PACKAGE_VERSION)

.PHONY:	hostchecks hostcheck remote-checks web-update

udunits2.info:	success.texi failure.texi make.texi
udunits2.html:	success.texi failure.texi make.texi
udunits2.pdf:	success.texi failure.texi make.texi

$(srcdir)/version.texi:  $(srcdir)/stamp-vti
	@cp $(srcdir)/stamp-vti $@
