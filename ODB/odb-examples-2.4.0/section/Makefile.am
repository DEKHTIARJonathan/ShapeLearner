# file      : template/Makefile.am
# copyright : not copyrighted - public domain

EXTRA_DIST = README section-mysql-vc8.vcproj section-sqlite-vc8.vcproj section-pgsql-vc8.vcproj section-oracle-vc8.vcproj section-mssql-vc8.vcproj section-mysql-vc9.vcproj section-sqlite-vc9.vcproj section-pgsql-vc9.vcproj section-oracle-vc9.vcproj section-mssql-vc9.vcproj section-mysql-vc10.vcxproj section-sqlite-vc10.vcxproj section-pgsql-vc10.vcxproj section-oracle-vc10.vcxproj section-mssql-vc10.vcxproj section-mysql-vc10.vcxproj.filters section-sqlite-vc10.vcxproj.filters section-pgsql-vc10.vcxproj.filters section-oracle-vc10.vcxproj.filters section-mssql-vc10.vcxproj.filters section-mysql-vc11.vcxproj section-sqlite-vc11.vcxproj section-pgsql-vc11.vcxproj section-oracle-vc11.vcxproj section-mssql-vc11.vcxproj section-mysql-vc11.vcxproj.filters section-sqlite-vc11.vcxproj.filters section-pgsql-vc11.vcxproj.filters section-oracle-vc11.vcxproj.filters section-mssql-vc11.vcxproj.filters section-mysql-vc12.vcxproj section-sqlite-vc12.vcxproj section-pgsql-vc12.vcxproj section-oracle-vc12.vcxproj section-mssql-vc12.vcxproj section-mysql-vc12.vcxproj.filters section-sqlite-vc12.vcxproj.filters section-pgsql-vc12.vcxproj.filters section-oracle-vc12.vcxproj.filters section-mssql-vc12.vcxproj.filters

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
	$(ODB) $(ODBCPPFLAGS) $(CPPFLAGS) $(ODBFLAGS) --database @database@ --generate-schema --table-prefix section_ $<
