# file      : template/Makefile.am
# copyright : not copyrighted - public domain

EXTRA_DIST = README pimpl-mysql-vc8.vcproj pimpl-sqlite-vc8.vcproj pimpl-pgsql-vc8.vcproj pimpl-oracle-vc8.vcproj pimpl-mssql-vc8.vcproj pimpl-mysql-vc9.vcproj pimpl-sqlite-vc9.vcproj pimpl-pgsql-vc9.vcproj pimpl-oracle-vc9.vcproj pimpl-mssql-vc9.vcproj pimpl-mysql-vc10.vcxproj pimpl-sqlite-vc10.vcxproj pimpl-pgsql-vc10.vcxproj pimpl-oracle-vc10.vcxproj pimpl-mssql-vc10.vcxproj pimpl-mysql-vc10.vcxproj.filters pimpl-sqlite-vc10.vcxproj.filters pimpl-pgsql-vc10.vcxproj.filters pimpl-oracle-vc10.vcxproj.filters pimpl-mssql-vc10.vcxproj.filters pimpl-mysql-vc11.vcxproj pimpl-sqlite-vc11.vcxproj pimpl-pgsql-vc11.vcxproj pimpl-oracle-vc11.vcxproj pimpl-mssql-vc11.vcxproj pimpl-mysql-vc11.vcxproj.filters pimpl-sqlite-vc11.vcxproj.filters pimpl-pgsql-vc11.vcxproj.filters pimpl-oracle-vc11.vcxproj.filters pimpl-mssql-vc11.vcxproj.filters pimpl-mysql-vc12.vcxproj pimpl-sqlite-vc12.vcxproj pimpl-pgsql-vc12.vcxproj pimpl-oracle-vc12.vcxproj pimpl-mssql-vc12.vcxproj pimpl-mysql-vc12.vcxproj.filters pimpl-sqlite-vc12.vcxproj.filters pimpl-pgsql-vc12.vcxproj.filters pimpl-oracle-vc12.vcxproj.filters pimpl-mssql-vc12.vcxproj.filters

noinst_PROGRAMS = driver
driver_SOURCES = driver.cxx database.hxx person.cxx 

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
	$(ODB) $(ODBCPPFLAGS) $(CPPFLAGS) $(ODBFLAGS) --database @database@ --generate-schema --generate-query --table-prefix pimpl_ $<
