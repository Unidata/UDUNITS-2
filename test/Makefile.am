# Copyright 2008, 2009 University Corporation for Atmospheric Research
#
# This file is part of the UDUNITS-2 package.  See the file LICENSE
# in the top-level source-directory of the package for copying and
# redistribution conditions.
#
## Process this file with automake to produce Makefile.in
EXTRA_DIST = \
    results.tab
distName = $(PACKAGE)-$(VERSION)
distArchive = $(top_srcdir)/$(distName).tar.gz
topTmpDir = /tmp/$(distName)

# The following rule is executed on the remote system.
hostcheck: $(distArchive)
	-test -d $(topTmpDir) && \
	    (chmod -R +w $(topTmpDir) && rm -rf $(topTmpDir))
	gunzip -c $(distArchive) | \
	    (mkdir $(topTmpDir) && cd $(topTmpDir) && pax -r)
	if cd $(topTmpDir)/$(distName); then \
	    eval ./configure --prefix=$(topTmpDir) --enable-debug \
		$(SHARED_OPT) CC='$(CC)'; \
	    status=$$?; \
	    if test $$status -eq 0; then \
		$(MAKE) all check install install-html \
		    install-pdf uninstall; \
		status=$$?; \
	    fi; \
	    cd /tmp && rm -rf $(topTmpDir); \
	    exit $$status; \
	fi

# The following rule starts multiple, parallel, asynchronous builds.
hostchecks:	$(distArchive)
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

results.tab:
	ls *.results | sed 's/.results//' | \
	while read host; do \
	    IFS=':' read uname shell <$$host.host || break; \
	    id=`ssh $$host "$$uname" </dev/null` || break; \
	    sed -n "s/^/$$host:$$id:/p" $$host.results || break; \
	done >$@

remote-checks:	hostchecks results.tab

.PHONY:	hostchecks hostcheck remote-checks

## The following entries are necessary to stop make(1)s that don't
## understand the .PHONY target from trying to create the given files.
install-info-am:
install-html-am:
install-pdf-am:
