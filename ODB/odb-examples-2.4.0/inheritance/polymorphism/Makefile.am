# file      : template/Makefile.am
# copyright : not copyrighted - public domain

EXTRA_DIST = README inheritance-polymorphism-mysql-vc8.vcproj inheritance-polymorphism-sqlite-vc8.vcproj inheritance-polymorphism-pgsql-vc8.vcproj inheritance-polymorphism-oracle-vc8.vcproj inheritance-polymorphism-mssql-vc8.vcproj inheritance-polymorphism-mysql-vc9.vcproj inheritance-polymorphism-sqlite-vc9.vcproj inheritance-polymorphism-pgsql-vc9.vcproj inheritance-polymorphism-oracle-vc9.vcproj inheritance-polymorphism-mssql-vc9.vcproj inheritance-polymorphism-mysql-vc10.vcxproj inheritance-polymorphism-sqlite-vc10.vcxproj inheritance-polymorphism-pgsql-vc10.vcxproj inheritance-polymorphism-oracle-vc10.vcxproj inheritance-polymorphism-mssql-vc10.vcxproj inheritance-polymorphism-mysql-vc10.vcxproj.filters inheritance-polymorphism-sqlite-vc10.vcxproj.filters inheritance-polymorphism-pgsql-vc10.vcxproj.filters inheritance-polymorphism-oracle-vc10.vcxproj.filters inheritance-polymorphism-mssql-vc10.vcxproj.filters inheritance-polymorphism-mysql-vc11.vcxproj inheritance-polymorphism-sqlite-vc11.vcxproj inheritance-polymorphism-pgsql-vc11.vcxproj inheritance-polymorphism-oracle-vc11.vcxproj inheritance-polymorphism-mssql-vc11.vcxproj inheritance-polymorphism-mysql-vc11.vcxproj.filters inheritance-polymorphism-sqlite-vc11.vcxproj.filters inheritance-polymorphism-pgsql-vc11.vcxproj.filters inheritance-polymorphism-oracle-vc11.vcxproj.filters inheritance-polymorphism-mssql-vc11.vcxproj.filters inheritance-polymorphism-mysql-vc12.vcxproj inheritance-polymorphism-sqlite-vc12.vcxproj inheritance-polymorphism-pgsql-vc12.vcxproj inheritance-polymorphism-oracle-vc12.vcxproj inheritance-polymorphism-mssql-vc12.vcxproj inheritance-polymorphism-mysql-vc12.vcxproj.filters inheritance-polymorphism-sqlite-vc12.vcxproj.filters inheritance-polymorphism-pgsql-vc12.vcxproj.filters inheritance-polymorphism-oracle-vc12.vcxproj.filters inheritance-polymorphism-mssql-vc12.vcxproj.filters

noinst_PROGRAMS = driver
driver_SOURCES = driver.cxx database.hxx employee.cxx 

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
	$(ODB) $(ODBCPPFLAGS) $(CPPFLAGS) $(ODBFLAGS) --database @database@ --generate-schema --generate-query --table-prefix inh_poly_ $<
