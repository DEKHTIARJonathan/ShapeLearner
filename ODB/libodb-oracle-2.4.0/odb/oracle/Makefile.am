# file      : odb/oracle/Makefile.am
# copyright : Copyright (c) 2009-2015 Code Synthesis Tools CC
# license   : ODB NCUEL; see accompanying LICENSE file

lib_LTLIBRARIES = libodb-oracle.la
libodb_oracle_la_SOURCES = auto-descriptor.cxx auto-handle.cxx connection.cxx connection-factory.cxx database.cxx error.cxx exceptions.cxx oracle-types.cxx prepared-query.cxx query.cxx query-dynamic.cxx query-const-expr.cxx simple-object-statements.cxx statement.cxx statements-base.cxx tracer.cxx traits.cxx transaction.cxx transaction-impl.cxx details/number.cxx details/options.cxx

odboracleincludedir = $(includedir)/odb/oracle/
nobase_odboracleinclude_HEADERS = auto-descriptor.hxx auto-handle.hxx binding.hxx connection-factory.hxx connection.hxx connection.ixx container-statements.hxx container-statements.txx database.hxx database.ixx details/config.hxx details/conversion.hxx details/date.hxx details/export.hxx details/number.hxx details/options.hxx details/options.ixx error.hxx exceptions.hxx forward.hxx no-id-object-result.hxx no-id-object-result.txx no-id-object-statements.hxx no-id-object-statements.txx oracle-fwd.hxx oracle-types.hxx polymorphic-object-result.hxx polymorphic-object-result.txx polymorphic-object-statements.hxx polymorphic-object-statements.txx prepared-query.hxx query-dynamic.hxx query-dynamic.ixx query-dynamic.txx query.hxx query.ixx query.txx section-statements.hxx section-statements.txx simple-object-result.hxx simple-object-result.txx simple-object-statements.hxx simple-object-statements.ixx simple-object-statements.txx statement-cache.hxx statement-cache.txx statement.hxx statement.ixx statements-base.hxx tracer.hxx traits-calls.hxx traits.hxx traits.txx transaction-impl.hxx transaction-impl.ixx transaction.hxx transaction.ixx version.hxx view-result.hxx view-result.txx view-statements.hxx view-statements.txx
nobase_nodist_odboracleinclude_HEADERS = details/config.h

EXTRA_DIST = details/options.cli details/config.h.in libodb-oracle-vc8.vcproj libodb-oracle-vc9.vcproj libodb-oracle-vc10.vcxproj libodb-oracle-vc10.vcxproj.filters libodb-oracle-vc11.vcxproj libodb-oracle-vc11.vcxproj.filters libodb-oracle-vc12.vcxproj libodb-oracle-vc12.vcxproj.filters

AM_CPPFLAGS= -I'$(top_builddir)' -I'$(top_srcdir)' -DLIBODB_ORACLE_DYNAMIC_LIB
AM_LDFLAGS = -release 2.4 -no-undefined
