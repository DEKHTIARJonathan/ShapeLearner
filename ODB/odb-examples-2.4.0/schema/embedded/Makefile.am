# file      : template/Makefile.am
# copyright : not copyrighted - public domain

EXTRA_DIST = README schema-embedded-mysql-vc8.vcproj schema-embedded-sqlite-vc8.vcproj schema-embedded-pgsql-vc8.vcproj schema-embedded-oracle-vc8.vcproj schema-embedded-mssql-vc8.vcproj schema-embedded-mysql-vc9.vcproj schema-embedded-sqlite-vc9.vcproj schema-embedded-pgsql-vc9.vcproj schema-embedded-oracle-vc9.vcproj schema-embedded-mssql-vc9.vcproj schema-embedded-mysql-vc10.vcxproj schema-embedded-sqlite-vc10.vcxproj schema-embedded-pgsql-vc10.vcxproj schema-embedded-oracle-vc10.vcxproj schema-embedded-mssql-vc10.vcxproj schema-embedded-mysql-vc10.vcxproj.filters schema-embedded-sqlite-vc10.vcxproj.filters schema-embedded-pgsql-vc10.vcxproj.filters schema-embedded-oracle-vc10.vcxproj.filters schema-embedded-mssql-vc10.vcxproj.filters schema-embedded-mysql-vc11.vcxproj schema-embedded-sqlite-vc11.vcxproj schema-embedded-pgsql-vc11.vcxproj schema-embedded-oracle-vc11.vcxproj schema-embedded-mssql-vc11.vcxproj schema-embedded-mysql-vc11.vcxproj.filters schema-embedded-sqlite-vc11.vcxproj.filters schema-embedded-pgsql-vc11.vcxproj.filters schema-embedded-oracle-vc11.vcxproj.filters schema-embedded-mssql-vc11.vcxproj.filters schema-embedded-mysql-vc12.vcxproj schema-embedded-sqlite-vc12.vcxproj schema-embedded-pgsql-vc12.vcxproj schema-embedded-oracle-vc12.vcxproj schema-embedded-mssql-vc12.vcxproj schema-embedded-mysql-vc12.vcxproj.filters schema-embedded-sqlite-vc12.vcxproj.filters schema-embedded-pgsql-vc12.vcxproj.filters schema-embedded-oracle-vc12.vcxproj.filters schema-embedded-mssql-vc12.vcxproj.filters

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
	$(ODB) $(ODBCPPFLAGS) $(CPPFLAGS) $(ODBFLAGS) --database @database@ --generate-query --generate-schema --schema-format embedded --table-prefix schema_embedded_ $<
