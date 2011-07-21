AC_LANG(C)
AC_PREREQ([2.65])
AC_INIT([libsmile], [0.1], [pierre@mouraf.org])
AM_INIT_AUTOMAKE([-Wall -Werror])
AC_LANG_WERROR

AC_CONFIG_SRCDIR([src], [test])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CC
AM_PROG_CC_C_O

# Checks for libraries.
LT_INIT

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h limits.h stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

# Checks for library functions.

AC_CONFIG_FILES([Makefile src/Makefile test/Makefile])
AC_OUTPUT
