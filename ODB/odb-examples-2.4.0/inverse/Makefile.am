# file      : template/Makefile.am
# copyright : not copyrighted - public domain

EXTRA_DIST = README inverse-mysql-vc8.vcproj inverse-sqlite-vc8.vcproj inverse-pgsql-vc8.vcproj inverse-oracle-vc8.vcproj inverse-mssql-vc8.vcproj inverse-mysql-vc9.vcproj inverse-sqlite-vc9.vcproj inverse-pgsql-vc9.vcproj inverse-oracle-vc9.vcproj inverse-mssql-vc9.vcproj inverse-mysql-vc10.vcxproj inverse-sqlite-vc10.vcxproj inverse-pgsql-vc10.vcxproj inverse-oracle-vc10.vcxproj inverse-mssql-vc10.vcxproj inverse-mysql-vc10.vcxproj.filters inverse-sqlite-vc10.vcxproj.filters inverse-pgsql-vc10.vcxproj.filters inverse-oracle-vc10.vcxproj.filters inverse-mssql-vc10.vcxproj.filters inverse-mysql-vc11.vcxproj inverse-sqlite-vc11.vcxproj inverse-pgsql-vc11.vcxproj inverse-oracle-vc11.vcxproj inverse-mssql-vc11.vcxproj inverse-mysql-vc11.vcxproj.filters inverse-sqlite-vc11.vcxproj.filters inverse-pgsql-vc11.vcxproj.filters inverse-oracle-vc11.vcxproj.filters inverse-mssql-vc11.vcxproj.filters inverse-mysql-vc12.vcxproj inverse-sqlite-vc12.vcxproj inverse-pgsql-vc12.vcxproj inverse-oracle-vc12.vcxproj inverse-mssql-vc12.vcxproj inverse-mysql-vc12.vcxproj.filters inverse-sqlite-vc12.vcxproj.filters inverse-pgsql-vc12.vcxproj.filters inverse-oracle-vc12.vcxproj.filters inverse-mssql-vc12.vcxproj.filters

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
	$(ODB) $(ODBCPPFLAGS) $(CPPFLAGS) $(ODBFLAGS) --database @database@ --generate-query --generate-schema --generate-session --default-pointer std::tr1::shared_ptr --table-prefix inverse_ $<
