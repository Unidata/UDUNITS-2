#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(UDUNITS, 2.0, support@unidata.ucar.edu)
AC_CONFIG_SRCDIR([lib/converter.c])
AM_INIT_AUTOMAKE([foreign subdir-objects])
AC_CONFIG_HEADER([config.h])

case "$CFLAGS" in
    *-g*)  YFLAGS="${YFLAGS:+$YFLAGS }-t"
	    AC_SUBST(YFLAGS)
	    LFLAGS="${LFLAGS:+$LFLAGS }-d"
	    AC_SUBST(LFLAGS)
	    ;;
esac

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AM_PROG_CC_C_O
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_YACC
AM_PROG_LEX

# Checks for libraries.

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
                 lib/Makefile])
AC_OUTPUT
