# file      : template/Makefile.am
# copyright : not copyrighted - public domain

EXTRA_DIST = README relationship-mysql-vc8.vcproj relationship-sqlite-vc8.vcproj relationship-pgsql-vc8.vcproj relationship-oracle-vc8.vcproj relationship-mssql-vc8.vcproj relationship-mysql-vc9.vcproj relationship-sqlite-vc9.vcproj relationship-pgsql-vc9.vcproj relationship-oracle-vc9.vcproj relationship-mssql-vc9.vcproj relationship-mysql-vc10.vcxproj relationship-sqlite-vc10.vcxproj relationship-pgsql-vc10.vcxproj relationship-oracle-vc10.vcxproj relationship-mssql-vc10.vcxproj relationship-mysql-vc10.vcxproj.filters relationship-sqlite-vc10.vcxproj.filters relationship-pgsql-vc10.vcxproj.filters relationship-oracle-vc10.vcxproj.filters relationship-mssql-vc10.vcxproj.filters relationship-mysql-vc11.vcxproj relationship-sqlite-vc11.vcxproj relationship-pgsql-vc11.vcxproj relationship-oracle-vc11.vcxproj relationship-mssql-vc11.vcxproj relationship-mysql-vc11.vcxproj.filters relationship-sqlite-vc11.vcxproj.filters relationship-pgsql-vc11.vcxproj.filters relationship-oracle-vc11.vcxproj.filters relationship-mssql-vc11.vcxproj.filters relationship-mysql-vc12.vcxproj relationship-sqlite-vc12.vcxproj relationship-pgsql-vc12.vcxproj relationship-oracle-vc12.vcxproj relationship-mssql-vc12.vcxproj relationship-mysql-vc12.vcxproj.filters relationship-sqlite-vc12.vcxproj.filters relationship-pgsql-vc12.vcxproj.filters relationship-oracle-vc12.vcxproj.filters relationship-mssql-vc12.vcxproj.filters

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
	$(ODB) $(ODBCPPFLAGS) $(CPPFLAGS) $(ODBFLAGS) --database @database@ --generate-query --generate-schema --generate-session --default-pointer std::tr1::shared_ptr --table-prefix relation_ $<
