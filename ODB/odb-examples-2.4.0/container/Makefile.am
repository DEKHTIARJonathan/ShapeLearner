# file      : template/Makefile.am
# copyright : not copyrighted - public domain

EXTRA_DIST = README container-mysql-vc8.vcproj container-sqlite-vc8.vcproj container-pgsql-vc8.vcproj container-oracle-vc8.vcproj container-mssql-vc8.vcproj container-mysql-vc9.vcproj container-sqlite-vc9.vcproj container-pgsql-vc9.vcproj container-oracle-vc9.vcproj container-mssql-vc9.vcproj container-mysql-vc10.vcxproj container-sqlite-vc10.vcxproj container-pgsql-vc10.vcxproj container-oracle-vc10.vcxproj container-mssql-vc10.vcxproj container-mysql-vc10.vcxproj.filters container-sqlite-vc10.vcxproj.filters container-pgsql-vc10.vcxproj.filters container-oracle-vc10.vcxproj.filters container-mssql-vc10.vcxproj.filters container-mysql-vc11.vcxproj container-sqlite-vc11.vcxproj container-pgsql-vc11.vcxproj container-oracle-vc11.vcxproj container-mssql-vc11.vcxproj container-mysql-vc11.vcxproj.filters container-sqlite-vc11.vcxproj.filters container-pgsql-vc11.vcxproj.filters container-oracle-vc11.vcxproj.filters container-mssql-vc11.vcxproj.filters container-mysql-vc12.vcxproj container-sqlite-vc12.vcxproj container-pgsql-vc12.vcxproj container-oracle-vc12.vcxproj container-mssql-vc12.vcxproj container-mysql-vc12.vcxproj.filters container-sqlite-vc12.vcxproj.filters container-pgsql-vc12.vcxproj.filters container-oracle-vc12.vcxproj.filters container-mssql-vc12.vcxproj.filters

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
	$(ODB) $(ODBCPPFLAGS) $(CPPFLAGS) $(ODBFLAGS) --database @database@ --generate-schema --table-prefix container_ $<
