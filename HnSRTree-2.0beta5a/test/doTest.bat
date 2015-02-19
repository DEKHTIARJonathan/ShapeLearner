@echo off

set commandDir=..\c-samples

set dynamicBuild=%commandDir%\dynamicBuild
set staticBuild=%commandDir%\staticBuild
set storeRecords=%commandDir%\storeRecords
set removeRecords=%commandDir%\removeRecords
set fetchInRect=%commandDir%\fetchInRect
set fetchNN=%commandDir%\fetchNN
set fetchColoredNN=%commandDir%\fetchColoredNN
set checkIndex=%commandDir%\checkIndex
set printIndex=%commandDir%\printIndex

set attrSize=512

echo Building an SR-tree dynamically for 1000 points ... 
%dynamicBuild% -s %attrSize% sample.rcd sample.idx > dynamicBuild.log
if errorlevel 1 goto failed
echo done

echo Checking the built SR-tree ... 
%checkIndex% sample.idx
if errorlevel 1 goto failed
echo done

echo Verifying the built SR-tree ... 
%fetchInRect% -v sample.rcd sample.idx > fetchInRect.log
if errorlevel 1 goto failed
echo done

echo Removing all points from the SR-tree ... 
%removeRecords% sample.rcd sample.idx > removeRecords.log
if errorlevel 1 goto failed
echo done

echo Checking the modified SR-tree ... 
%checkIndex% sample.idx
if errorlevel 1 goto failed
echo done

echo Verifying the modified SR-tree ... 
%printIndex% sample.idx | find "Number of leaf entries: 0" > nul:
if errorlevel 1 goto failed
echo done

echo Restoring 1000 points to the SR-tree ... 
%storeRecords% sample.rcd sample.idx > storeRecords.log
if errorlevel 1 goto failed
echo done

echo Checking the restored SR-tree ... 
%checkIndex% sample.idx
if errorlevel 1 goto failed
echo done

echo Verifying the restored SR-tree ... 
%fetchInRect% -v sample.rcd sample.idx > fetchInRect.log
if errorlevel 1 goto failed
echo done

echo Running nearest neighbor searches ... 
%fetchNN% -v -n 4 sample.rcd sample.idx > fetchNN.log
if errorlevel 1 goto failed
echo done

echo Verifying the result of the nearest neighbor searches ... 
extans < fetchNN.log > neighbor.tmp
cmp neighbor.tmp neighbor.ans > nul:
if errorlevel 1 goto failed
echo done

echo Running range searches ... 
%fetchInRect% -v -w 0.1 sample.rcd sample.idx > fetchInRect.log
if errorlevel 1 goto failed
echo done

echo Verifying the result of the range searches ... 
extans < fetchInRect.log > range.tmp
cmp range.tmp range.ans > nul:
if errorlevel 1 goto failed
echo done

echo Building an SR-tree statically for 1000 points ... 
%staticBuild% -s %attrSize% sample.rcd sample.idx > staticBuild.log
if errorlevel 1 goto failed
echo done

echo Checking the built SR-tree ... 
%checkIndex% sample.idx
if errorlevel 1 goto failed
echo done

echo Verifying the built SR-tree ... 
%fetchInRect% -v sample.rcd sample.idx > fetchInRect.log
if errorlevel 1 goto failed
echo done

echo Removing all points from the SR-tree ... 
%removeRecords% sample.rcd sample.idx > removeRecords.log
if errorlevel 1 goto failed
echo done

echo Checking the modified SR-tree ... 
%checkIndex% sample.idx
if errorlevel 1 goto failed
echo done

echo Verifying the modified SR-tree ... 
%printIndex% sample.idx | find "Number of leaf entries: 0" > nul:
if errorlevel 1 goto failed
echo done

echo Building an SR-tree statically for 1280 colored points ... 
%staticBuild% -s %attrSize% colored.rcd colored.idx > staticBuild.log
if errorlevel 1 goto failed
echo done

echo Running colored nearest neighbor searches ... 
%fetchColoredNN% -v -n 4 colored.rcd colored.idx > fetchColoredNN.log
if errorlevel 1 goto failed
echo done

echo Verifying the result of the colored nearest neighbor searches ... 
extans < fetchColoredNN.log > colored.tmp
cmp colored.tmp colored.ans > nul:
if errorlevel 1 goto failed
echo done

echo Congratulations! All tests succeeded.

goto exit 

:failed 
echo ******** Test failed. ******** 

:exit
