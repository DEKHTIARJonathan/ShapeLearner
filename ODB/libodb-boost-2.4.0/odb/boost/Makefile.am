# file      : odb/boost/Makefile.am
# copyright : Copyright (c) 2009-2015 Code Synthesis Tools CC
# license   : GNU GPL v2; see accompanying LICENSE file

lib_LTLIBRARIES = libodb-boost.la
libodb_boost_la_SOURCES = date-time/exceptions.cxx

odbboostincludedir = $(includedir)/odb/boost/
nobase_odbboostinclude_HEADERS = version.hxx uuid/mssql/uuid-mapping.hxx uuid/mssql/uuid-traits.hxx uuid/sqlite/uuid-mapping.hxx uuid/sqlite/uuid-traits.hxx uuid/pgsql/uuid-mapping.hxx uuid/pgsql/uuid-traits.hxx uuid/mysql/uuid-mapping.hxx uuid/mysql/uuid-traits.hxx uuid/oracle/uuid-mapping.hxx uuid/oracle/uuid-traits.hxx unordered/container-traits.hxx lazy-ptr.hxx exception.hxx smart-ptr/pointer-traits.hxx smart-ptr/lazy-ptr.hxx smart-ptr/lazy-pointer-traits.hxx smart-ptr/lazy-ptr.ixx smart-ptr/wrapper-traits.hxx smart-ptr/lazy-ptr.txx date-time/mssql/posix-time-traits.hxx date-time/mssql/gregorian-mapping.hxx date-time/mssql/posix-time-mapping.hxx date-time/mssql/gregorian-traits.hxx date-time/sqlite/posix-time-traits.hxx date-time/sqlite/gregorian-mapping.hxx date-time/sqlite/posix-time-mapping.hxx date-time/sqlite/gregorian-traits.hxx date-time/pgsql/posix-time-traits.hxx date-time/pgsql/gregorian-mapping.hxx date-time/pgsql/posix-time-mapping.hxx date-time/pgsql/gregorian-traits.hxx date-time/mysql/posix-time-traits.hxx date-time/mysql/gregorian-mapping.hxx date-time/mysql/posix-time-mapping.hxx date-time/mysql/gregorian-traits.hxx date-time/oracle/posix-time-traits.hxx date-time/oracle/gregorian-mapping.hxx date-time/oracle/posix-time-mapping.hxx date-time/oracle/gregorian-traits.hxx date-time/exceptions.hxx details/export.hxx details/config.hxx multi-index/container-traits.hxx optional/wrapper-traits.hxx unordered.options smart-ptr.options uuid/uuid-pgsql.options uuid/uuid-sqlite.options uuid/uuid-mysql.options uuid/uuid-mssql.options uuid/uuid-oracle.options uuid/uuid-common.options uuid.options date-time.options version.options date-time/posix-time-sqlite.options date-time/posix-time-mssql.options date-time/posix-time-pgsql.options date-time/gregorian-mysql.options date-time/gregorian-mssql.options date-time/posix-time-oracle.options date-time/posix-time-mysql.options date-time/posix-time-common.options date-time/gregorian-oracle.options date-time/gregorian-sqlite.options date-time/gregorian-pgsql.options date-time/gregorian-common.options multi-index.options optional.options
nobase_nodist_odbboostinclude_HEADERS = details/config.h

EXTRA_DIST = libodb-boost-vc8.vcproj libodb-boost-vc9.vcproj libodb-boost-vc10.vcxproj libodb-boost-vc10.vcxproj.filters libodb-boost-vc11.vcxproj libodb-boost-vc11.vcxproj.filters libodb-boost-vc12.vcxproj libodb-boost-vc12.vcxproj.filters

AM_CPPFLAGS= -I'$(top_builddir)' -I'$(top_srcdir)' -DLIBODB_BOOST_DYNAMIC_LIB
AM_LDFLAGS = -release 2.4 -no-undefined
