#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(UDUNITS, 2.0, support@unidata.ucar.edu)
AC_CONFIG_AUX_DIR([.])
AC_CONFIG_SRCDIR([lib/converter.c])
AM_INIT_AUTOMAKE([foreign subdir-objects])
AC_CONFIG_HEADER([config.h])

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
esac],[debug=false])
AM_CONDITIONAL([DEBUG], [test x$debug = xtrue])

# Ensure that the CFLAGS default is "-O" and not "-g" or "-g -O2"
CFLAGS=${CFLAGS--O}

# Checks for programs.
AC_PROG_CC
AM_PROG_CC_C_O
if test "$ac_cv_prog_cc_${ac_cc}_c_o" == yes; then
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
save_LIBS=$LIBS
LIBS=
AC_SEARCH_LIBS([CU_basic_run_tests], [cunit],
[LD_CUNIT=$LIBS
AC_SUBST(LD_CUNIT)
LIBS=$save_LIBS
cunit=true], [cunit=false])
AM_CONDITIONAL([HAVE_CUNIT], [test x$cunit = xtrue])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([float.h inttypes.h stddef.h stdlib.h string.h strings.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([floor memmove memset modf pow strcasecmp strdup strpbrk])

AC_PROG_LIBTOOL

AC_CONFIG_FILES([Makefile
		 expat/Makefile
                 lib/Makefile
		 udunits2/Makefile])
AC_OUTPUT
