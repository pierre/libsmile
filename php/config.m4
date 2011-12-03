dnl $Id$
dnl config.m4 for extension libsmile

PHP_ARG_ENABLE(libsmile, whether to enable Smile support for PHP,
               [  --enable-libsmile           Enable Smile support for PHP])


dnl Tell the build system about the C libsmile library
C_LIBSMILE_PATH=../
C_LIBSMILE_INCLUDE_PATH=../include/
C_LIBSMILE_NAME=smile
C_LIBSMILE_SYMBOL=smile_decode_block

PHP_CHECK_LIBRARY($C_LIBSMILE_NAME, $C_LIBSMILE_SYMBOL,
[
  PHP_ADD_INCLUDE($C_LIBSMILE_INCLUDE_PATH)
  PHP_ADD_LIBRARY_WITH_PATH($C_LIBSMILE_NAME, $C_LIBSMILE_PATH, LIBSMILE_SHARED_LIBADD)
  AC_DEFINE(HAVE_LIBSMILELIB,1,[Whether C libsmile support is present and requested])
],[
  AC_MSG_ERROR([C libsmile not found])
],[
  -L$C_LIBSMILE_PATH
])

if test "$PHP_LIBSMILE" != "no"; then
  PHP_NEW_EXTENSION(libsmile, php_libsmile.c, $ext_shared)
  PHP_SUBST(LIBSMILE_SHARED_LIBADD)
fi
