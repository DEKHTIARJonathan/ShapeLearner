odb -I C:\tools\libodb-pgsql-2.4.0 -d pgsql --generate-query --generate-schema GraphClass.h
odb -I C:\tools\libodb-pgsql-2.4.0 -d pgsql --generate-query --generate-schema ObjectClass.h
odb -I C:\tools\libodb-pgsql-2.4.0 -d pgsql --generate-query --generate-schema Graph.h
odb -I C:\tools\libodb-pgsql-2.4.0 -d pgsql --generate-query --generate-schema Node.h
odb -I C:\tools\libodb-pgsql-2.4.0 -d pgsql --generate-query --generate-schema Edge.h
odb -I C:\tools\libodb-pgsql-2.4.0 -d pgsql --generate-query --generate-schema Point.h

del structure.sql
copy /b GraphClass.sql+ObjectClass.sql+Graph.sql+Node.sql+Edge.sql+Point.sql structure.sql

set /p DUMMY=Hit ENTER to continue...