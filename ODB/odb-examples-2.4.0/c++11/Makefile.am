# file      : template/Makefile.am
# copyright : not copyrighted - public domain

EXTRA_DIST = README c++11-mysql-vc10.vcxproj c++11-sqlite-vc10.vcxproj c++11-pgsql-vc10.vcxproj c++11-oracle-vc10.vcxproj c++11-mssql-vc10.vcxproj c++11-mysql-vc10.vcxproj.filters c++11-sqlite-vc10.vcxproj.filters c++11-pgsql-vc10.vcxproj.filters c++11-oracle-vc10.vcxproj.filters c++11-mssql-vc10.vcxproj.filters c++11-mysql-vc11.vcxproj c++11-sqlite-vc11.vcxproj c++11-pgsql-vc11.vcxproj c++11-oracle-vc11.vcxproj c++11-mssql-vc11.vcxproj c++11-mysql-vc11.vcxproj.filters c++11-sqlite-vc11.vcxproj.filters c++11-pgsql-vc11.vcxproj.filters c++11-oracle-vc11.vcxproj.filters c++11-mssql-vc11.vcxproj.filters c++11-mysql-vc12.vcxproj c++11-sqlite-vc12.vcxproj c++11-pgsql-vc12.vcxproj c++11-oracle-vc12.vcxproj c++11-mssql-vc12.vcxproj c++11-mysql-vc12.vcxproj.filters c++11-sqlite-vc12.vcxproj.filters c++11-pgsql-vc12.vcxproj.filters c++11-oracle-vc12.vcxproj.filters c++11-mssql-vc12.vcxproj.filters

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
	$(ODB) $(ODBCPPFLAGS) $(CPPFLAGS) $(ODBFLAGS) --database @database@ --generate-query --generate-schema --table-prefix boost_ $<
