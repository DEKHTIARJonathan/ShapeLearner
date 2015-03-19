odb -I C:\tools\libodb-pgsql-2.4.0 -I C:\boost_1_57_0 -d pgsql --generate-query --generate-prepared --generate-schema GraphClass.h
odb -I C:\tools\libodb-pgsql-2.4.0 -I C:\boost_1_57_0 -d pgsql --generate-query --generate-prepared --generate-schema ObjectClass.h
odb -I C:\tools\libodb-pgsql-2.4.0 -I C:\boost_1_57_0 -d pgsql --generate-query --generate-prepared --generate-schema Graph.h
odb -I C:\tools\libodb-pgsql-2.4.0 -I C:\boost_1_57_0 -d pgsql --generate-query --generate-prepared --generate-schema Node.h
odb -I C:\tools\libodb-pgsql-2.4.0 -I C:\boost_1_57_0 -d pgsql --generate-query --generate-prepared --generate-schema Edge.h
odb -I C:\tools\libodb-pgsql-2.4.0 -I C:\boost_1_57_0 -d pgsql --generate-query --generate-prepared --generate-schema Point.h

del "../../ShapeLearnerUser/sources/structure.sql"
copy /b GraphClass.sql+ObjectClass.sql+Graph.sql+Node.sql+Edge.sql+Point.sql+functions.sql "../../ShapeLearnerUser/sources/structure.sql"

set /p DUMMY=Hit ENTER to continue...