dnl file      : m4/libmysqlclient.m4
dnl copyright : Copyright (c) 2009-2015 Code Synthesis Tools CC
dnl license   : GNU GPL v2; see accompanying LICENSE file
dnl
dnl LIBMYSQLCLIENT(MULTI-THREADED=none|<other>,
dnl                [ACTION-IF-FOUND[,
dnl                ACTION-IF-NOT-FOUND]])
dnl
dnl
AC_DEFUN([LIBMYSQLCLIENT], [
libmysqlclient_found=no

if test x"$1" != xnone; then
  libmysqlclient_name=mysqlclient_r
else
  libmysqlclient_name=mysqlclient
fi

AC_MSG_CHECKING([for lib$libmysqlclient_name])

save_LIBS="$LIBS"
LIBS="-l$libmysqlclient_name $LIBS"

# Some distributions (e.g., Fedora) hide the library in the mysql/
# subdirectory.
#
libmysqlclient_paths="none /usr/lib/mysql /usr/lib64/mysql"

for path in $libmysqlclient_paths; do

if test x"path" != xnone; then
  save_LDFLAGS="$LDFLAGS"
  LDFLAGS="$LDFLAGS -L$path"
fi

CXX_LIBTOOL_LINK_IFELSE([
AC_LANG_SOURCE([
#ifdef _WIN32
#  include <winsock2.h>
#endif
#include <mysql/mysql.h>

int
main ()
{
  MYSQL handle;
  mysql_init (&handle);
  mysql_real_connect (&handle, 0, 0, 0, 0, 0, 0, 0);
  MYSQL_STMT* stmt = mysql_stmt_init (&handle);
  mysql_stmt_close (stmt);
  mysql_close (&handle);
}
])],
[
libmysqlclient_found=yes
libmysqlclient_include=long
])

if test x"$libmysqlclient_found" = xyes; then
  break;
fi

CXX_LIBTOOL_LINK_IFELSE([
AC_LANG_SOURCE([
#ifdef _WIN32
#  include <winsock2.h>
#endif
#include <mysql.h>

int
main ()
{
  MYSQL handle;
  mysql_init (&handle);
  mysql_real_connect (&handle, 0, 0, 0, 0, 0, 0, 0);
  MYSQL_STMT* stmt = mysql_stmt_init (&handle);
  mysql_stmt_close (stmt);
  mysql_close (&handle);
}
])],
[
libmysqlclient_found=yes
libmysqlclient_include=short
])

if test x"$libmysqlclient_found" = xyes; then
  break;
fi

if test x"path" != xnone; then
  LDFLAGS="$save_LDFLAGS"
fi

done

if test x"$libmysqlclient_found" = xno; then
  LIBS="$save_LIBS"
fi

if test x"$libmysqlclient_found" = xyes; then
  AC_MSG_RESULT([yes])
  $2
else
  AC_MSG_RESULT([no])
  $3
fi

# Check if the THR_KEY_mysys pthread key symbol is visible.
#
libmysqlclient_thr_key_visible=no

if test x"$libmysqlclient_found" = xyes -a x"$1" = xposix; then

CXX_LIBTOOL_LINK_IFELSE([
AC_LANG_SOURCE([
#include <pthread.h>
extern pthread_key_t THR_KEY_mysys;
int
main ()
{
  return pthread_getspecific (THR_KEY_mysys) != 0;
}
])],
[
libmysqlclient_thr_key_visible=yes
])
fi

])dnl
