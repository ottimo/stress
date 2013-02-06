#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT(stress, 2.1.0, matteo.rosi@gmail.com)
#AM_INIT_AUTOMAKE([stress], [2.1.0])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_CONFIG_SRCDIR([lib/composite_factory.cc])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
#AC_PROG_LIBTOOL
AC_PROG_RANLIB
AC_PROG_YACC
AM_PROG_LEX
AC_DECL_YYTEXT
AC_PROG_MAKE_SET

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([string.h])
AC_CHECK_HEADERS([boost/asio.hpp])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_UINT8_T

# Checks for library functions.
AC_CHECK_FUNCS([memset])

AC_CONFIG_SRCDIR([src/main.cc])

AC_CONFIG_FILES([Makefile lib/Makefile lib/state/Makefile lib/actions/Makefile src/Makefile])
#AC_CONFIG_SUBDIRS([lib src])
#AC_SUBDIRS([src lib])
AC_OUTPUT
