#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(UDUNITS, 2.0, support-udunits@unidata.ucar.edu)
AC_CONFIG_AUX_DIR([.])
AC_CONFIG_SRCDIR([lib/converter.c])
AM_INIT_AUTOMAKE([foreign subdir-objects no-installinfo])
AC_CONFIG_HEADERS([config.h expat/expat_config.h])

CFLAGS_COVERAGE=''
LIBS_COVERAGE=''
AC_ARG_ENABLE([coverage],
[  --enable-coverage       Turn on code-coverage support],
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
[  --enable-debug          Turn on debugging support],
[case "${enableval}" in
  yes)
    CFLAGS="${CFLAGS:+$CFLAGS }-g"
    debug=true ;;
  no)
    CFLAGS="${CFLAGS:+$CFLAGS }-O"
    debug=false ;;
  *) AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
esac],
[if test "$coverage_enabled" = true; then
    CFLAGS="${CFLAGS:+$CFLAGS }-g"
    debug=true
else
    debug=false
fi
])
AM_CONDITIONAL([DEBUG], [test x$debug = xtrue])

# Ensure that compilation is optimized and with assertions disabled by default.
if test x$debug \!= xtrue; then
    CFLAGS="-O${CFLAGS:+ $CFLAGS}"
    CPPFLAGS="-DNDEBUG${CPPFLAGS:+ $CPPFLAGS}"
fi

# Checks for programs.
AC_PROG_CC
AM_PROG_CC_C_O
if test "$ac_cv_prog_cc_${ac_cc}_c_o" = yes; then
    case "$CFLAGS" in
	"-g") ;;
	*) CFLAGS="${CFLAGS:+$CFLAGS }-g";;
    esac
fi
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_YACC
AM_PROG_LEX

# Checks for libraries.

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([float.h inttypes.h stddef.h stdlib.h string.h strings.h])

# Check for CUNIT unit-testing package
AC_MSG_NOTICE([Checking for CUNIT unit-testing package.])
AC_CHECK_FILE([/opt/include/CUnit/CUnit.h],
    [CPPFLAGS_CUNIT=-I/opt/include
    AC_SUBST(CPPFLAGS_CUNIT)
    AC_CHECK_FILE([/opt/lib/libcunit.so],
	[LIBS_CUNIT='-L/opt/lib -lcunit'
	AC_SUBST(LIBS_CUNIT)],
	[AC_CHECK_FILE([/opt/lib/libcunit.a],
	    [LIBS_CUNIT='-L/opt/lib -lcunit'
	    AC_SUBST(LIBS_CUNIT)])])],
    [AC_CHECK_FILE([/usr/local/include/CUnit/CUnit.h],
	[CPPFLAGS_CUNIT=-I/usr/local/include
	AC_SUBST(CPPFLAGS_CUNIT)
	AC_CHECK_FILE([/usr/local/lib/libcunit.so],
	    [LIBS_CUNIT='-L/usr/local/lib -lcunit'
	    AC_SUBST(LIBS_CUNIT)],
	    [AC_CHECK_FILE([/usr/local/lib/libcunit.a],
		[LIBS_CUNIT='-L/usr/local/lib -lcunit'
		AC_SUBST(LIBS_CUNIT)])])])])
if test "$CPPFLAGS_CUNIT" -a "$LIBS_CUNIT"; then
    AC_MSG_NOTICE([CUNIT found.  Enabling unit-tests.])
    AM_CONDITIONAL([HAVE_CUNIT], [true])
else
    AC_MSG_NOTICE([CUNIT not found.  Disabling unit-tests.])
    AM_CONDITIONAL([HAVE_CUNIT], [false])
fi

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T

# Needed for the Expat subpackage:
AC_C_BIGENDIAN([byteorder=4321],[byteorder=1234])
AC_DEFINE_UNQUOTED([BYTEORDER],[$byteorder], [Define to 4321 for big-endian and 1234 for little-endian])
AC_DEFINE([XML_CONTEXT_BYTES], 1024, [Define to specify how much context to retain around the current parse point.])


# Checks for library functions.
AC_CHECK_FUNCS([floor memmove memset modf pow strcasecmp strdup strpbrk])

AC_PROG_LIBTOOL

AC_CONFIG_FILES([Makefile
		 expat/Makefile
                 lib/Makefile
                 installdirs.texi
		 prog/Makefile])
AC_OUTPUT
