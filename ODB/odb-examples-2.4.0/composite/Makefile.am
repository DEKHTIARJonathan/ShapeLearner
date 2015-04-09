# file      : template/Makefile.am
# copyright : not copyrighted - public domain

EXTRA_DIST = README composite-mysql-vc8.vcproj composite-sqlite-vc8.vcproj composite-pgsql-vc8.vcproj composite-oracle-vc8.vcproj composite-mssql-vc8.vcproj composite-mysql-vc9.vcproj composite-sqlite-vc9.vcproj composite-pgsql-vc9.vcproj composite-oracle-vc9.vcproj composite-mssql-vc9.vcproj composite-mysql-vc10.vcxproj composite-sqlite-vc10.vcxproj composite-pgsql-vc10.vcxproj composite-oracle-vc10.vcxproj composite-mssql-vc10.vcxproj composite-mysql-vc10.vcxproj.filters composite-sqlite-vc10.vcxproj.filters composite-pgsql-vc10.vcxproj.filters composite-oracle-vc10.vcxproj.filters composite-mssql-vc10.vcxproj.filters composite-mysql-vc11.vcxproj composite-sqlite-vc11.vcxproj composite-pgsql-vc11.vcxproj composite-oracle-vc11.vcxproj composite-mssql-vc11.vcxproj composite-mysql-vc11.vcxproj.filters composite-sqlite-vc11.vcxproj.filters composite-pgsql-vc11.vcxproj.filters composite-oracle-vc11.vcxproj.filters composite-mssql-vc11.vcxproj.filters composite-mysql-vc12.vcxproj composite-sqlite-vc12.vcxproj composite-pgsql-vc12.vcxproj composite-oracle-vc12.vcxproj composite-mssql-vc12.vcxproj composite-mysql-vc12.vcxproj.filters composite-sqlite-vc12.vcxproj.filters composite-pgsql-vc12.vcxproj.filters composite-oracle-vc12.vcxproj.filters composite-mssql-vc12.vcxproj.filters

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
	$(ODB) $(ODBCPPFLAGS) $(CPPFLAGS) $(ODBFLAGS) --database @database@ --generate-schema --generate-query --table-prefix c_ $<
