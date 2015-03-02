odb -I C:\tools\libodb-pgsql-2.4.0 -d pgsql --generate-query --generate-schema GraphClass.h
odb -I C:\tools\libodb-pgsql-2.4.0 -d pgsql --generate-query --generate-schema ObjectClass.h
odb -I C:\tools\libodb-pgsql-2.4.0 -d pgsql --generate-query --generate-schema Graph.h

del structure.sql
copy *.sql structure.sql /b

set /p DUMMY=Hit ENTER to continue...