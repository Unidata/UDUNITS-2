## Process this file with automake to produce Makefile.in
SUBDIRS	= expat lib udunits2
info_TEXINFOS = udunits2.texi
AM_MAKEINFOHTMLFLAGS = --no-split
# DISTCHECK_CONFIGURE_FLAGS = 

hostcheck:
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

hostchecks:	$(DIST_ARCHIVES)
	rm -f $@
	hosts=`ls *.host | sed 's/.host//'`; \
	for host in $$hosts; do \
	    $(MAKE) $$host.check >$$host.log 2>&1 & pid="$$!"; \
	    echo $$host $$pid >>$@; \
	    echo "Building on $$host"; \
	done
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

results.tab:
	rm -f $@
	ls *.results | sed 's/.results//' | \
	while read host; do \
	    IFS=':' read uname shell <$$host.host || break; \
	    id=`ssh $$host "$$uname" </dev/null` || break; \
	    sed -n "2,$$ s/^/$$host:$$id:/p" $$host.results || break; \
	done >$@

success.texi:	results.tab
	awk -F: ' \
	    BEGIN { \
		print "@multitable {Linux 2.6.18-1.2257.fc5smp} {/opt/csw/gcc4/bin/gcc}"; \
		print "@headitem O/S @tab Configure Option @tab Compiler"; \
	    } \
	    $$6 == 1 { \
		print "@item" $$2 "@tab" $$5 "@tab" $$4; \
	    } \
	    END { \
		print "@end multitable"; \
	    }' results.tab >$@

.PHONY:	hostchecks hostcheck results.tab

## The following entries are necessary to stop make(1)s that don't
## understand the .PHONY target from trying to create the given files.
install-info-am:
install-html-am:
install-pdf-am:
