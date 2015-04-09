# file      : template/Makefile.am
# copyright : not copyrighted - public domain

EXTRA_DIST = README mapping-mysql-vc8.vcproj mapping-sqlite-vc8.vcproj mapping-pgsql-vc8.vcproj mapping-oracle-vc8.vcproj mapping-mssql-vc8.vcproj mapping-mysql-vc9.vcproj mapping-sqlite-vc9.vcproj mapping-pgsql-vc9.vcproj mapping-oracle-vc9.vcproj mapping-mssql-vc9.vcproj mapping-mysql-vc10.vcxproj mapping-sqlite-vc10.vcxproj mapping-pgsql-vc10.vcxproj mapping-oracle-vc10.vcxproj mapping-mssql-vc10.vcxproj mapping-mysql-vc10.vcxproj.filters mapping-sqlite-vc10.vcxproj.filters mapping-pgsql-vc10.vcxproj.filters mapping-oracle-vc10.vcxproj.filters mapping-mssql-vc10.vcxproj.filters mapping-mysql-vc11.vcxproj mapping-sqlite-vc11.vcxproj mapping-pgsql-vc11.vcxproj mapping-oracle-vc11.vcxproj mapping-mssql-vc11.vcxproj mapping-mysql-vc11.vcxproj.filters mapping-sqlite-vc11.vcxproj.filters mapping-pgsql-vc11.vcxproj.filters mapping-oracle-vc11.vcxproj.filters mapping-mssql-vc11.vcxproj.filters mapping-mysql-vc12.vcxproj mapping-sqlite-vc12.vcxproj mapping-pgsql-vc12.vcxproj mapping-oracle-vc12.vcxproj mapping-mssql-vc12.vcxproj mapping-mysql-vc12.vcxproj.filters mapping-sqlite-vc12.vcxproj.filters mapping-pgsql-vc12.vcxproj.filters mapping-oracle-vc12.vcxproj.filters mapping-mssql-vc12.vcxproj.filters

noinst_PROGRAMS = driver
driver_SOURCES = driver.cxx database.hxx  traits.hxx traits-mysql.hxx traits-sqlite.hxx traits-pgsql.hxx traits-oracle.hxx traits-mssql.hxx

AM_CPPFLAGS = -I'$(builddir)' -I'$(srcdir)'

if DATABASE_MYSQL
AM_CPPFLAGS += -DDATABASE_MYSQL
endif

if DATABASE_SQLITE
AM_CPPFLAGS += -DDATABASE_SQLITE
endif

if DATABASE_PGSQL
AM_CPPFLAGS += -DDATABASE_PGSQL
endif

if DATABASE_ORACLE
AM_CPPFLAGS += -DDATABASE_ORACLE
endif

if DATABASE_MSSQL
AM_CPPFLAGS += -DDATABASE_MSSQL
endif

TESTS=$(top_builddir)/tester
TESTS_ENVIRONMENT=top_builddir=$(top_builddir); export top_builddir;

# ODB compilation.
#
driver_SOURCES += person.hxx
nodist_driver_SOURCES = person-odb.cxx
BUILT_SOURCES = person-odb.hxx
CLEANFILES = person-odb.hxx person-odb.ixx person-odb.cxx

ODB = @ODB@
ODBFLAGS = @ODBFLAGS@
ODBCPPFLAGS = @ODBCPPFLAGS@

if HAVE_CXX11
ODBFLAGS += --std c++11
endif

person-odb.hxx: person.hxx
	$(ODB) $(ODBCPPFLAGS) $(CPPFLAGS) $(ODBFLAGS) --database @database@ --generate-schema --generate-query --hxx-prologue '#include "traits.hxx"' --table-prefix mapping_ $<
