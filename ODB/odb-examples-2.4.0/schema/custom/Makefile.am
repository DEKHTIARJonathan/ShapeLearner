# file      : template/Makefile.am
# copyright : not copyrighted - public domain

EXTRA_DIST = README schema-custom-mysql-vc8.vcproj schema-custom-sqlite-vc8.vcproj schema-custom-pgsql-vc8.vcproj schema-custom-oracle-vc8.vcproj schema-custom-mssql-vc8.vcproj schema-custom-mysql-vc9.vcproj schema-custom-sqlite-vc9.vcproj schema-custom-pgsql-vc9.vcproj schema-custom-oracle-vc9.vcproj schema-custom-mssql-vc9.vcproj schema-custom-mysql-vc10.vcxproj schema-custom-sqlite-vc10.vcxproj schema-custom-pgsql-vc10.vcxproj schema-custom-oracle-vc10.vcxproj schema-custom-mssql-vc10.vcxproj schema-custom-mysql-vc10.vcxproj.filters schema-custom-sqlite-vc10.vcxproj.filters schema-custom-pgsql-vc10.vcxproj.filters schema-custom-oracle-vc10.vcxproj.filters schema-custom-mssql-vc10.vcxproj.filters schema-custom-mysql-vc11.vcxproj schema-custom-sqlite-vc11.vcxproj schema-custom-pgsql-vc11.vcxproj schema-custom-oracle-vc11.vcxproj schema-custom-mssql-vc11.vcxproj schema-custom-mysql-vc11.vcxproj.filters schema-custom-sqlite-vc11.vcxproj.filters schema-custom-pgsql-vc11.vcxproj.filters schema-custom-oracle-vc11.vcxproj.filters schema-custom-mssql-vc11.vcxproj.filters schema-custom-mysql-vc12.vcxproj schema-custom-sqlite-vc12.vcxproj schema-custom-pgsql-vc12.vcxproj schema-custom-oracle-vc12.vcxproj schema-custom-mssql-vc12.vcxproj schema-custom-mysql-vc12.vcxproj.filters schema-custom-sqlite-vc12.vcxproj.filters schema-custom-pgsql-vc12.vcxproj.filters schema-custom-oracle-vc12.vcxproj.filters schema-custom-mssql-vc12.vcxproj.filters

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
	$(ODB) $(ODBCPPFLAGS) $(CPPFLAGS) $(ODBFLAGS) --database @database@ --generate-query --generate-session --default-pointer std::tr1::shared_ptr $<
