# file      : template/Makefile.am
# copyright : not copyrighted - public domain

EXTRA_DIST = README query-mysql-vc8.vcproj query-sqlite-vc8.vcproj query-pgsql-vc8.vcproj query-oracle-vc8.vcproj query-mssql-vc8.vcproj query-mysql-vc9.vcproj query-sqlite-vc9.vcproj query-pgsql-vc9.vcproj query-oracle-vc9.vcproj query-mssql-vc9.vcproj query-mysql-vc10.vcxproj query-sqlite-vc10.vcxproj query-pgsql-vc10.vcxproj query-oracle-vc10.vcxproj query-mssql-vc10.vcxproj query-mysql-vc10.vcxproj.filters query-sqlite-vc10.vcxproj.filters query-pgsql-vc10.vcxproj.filters query-oracle-vc10.vcxproj.filters query-mssql-vc10.vcxproj.filters query-mysql-vc11.vcxproj query-sqlite-vc11.vcxproj query-pgsql-vc11.vcxproj query-oracle-vc11.vcxproj query-mssql-vc11.vcxproj query-mysql-vc11.vcxproj.filters query-sqlite-vc11.vcxproj.filters query-pgsql-vc11.vcxproj.filters query-oracle-vc11.vcxproj.filters query-mssql-vc11.vcxproj.filters query-mysql-vc12.vcxproj query-sqlite-vc12.vcxproj query-pgsql-vc12.vcxproj query-oracle-vc12.vcxproj query-mssql-vc12.vcxproj query-mysql-vc12.vcxproj.filters query-sqlite-vc12.vcxproj.filters query-pgsql-vc12.vcxproj.filters query-oracle-vc12.vcxproj.filters query-mssql-vc12.vcxproj.filters

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
	$(ODB) $(ODBCPPFLAGS) $(CPPFLAGS) $(ODBFLAGS) --database @database@ --generate-schema --generate-query --table-prefix query_ $<
