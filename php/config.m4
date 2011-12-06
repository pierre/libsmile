dnl $Id$
dnl config.m4 for extension libsmile

PHP_ARG_ENABLE(libsmile, whether to enable Smile support for PHP,
               [  --enable-libsmile           Enable Smile support for PHP])

if test "$PHP_LIBSMILE" != "no"; then
    C_LIBSMILE_PATH=../
    C_LIBSMILE_INCLUDE_PATH=../include/
    C_LIBSMILE_NAME=smile

    PHP_ADD_INCLUDE($C_LIBSMILE_INCLUDE_PATH)
    PHP_ADD_LIBRARY_WITH_PATH($C_LIBSMILE_NAME, $C_LIBSMILE_PATH, LIBSMILE_SHARED_LIBADD)
    PHP_SUBST(LIBSMILE_SHARED_LIBADD)

    PHP_NEW_EXTENSION(libsmile, php_libsmile.c, $ext_shared)
fi
