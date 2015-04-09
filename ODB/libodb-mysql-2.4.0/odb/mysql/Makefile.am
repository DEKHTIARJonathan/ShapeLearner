# file      : odb/mysql/Makefile.am
# copyright : Copyright (c) 2009-2015 Code Synthesis Tools CC
# license   : GNU GPL v2; see accompanying LICENSE file

lib_LTLIBRARIES = libodb-mysql.la
libodb_mysql_la_SOURCES = connection.cxx connection-factory.cxx database.cxx enum.cxx error.cxx exceptions.cxx prepared-query.cxx query.cxx query-dynamic.cxx query-const-expr.cxx simple-object-statements.cxx statement.cxx statements-base.cxx tracer.cxx traits.cxx transaction.cxx transaction-impl.cxx details/options.cxx

odbmysqlincludedir = $(includedir)/odb/mysql/
nobase_odbmysqlinclude_HEADERS = auto-handle.hxx binding.hxx connection-factory.hxx connection.hxx connection.ixx container-statements.hxx container-statements.txx database.hxx database.ixx details/config.hxx details/conversion.hxx details/export.hxx details/options.hxx details/options.ixx enum.hxx error.hxx exceptions.hxx forward.hxx mysql-types.hxx mysql.hxx no-id-object-result.hxx no-id-object-result.txx no-id-object-statements.hxx no-id-object-statements.txx polymorphic-object-result.hxx polymorphic-object-result.txx polymorphic-object-statements.hxx polymorphic-object-statements.txx prepared-query.hxx query-dynamic.hxx query-dynamic.ixx query-dynamic.txx query.hxx query.ixx query.txx section-statements.hxx section-statements.txx simple-object-result.hxx simple-object-result.txx simple-object-statements.hxx simple-object-statements.ixx simple-object-statements.txx statement-cache.hxx statement-cache.txx statement.hxx statements-base.hxx tracer.hxx traits-calls.hxx traits.hxx transaction-impl.hxx transaction-impl.ixx transaction.hxx transaction.ixx version.hxx view-result.hxx view-result.txx view-statements.hxx view-statements.txx
nobase_nodist_odbmysqlinclude_HEADERS = details/config.h

EXTRA_DIST = details/options.cli details/config.h.in libodb-mysql-vc8.vcproj libodb-mysql-vc9.vcproj libodb-mysql-vc10.vcxproj libodb-mysql-vc10.vcxproj.filters libodb-mysql-vc11.vcxproj libodb-mysql-vc11.vcxproj.filters libodb-mysql-vc12.vcxproj libodb-mysql-vc12.vcxproj.filters

AM_CPPFLAGS= -I'$(top_builddir)' -I'$(top_srcdir)' -DLIBODB_MYSQL_DYNAMIC_LIB
AM_LDFLAGS = -release 2.4 -no-undefined
