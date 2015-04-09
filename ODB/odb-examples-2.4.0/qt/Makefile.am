# file      : template/Makefile.am
# copyright : not copyrighted - public domain

EXTRA_DIST = README qt4-mysql-vc8.vcproj qt4-sqlite-vc8.vcproj qt4-pgsql-vc8.vcproj qt4-oracle-vc8.vcproj qt4-mssql-vc8.vcproj qt4-mysql-vc9.vcproj qt4-sqlite-vc9.vcproj qt4-pgsql-vc9.vcproj qt4-oracle-vc9.vcproj qt4-mssql-vc9.vcproj qt4-mysql-vc10.vcxproj qt4-sqlite-vc10.vcxproj qt4-pgsql-vc10.vcxproj qt4-oracle-vc10.vcxproj qt4-mssql-vc10.vcxproj qt4-mysql-vc10.vcxproj.filters qt4-sqlite-vc10.vcxproj.filters qt4-pgsql-vc10.vcxproj.filters qt4-oracle-vc10.vcxproj.filters qt4-mssql-vc10.vcxproj.filters qt4-mysql-vc11.vcxproj qt4-sqlite-vc11.vcxproj qt4-pgsql-vc11.vcxproj qt4-oracle-vc11.vcxproj qt4-mssql-vc11.vcxproj qt4-mysql-vc11.vcxproj.filters qt4-sqlite-vc11.vcxproj.filters qt4-pgsql-vc11.vcxproj.filters qt4-oracle-vc11.vcxproj.filters qt4-mssql-vc11.vcxproj.filters qt4-mysql-vc12.vcxproj qt4-sqlite-vc12.vcxproj qt4-pgsql-vc12.vcxproj qt4-oracle-vc12.vcxproj qt4-mssql-vc12.vcxproj qt4-mysql-vc12.vcxproj.filters qt4-sqlite-vc12.vcxproj.filters qt4-pgsql-vc12.vcxproj.filters qt4-oracle-vc12.vcxproj.filters qt4-mssql-vc12.vcxproj.filters qt5-mysql-vc9.vcproj qt5-sqlite-vc9.vcproj qt5-pgsql-vc9.vcproj qt5-oracle-vc9.vcproj qt5-mssql-vc9.vcproj qt5-mysql-vc10.vcxproj qt5-sqlite-vc10.vcxproj qt5-pgsql-vc10.vcxproj qt5-oracle-vc10.vcxproj qt5-mssql-vc10.vcxproj qt5-mysql-vc10.vcxproj.filters qt5-sqlite-vc10.vcxproj.filters qt5-pgsql-vc10.vcxproj.filters qt5-oracle-vc10.vcxproj.filters qt5-mssql-vc10.vcxproj.filters qt5-mysql-vc11.vcxproj qt5-sqlite-vc11.vcxproj qt5-pgsql-vc11.vcxproj qt5-oracle-vc11.vcxproj qt5-mssql-vc11.vcxproj qt5-mysql-vc11.vcxproj.filters qt5-sqlite-vc11.vcxproj.filters qt5-pgsql-vc11.vcxproj.filters qt5-oracle-vc11.vcxproj.filters qt5-mssql-vc11.vcxproj.filters qt5-mysql-vc12.vcxproj qt5-sqlite-vc12.vcxproj qt5-pgsql-vc12.vcxproj qt5-oracle-vc12.vcxproj qt5-mssql-vc12.vcxproj qt5-mysql-vc12.vcxproj.filters qt5-sqlite-vc12.vcxproj.filters qt5-pgsql-vc12.vcxproj.filters qt5-oracle-vc12.vcxproj.filters qt5-mssql-vc12.vcxproj.filters qt4-mysql-vc8.sln qt4-sqlite-vc8.sln qt4-pgsql-vc8.sln qt4-oracle-vc8.sln qt4-mssql-vc8.sln qt4-mysql-vc9.sln qt4-sqlite-vc9.sln qt4-pgsql-vc9.sln qt4-oracle-vc9.sln qt4-mssql-vc9.sln qt4-mysql-vc10.sln qt4-sqlite-vc10.sln qt4-pgsql-vc10.sln qt4-oracle-vc10.sln qt4-mssql-vc10.sln qt4-mysql-vc11.sln qt4-sqlite-vc11.sln qt4-pgsql-vc11.sln qt4-oracle-vc11.sln qt4-mssql-vc11.sln qt4-mysql-vc12.sln qt4-sqlite-vc12.sln qt4-pgsql-vc12.sln qt4-oracle-vc12.sln qt4-mssql-vc12.sln qt5-mysql-vc9.sln qt5-sqlite-vc9.sln qt5-pgsql-vc9.sln qt5-oracle-vc9.sln qt5-mssql-vc9.sln qt5-mysql-vc10.sln qt5-sqlite-vc10.sln qt5-pgsql-vc10.sln qt5-oracle-vc10.sln qt5-mssql-vc10.sln qt5-mysql-vc11.sln qt5-sqlite-vc11.sln qt5-pgsql-vc11.sln qt5-oracle-vc11.sln qt5-mssql-vc11.sln qt5-mysql-vc12.sln qt5-sqlite-vc12.sln qt5-pgsql-vc12.sln qt5-oracle-vc12.sln qt5-mssql-vc12.sln

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
	$(ODB) $(ODBCPPFLAGS) $(CPPFLAGS) $(ODBFLAGS) --database @database@ --generate-schema --generate-query --generate-session --profile qt --table-prefix qt_ $<
