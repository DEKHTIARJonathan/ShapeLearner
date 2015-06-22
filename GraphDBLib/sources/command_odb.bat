

::--------------------------------------------------------
::-- ODB Compilation
::--------------------------------------------------------
odb -I C:\ODB\libodb-pgsql-2.4.0 -I C:\boost_1_57_0 -I "C:\Users\dekhtiar\Desktop\Git Projects\ShapeLearner\ShapeLearnerProject\include" --database pgsql --generate-query --generate-prepared --generate-schema GraphClass.h
odb -I C:\ODB\libodb-pgsql-2.4.0 -I C:\boost_1_57_0 -I "C:\Users\dekhtiar\Desktop\Git Projects\ShapeLearner\ShapeLearnerProject\include" --database pgsql --generate-query --generate-prepared --generate-schema ObjectClass.h
odb -I C:\ODB\libodb-pgsql-2.4.0 -I C:\boost_1_57_0 -I "C:\Users\dekhtiar\Desktop\Git Projects\ShapeLearner\ShapeLearnerProject\include" --database pgsql --generate-query --generate-prepared --generate-schema Graph.h
odb -I C:\ODB\libodb-pgsql-2.4.0 -I C:\boost_1_57_0 -I "C:\Users\dekhtiar\Desktop\Git Projects\ShapeLearner\ShapeLearnerProject\include" --database pgsql --generate-query --generate-prepared --generate-schema Node.h
odb -I C:\ODB\libodb-pgsql-2.4.0 -I C:\boost_1_57_0 -I "C:\Users\dekhtiar\Desktop\Git Projects\ShapeLearner\ShapeLearnerProject\include" --database pgsql --generate-query --generate-prepared --generate-schema Edge.h
odb -I C:\ODB\libodb-pgsql-2.4.0 -I C:\boost_1_57_0 -I "C:\Users\dekhtiar\Desktop\Git Projects\ShapeLearner\ShapeLearnerProject\include" --database pgsql --generate-query --generate-prepared --generate-schema Point.h


call:insertPrecompiledHeaders Edge-odb.cxx
call:insertPrecompiledHeaders GraphClass-odb.cxx
call:insertPrecompiledHeaders Graph-odb.cxx
call:insertPrecompiledHeaders Node-odb.cxx
call:insertPrecompiledHeaders ObjectClass-odb.cxx
call:insertPrecompiledHeaders Point-odb.cxx



IF EXIST "..\..\ShapeLearnerUser\sources\structure.sql" (
	del "..\..\ShapeLearnerUser\sources\structure.sql"
)
copy GraphClass.sql /b + ObjectClass.sql /b + Graph.sql /b + Node.sql /b + Edge.sql /b + Point.sql /b + functions.sql /b + viewLearningData.sql /b + foreignTables.sql /b "..\..\ShapeLearnerUser\sources\structure.sql" /b


copy "..\..\ShapeLearnerUser\sources\structure.sql" "..\..\ShapeLearnerDLL\Server\Release\structure.sql"
copy "..\..\ShapeLearnerUser\sources\structure.sql" "..\..\ShapeLearnerDLL\Server\Debug\structure.sql"

set /p DUMMY=Hit ENTER to continue...



::--------------------------------------------------------
::-- Function section starts below here
::--------------------------------------------------------

:insertPrecompiledHeaders
copy %~1 temp.cxx
echo.#include "stdafx.h">%~1
type temp.cxx >>%~1
del temp.cxx

goto:eof