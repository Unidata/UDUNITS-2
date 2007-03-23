## Process this file with automake to produce Makefile.in
SUBDIRS	= expat lib prog
info_TEXINFOS = udunits-2.texi
EXTRA_DIST = success.texi failure.texi
AM_MAKEINFOHTMLFLAGS = --no-split
# DISTCHECK_CONFIGURE_FLAGS = 

# The following rule is executed on the remote system.
hostcheck:	$(DIST_ARCHIVES)
	-test -d /tmp/$(distdir) && \
	    (chmod -R +w /tmp/$(distdir) && rm -rf /tmp/$(distdir))
	gunzip -c $(DIST_ARCHIVES) | \
	    (mkdir /tmp/$(distdir) && cd /tmp/$(distdir) && tar xf -)
	cd /tmp/$(distdir)/$(distdir) && \
	    eval ./configure --prefix=/tmp/$(distdir) --enable-debug \
		$(SHARED_OPT) CC='$(CC)' && \
	    $(MAKE) all check install install-info install-html install-pdf \
		uninstall; \
	    status=$$?; \
	    cd /tmp && rm -rf /tmp/$(distdir); \
	    exit $$status

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
hostchecks:	distcheck
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
	exit $$status

results.tab:	redo-results
	rm -f $@
	ls *.results | sed 's/.results//' | \
	while read host; do \
	    IFS=':' read uname shell <$$host.host || break; \
	    id=`ssh $$host "$$uname" </dev/null` || break; \
	    sed -n "s/^/$$host:$$id:/p" $$host.results || break; \
	done >$@

success.tab:	results.tab
	sort -t : -k +2 results.tab | \
	awk -F: ' \
	    BEGIN { \
		print "@multitable {Linux 2.6.18-1.2257.fc5smp} {/opt/csw/gcc4/bin/gcc} {--disable-shared}"; \
		print "@headitem O/S @tab Compiler @tab @code{configure} Option"; \
	    } \
	    $$6 == 1 { \
		print "@item",$$2,"@tab",$$4,"@tab",$$5; \
	    } \
	    END { \
		print "@end multitable"; \
	    }' >success.texi

failure.tab:	results.tab
	sort -t : -k +2 results.tab | \
	awk -F: ' \
	    BEGIN { \
		print "@multitable {Linux 2.6.18-1.2257.fc5smp} {/opt/SUNWspro/bin/c99} {--disable-shared}"; \
		print "@headitem O/S @tab Compiler @tab @code{configure} Option"; \
	    } \
	    $$6 == 0 { \
		print "@item",$$2,"@tab",$$4,"@tab",$$5; \
	    } \
	    END { \
		print "@end multitable"; \
	    }' >failure.texi

remote-checks:	hostchecks success.tab failure.tab

.PHONY:	hostchecks hostcheck remote-checks

## The following entries are necessary to stop make(1)s that don't
## understand the .PHONY target from trying to create the given files.
install-info-am:
install-html-am:
install-pdf-am:
