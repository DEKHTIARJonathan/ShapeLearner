# file      : template/Makefile.am
# copyright : not copyrighted - public domain

EXTRA_DIST = README optimistic-mysql-vc8.vcproj optimistic-sqlite-vc8.vcproj optimistic-pgsql-vc8.vcproj optimistic-oracle-vc8.vcproj optimistic-mssql-vc8.vcproj optimistic-mysql-vc9.vcproj optimistic-sqlite-vc9.vcproj optimistic-pgsql-vc9.vcproj optimistic-oracle-vc9.vcproj optimistic-mssql-vc9.vcproj optimistic-mysql-vc10.vcxproj optimistic-sqlite-vc10.vcxproj optimistic-pgsql-vc10.vcxproj optimistic-oracle-vc10.vcxproj optimistic-mssql-vc10.vcxproj optimistic-mysql-vc10.vcxproj.filters optimistic-sqlite-vc10.vcxproj.filters optimistic-pgsql-vc10.vcxproj.filters optimistic-oracle-vc10.vcxproj.filters optimistic-mssql-vc10.vcxproj.filters optimistic-mysql-vc11.vcxproj optimistic-sqlite-vc11.vcxproj optimistic-pgsql-vc11.vcxproj optimistic-oracle-vc11.vcxproj optimistic-mssql-vc11.vcxproj optimistic-mysql-vc11.vcxproj.filters optimistic-sqlite-vc11.vcxproj.filters optimistic-pgsql-vc11.vcxproj.filters optimistic-oracle-vc11.vcxproj.filters optimistic-mssql-vc11.vcxproj.filters optimistic-mysql-vc12.vcxproj optimistic-sqlite-vc12.vcxproj optimistic-pgsql-vc12.vcxproj optimistic-oracle-vc12.vcxproj optimistic-mssql-vc12.vcxproj optimistic-mysql-vc12.vcxproj.filters optimistic-sqlite-vc12.vcxproj.filters optimistic-pgsql-vc12.vcxproj.filters optimistic-oracle-vc12.vcxproj.filters optimistic-mssql-vc12.vcxproj.filters

noinst_PROGRAMS = driver
driver_SOURCES = driver.cxx database.hxx  

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
	$(ODB) $(ODBCPPFLAGS) $(CPPFLAGS) $(ODBFLAGS) --database @database@ --generate-schema --table-prefix optimistic_ $<
