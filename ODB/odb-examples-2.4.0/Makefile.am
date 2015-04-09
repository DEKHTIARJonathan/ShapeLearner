# file      : Makefile.am
# copyright : Copyright (c) 2009-2015 Code Synthesis Tools CC
# license   : GNU GPL v2; see accompanying LICENSE file

SUBDIRS = access composite container hello inheritance/polymorphism inheritance/reuse query mapping optimistic pimpl prepared schema/embedded section

if HAVE_TR1_MEMORY
SUBDIRS += relationship inverse schema/custom view
endif

if HAVE_CXX11
SUBDIRS += c++11
endif

if ODB_EXAMPLES_BOOST
SUBDIRS += boost
endif

if ODB_EXAMPLES_QT
SUBDIRS += qt
endif

EXTRA_DIST = GPLv2 LICENSE README NEWS INSTALL version tester.bat mysql-driver.bat mysql.options sqlite-driver.bat sqlite.options pgsql-driver.bat pgsql.options oracle-driver.bat oracle.options mssql-driver.bat mssql.options bootstrap tester.in test.bat examples-mysql-vc8.sln examples-sqlite-vc8.sln examples-pgsql-vc8.sln examples-oracle-vc8.sln examples-mssql-vc8.sln examples-mysql-vc9.sln examples-sqlite-vc9.sln examples-pgsql-vc9.sln examples-oracle-vc9.sln examples-mssql-vc9.sln examples-mysql-vc10.sln examples-sqlite-vc10.sln examples-pgsql-vc10.sln examples-oracle-vc10.sln examples-mssql-vc10.sln examples-mysql-vc11.sln examples-sqlite-vc11.sln examples-pgsql-vc11.sln examples-oracle-vc11.sln examples-mssql-vc11.sln examples-mysql-vc12.sln examples-sqlite-vc12.sln examples-pgsql-vc12.sln examples-oracle-vc12.sln examples-mssql-vc12.sln
ACLOCAL_AMFLAGS = -I m4
