# file      : template/Makefile.am
# copyright : not copyrighted - public domain

EXTRA_DIST = README access-mysql-vc8.vcproj access-sqlite-vc8.vcproj access-pgsql-vc8.vcproj access-oracle-vc8.vcproj access-mssql-vc8.vcproj access-mysql-vc9.vcproj access-sqlite-vc9.vcproj access-pgsql-vc9.vcproj access-oracle-vc9.vcproj access-mssql-vc9.vcproj access-mysql-vc10.vcxproj access-sqlite-vc10.vcxproj access-pgsql-vc10.vcxproj access-oracle-vc10.vcxproj access-mssql-vc10.vcxproj access-mysql-vc10.vcxproj.filters access-sqlite-vc10.vcxproj.filters access-pgsql-vc10.vcxproj.filters access-oracle-vc10.vcxproj.filters access-mssql-vc10.vcxproj.filters access-mysql-vc11.vcxproj access-sqlite-vc11.vcxproj access-pgsql-vc11.vcxproj access-oracle-vc11.vcxproj access-mssql-vc11.vcxproj access-mysql-vc11.vcxproj.filters access-sqlite-vc11.vcxproj.filters access-pgsql-vc11.vcxproj.filters access-oracle-vc11.vcxproj.filters access-mssql-vc11.vcxproj.filters access-mysql-vc12.vcxproj access-sqlite-vc12.vcxproj access-pgsql-vc12.vcxproj access-oracle-vc12.vcxproj access-mssql-vc12.vcxproj access-mysql-vc12.vcxproj.filters access-sqlite-vc12.vcxproj.filters access-pgsql-vc12.vcxproj.filters access-oracle-vc12.vcxproj.filters access-mssql-vc12.vcxproj.filters

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
	$(ODB) $(ODBCPPFLAGS) $(CPPFLAGS) $(ODBFLAGS) --database @database@ --generate-schema --generate-query --table-prefix access_ $<
