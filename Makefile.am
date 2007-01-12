## Process this file with automake to produce Makefile.in
SUBDIRS	= expat lib udunits2
# DISTCHECK_CONFIGURE_FLAGS = 

remotecheck:    $(DIST_ARCHIVES)
	-test -d /tmp/$(distdir) && \
	    (chmod -R +w /tmp/$(distdir) && rm -rf /tmp/$(distdir))
	gunzip -c $(DIST_ARCHIVES) | \
	    (mkdir /tmp/$(distdir) && cd /tmp/$(distdir) && tar xf -)
	cd /tmp/$(distdir)/$(distdir) && \
	    eval ./configure --prefix=/tmp/$(distdir) --enable-debug $$MACS && \
	    $(MAKE) all check install || exit 1; \
	    $(MAKE) -i install-info; \
	    $(MAKE) -i install-html; \
	    $(MAKE) -i install-pdf; \
	    $(MAKE) uninstall || exit 1;
	rm -rf /tmp/$(distdir)

.remote.check:
	(read shell && \
	while IFS=':' read make macs; do \
	    if echo $$make | grep -qv '^#'; then \
		ssh $* \
		    "$$shell \"cd $$PWD && MACS='$$macs' $$make remotecheck\"" \
		    </dev/null || exit 1; \
	    fi; \
	done) <$<

allremotechecks:
	hosts=`ls *.remote | sed 's/.remote//g'`; \
	for host in $$hosts; do \
	    $(MAKE) $$host.check </dev/null >$$host.log 2>&1 & \
	    eval $${host}Pid="$$!"; \
	done; \
	status=0; \
	for host in $$hosts; do \
	    eval wait \$${$${host}Pid}; \
	    stat=$$?; \
	    if test $$stat -ne 0; then \
		echo 1>&2 "Build on \"$$host\" exited with status $$stat.  " \
		    "See file \"$$host.log\"."; \
		status=$$(($$status+1)); \
	    fi; \
	done; \
	exit $$status

.PHONY:	allremotechecks remotecheck

## The following entries are necessary to stop make(1)s that don't
## understand the .PHONY target from trying to create the given files.
install-info-am:
install-html-am:
install-pdf-am:
