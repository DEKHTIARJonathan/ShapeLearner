# file      : template/Makefile.am
# copyright : not copyrighted - public domain

EXTRA_DIST = README prepared-mysql-vc8.vcproj prepared-sqlite-vc8.vcproj prepared-pgsql-vc8.vcproj prepared-oracle-vc8.vcproj prepared-mssql-vc8.vcproj prepared-mysql-vc9.vcproj prepared-sqlite-vc9.vcproj prepared-pgsql-vc9.vcproj prepared-oracle-vc9.vcproj prepared-mssql-vc9.vcproj prepared-mysql-vc10.vcxproj prepared-sqlite-vc10.vcxproj prepared-pgsql-vc10.vcxproj prepared-oracle-vc10.vcxproj prepared-mssql-vc10.vcxproj prepared-mysql-vc10.vcxproj.filters prepared-sqlite-vc10.vcxproj.filters prepared-pgsql-vc10.vcxproj.filters prepared-oracle-vc10.vcxproj.filters prepared-mssql-vc10.vcxproj.filters prepared-mysql-vc11.vcxproj prepared-sqlite-vc11.vcxproj prepared-pgsql-vc11.vcxproj prepared-oracle-vc11.vcxproj prepared-mssql-vc11.vcxproj prepared-mysql-vc11.vcxproj.filters prepared-sqlite-vc11.vcxproj.filters prepared-pgsql-vc11.vcxproj.filters prepared-oracle-vc11.vcxproj.filters prepared-mssql-vc11.vcxproj.filters prepared-mysql-vc12.vcxproj prepared-sqlite-vc12.vcxproj prepared-pgsql-vc12.vcxproj prepared-oracle-vc12.vcxproj prepared-mssql-vc12.vcxproj prepared-mysql-vc12.vcxproj.filters prepared-sqlite-vc12.vcxproj.filters prepared-pgsql-vc12.vcxproj.filters prepared-oracle-vc12.vcxproj.filters prepared-mssql-vc12.vcxproj.filters

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
	$(ODB) $(ODBCPPFLAGS) $(CPPFLAGS) $(ODBFLAGS) --database @database@ --generate-query --generate-schema --generate-prepared $<
