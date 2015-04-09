# file      : template/Makefile.am
# copyright : not copyrighted - public domain

EXTRA_DIST = README inheritance-reuse-mysql-vc8.vcproj inheritance-reuse-sqlite-vc8.vcproj inheritance-reuse-pgsql-vc8.vcproj inheritance-reuse-oracle-vc8.vcproj inheritance-reuse-mssql-vc8.vcproj inheritance-reuse-mysql-vc9.vcproj inheritance-reuse-sqlite-vc9.vcproj inheritance-reuse-pgsql-vc9.vcproj inheritance-reuse-oracle-vc9.vcproj inheritance-reuse-mssql-vc9.vcproj inheritance-reuse-mysql-vc10.vcxproj inheritance-reuse-sqlite-vc10.vcxproj inheritance-reuse-pgsql-vc10.vcxproj inheritance-reuse-oracle-vc10.vcxproj inheritance-reuse-mssql-vc10.vcxproj inheritance-reuse-mysql-vc10.vcxproj.filters inheritance-reuse-sqlite-vc10.vcxproj.filters inheritance-reuse-pgsql-vc10.vcxproj.filters inheritance-reuse-oracle-vc10.vcxproj.filters inheritance-reuse-mssql-vc10.vcxproj.filters inheritance-reuse-mysql-vc11.vcxproj inheritance-reuse-sqlite-vc11.vcxproj inheritance-reuse-pgsql-vc11.vcxproj inheritance-reuse-oracle-vc11.vcxproj inheritance-reuse-mssql-vc11.vcxproj inheritance-reuse-mysql-vc11.vcxproj.filters inheritance-reuse-sqlite-vc11.vcxproj.filters inheritance-reuse-pgsql-vc11.vcxproj.filters inheritance-reuse-oracle-vc11.vcxproj.filters inheritance-reuse-mssql-vc11.vcxproj.filters inheritance-reuse-mysql-vc12.vcxproj inheritance-reuse-sqlite-vc12.vcxproj inheritance-reuse-pgsql-vc12.vcxproj inheritance-reuse-oracle-vc12.vcxproj inheritance-reuse-mssql-vc12.vcxproj inheritance-reuse-mysql-vc12.vcxproj.filters inheritance-reuse-sqlite-vc12.vcxproj.filters inheritance-reuse-pgsql-vc12.vcxproj.filters inheritance-reuse-oracle-vc12.vcxproj.filters inheritance-reuse-mssql-vc12.vcxproj.filters

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
	$(ODB) $(ODBCPPFLAGS) $(CPPFLAGS) $(ODBFLAGS) --database @database@ --generate-schema --generate-query --table-prefix inh_reuse_ $<
