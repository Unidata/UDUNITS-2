#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
#
# Copyright 2013 University Corporation for Atmospheric Research
#
# This file is part of the UDUNITS-2 package.  See the file COPYRIGHT
# in the top-level source-directory of the package for terms and
# conditions.
#

AC_PREREQ(2.59)
AC_INIT([UDUNITS], [2.2.28], [support-udunits@unidata.ucar.edu])
AC_CONFIG_SRCDIR([lib/converter.c])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([foreign subdir-objects])
AC_CONFIG_HEADERS([config.h])
LT_INIT([win32-dll])

test -z "$prefix" -o "$prefix" = NONE && prefix=`(cd .. && pwd)`

CFLAGS_COVERAGE=''
LIBS_COVERAGE=''

AC_ARG_ENABLE([coverage],
[AS_HELP_STRING([--enable-coverage],[Turn on code-coverage support])],
[case "${enableval}" in
  yes) CFLAGS_COVERAGE='--coverage'
       LIBS_COVERAGE=-lgcov
       coverage_enabled=true;;
  no) ;;
  *) AC_MSG_ERROR([bad value ${enableval} for --enable-coverage]) ;;
esac])
AC_SUBST(CFLAGS_COVERAGE)
AC_SUBST(LIBS_COVERAGE)

AC_ARG_ENABLE([debug],
[AS_HELP_STRING([--enable-debug],[Turn on debugging support])],
[case "${enableval}" in
  yes)
    CFLAGS="${CFLAGS:+$CFLAGS }-g -O0"
    debug=true ;;
  no)
    CFLAGS="-O${CFLAGS:+ $CFLAGS}"
    debug=false ;;
  *) AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
esac],
[if test "$coverage_enabled" = true; then
    CFLAGS="-g${CFLAGS:+ $CFLAGS}"
    debug=true
else
    debug=false
fi
])
AM_CONDITIONAL([DEBUG], [test x$debug = xtrue])

AM_CONDITIONAL([ENABLE_UDUNITS_1], [true])
AC_ARG_ENABLE([udunits-1],
    [AS_HELP_STRING([--disable-udunits-1],
        [Turn off support for the UDUNITS-1 API [default=enabled]])],
    [case "${enableval}" in
      no)   AM_CONDITIONAL([ENABLE_UDUNITS_1], [false]) ;;
      yes)  ;;
      *)    AC_MSG_ERROR([bad value ${enableval} for --enable-udunits-1]) ;;
    esac])

# Ensure that compilation is optimized and with assertions disabled by default.
CFLAGS=${CFLAGS:--O}
CPPFLAGS=${CPPFLAGS:--DNDEBUG}

# The default absolute pathname of the installed units database. "pkgdatadir"
# isn't a configure-variable in the normal sense: it doesn't appear in 
# "config.status" yet appears in "Makefile"; consequently, the following
# nonsense just to avoid defining the pathname in the makefile so that Eclipse
# is happy.
pkgdatadir=$(eval echo $(eval echo `echo ${datadir}`/${PACKAGE}))
AC_DEFINE_UNQUOTED([DEFAULT_UDUNITS2_XML_PATH], ["${pkgdatadir}/udunits2.xml"],
    [The default absolute pathname of the installed units database])

# Checks for programs.
AC_PROG_CC
AM_PROG_CC_C_O
#if test "$ac_cv_prog_cc_${ac_cc}_c_o" = yes; then
#    case "$AM_CFLAGS" in
#	"-g") ;;
#	*) AM_CFLAGS="${AM_CFLAGS:+$AM_CFLAGS }-g";;
#    esac
#fi
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_YACC
AM_PROG_LEX
AC_PROG_FC

# NB: Solaris 10 doesn't support _XOPEN_SOURCE beyond 600
AC_DEFINE([_XOPEN_SOURCE], [600], [Level of X/OPEN compliance])

# Checks for libraries.
AC_SEARCH_LIBS([dirname], [gen], , 
    AC_MSG_ERROR([cannot find function dirname]))
AC_SEARCH_LIBS([log10], [m], , 
    AC_MSG_ERROR([cannot find function log10]))
AC_SEARCH_LIBS([XML_StopParser], [expat], , 
    AC_MSG_ERROR([cannot find EXPAT function XML_StopParser]))

AC_CHECK_LIB([dl], [dlopen])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([float.h inttypes.h stddef.h stdlib.h string.h strings.h])

# Checks for the CUNIT unit-testing package
LD_CUNIT=
AC_MSG_NOTICE([Checking for the CUNIT unit-testing package.])
AC_CHECK_HEADER([CUnit/CUnit.h],
    [AC_CHECK_LIB([cunit], [CU_initialize_registry], [LD_CUNIT=-lcunit])])
AC_SUBST([LD_CUNIT])
if test "$LD_CUNIT"; then
    AC_MSG_NOTICE([CUNIT found.  Enabling unit-tests.])
else
    AC_MSG_NOTICE([CUNIT not found.  Disabling unit-tests.])
fi
AM_CONDITIONAL([HAVE_CUNIT], [test "$LD_CUNIT"])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T

# Checks for library functions.
AC_CHECK_FUNCS([floor memmove memset modf pow strcasecmp strdup strpbrk])

AC_PROG_LIBTOOL

AC_CONFIG_FILES([Makefile
                 lib/Makefile
                 lib/xmlFailures/Makefile
                 lib/xmlSuccesses/Makefile
		 prog/Makefile])
AC_OUTPUT
