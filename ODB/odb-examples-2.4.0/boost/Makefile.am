# file      : template/Makefile.am
# copyright : not copyrighted - public domain

EXTRA_DIST = README boost-mysql-vc8.vcproj boost-sqlite-vc8.vcproj boost-pgsql-vc8.vcproj boost-oracle-vc8.vcproj boost-mssql-vc8.vcproj boost-mysql-vc9.vcproj boost-sqlite-vc9.vcproj boost-pgsql-vc9.vcproj boost-oracle-vc9.vcproj boost-mssql-vc9.vcproj boost-mysql-vc10.vcxproj boost-sqlite-vc10.vcxproj boost-pgsql-vc10.vcxproj boost-oracle-vc10.vcxproj boost-mssql-vc10.vcxproj boost-mysql-vc10.vcxproj.filters boost-sqlite-vc10.vcxproj.filters boost-pgsql-vc10.vcxproj.filters boost-oracle-vc10.vcxproj.filters boost-mssql-vc10.vcxproj.filters boost-mysql-vc11.vcxproj boost-sqlite-vc11.vcxproj boost-pgsql-vc11.vcxproj boost-oracle-vc11.vcxproj boost-mssql-vc11.vcxproj boost-mysql-vc11.vcxproj.filters boost-sqlite-vc11.vcxproj.filters boost-pgsql-vc11.vcxproj.filters boost-oracle-vc11.vcxproj.filters boost-mssql-vc11.vcxproj.filters boost-mysql-vc12.vcxproj boost-sqlite-vc12.vcxproj boost-pgsql-vc12.vcxproj boost-oracle-vc12.vcxproj boost-mssql-vc12.vcxproj boost-mysql-vc12.vcxproj.filters boost-sqlite-vc12.vcxproj.filters boost-pgsql-vc12.vcxproj.filters boost-oracle-vc12.vcxproj.filters boost-mssql-vc12.vcxproj.filters boost-mysql-vc8.sln boost-sqlite-vc8.sln boost-pgsql-vc8.sln boost-oracle-vc8.sln boost-mssql-vc8.sln boost-mysql-vc9.sln boost-sqlite-vc9.sln boost-pgsql-vc9.sln boost-oracle-vc9.sln boost-mssql-vc9.sln boost-mysql-vc10.sln boost-sqlite-vc10.sln boost-pgsql-vc10.sln boost-oracle-vc10.sln boost-mssql-vc10.sln boost-mysql-vc11.sln boost-sqlite-vc11.sln boost-pgsql-vc11.sln boost-oracle-vc11.sln boost-mssql-vc11.sln boost-mysql-vc12.sln boost-sqlite-vc12.sln boost-pgsql-vc12.sln boost-oracle-vc12.sln boost-mssql-vc12.sln

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
driver_SOURCES += employee.hxx
nodist_driver_SOURCES = employee-odb.cxx
BUILT_SOURCES = employee-odb.hxx
CLEANFILES = employee-odb.hxx employee-odb.ixx employee-odb.cxx

ODB = @ODB@
ODBFLAGS = @ODBFLAGS@
ODBCPPFLAGS = @ODBCPPFLAGS@

if HAVE_CXX11
ODBFLAGS += --std c++11
endif

employee-odb.hxx: employee.hxx
	$(ODB) $(ODBCPPFLAGS) $(CPPFLAGS) $(ODBFLAGS) --database @database@ --profile boost --generate-query --generate-schema --generate-session --table-prefix boost_ $<
