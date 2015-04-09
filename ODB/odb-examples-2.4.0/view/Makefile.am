# file      : template/Makefile.am
# copyright : not copyrighted - public domain

EXTRA_DIST = README view-mysql-vc8.vcproj view-sqlite-vc8.vcproj view-pgsql-vc8.vcproj view-oracle-vc8.vcproj view-mssql-vc8.vcproj view-mysql-vc9.vcproj view-sqlite-vc9.vcproj view-pgsql-vc9.vcproj view-oracle-vc9.vcproj view-mssql-vc9.vcproj view-mysql-vc10.vcxproj view-sqlite-vc10.vcxproj view-pgsql-vc10.vcxproj view-oracle-vc10.vcxproj view-mssql-vc10.vcxproj view-mysql-vc10.vcxproj.filters view-sqlite-vc10.vcxproj.filters view-pgsql-vc10.vcxproj.filters view-oracle-vc10.vcxproj.filters view-mssql-vc10.vcxproj.filters view-mysql-vc11.vcxproj view-sqlite-vc11.vcxproj view-pgsql-vc11.vcxproj view-oracle-vc11.vcxproj view-mssql-vc11.vcxproj view-mysql-vc11.vcxproj.filters view-sqlite-vc11.vcxproj.filters view-pgsql-vc11.vcxproj.filters view-oracle-vc11.vcxproj.filters view-mssql-vc11.vcxproj.filters view-mysql-vc12.vcxproj view-sqlite-vc12.vcxproj view-pgsql-vc12.vcxproj view-oracle-vc12.vcxproj view-mssql-vc12.vcxproj view-mysql-vc12.vcxproj.filters view-sqlite-vc12.vcxproj.filters view-pgsql-vc12.vcxproj.filters view-oracle-vc12.vcxproj.filters view-mssql-vc12.vcxproj.filters

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
	$(ODB) $(ODBCPPFLAGS) $(CPPFLAGS) $(ODBFLAGS) --database @database@ --generate-query --generate-schema --default-pointer std::tr1::shared_ptr --generate-session --table-prefix view_ $<
