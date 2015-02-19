CPPFLAGS = /nologo $(BASIC_CXXFLAGS) /I../include -D_CRT_SECURE_NO_DEPRECATE

all: \
        checkIndex.exe \
        dynamicBuild.exe \
        fetchColoredNN.exe \
        fetchInRect.exe \
        fetchNN.exe \
        printIndex.exe \
        removeRecords.exe \
        staticBuild.exe \
        storeRecords.exe

checkIndex.exe: checkIndex.cpp RecordFileSt.obj
	cl $(CPPFLAGS) checkIndex.cpp \
	   RecordFileSt.obj ../lib/HnSRTree.lib
dynamicBuild.exe: dynamicBuild.cpp RecordFileSt.obj
	cl $(CPPFLAGS) dynamicBuild.cpp \
	   RecordFileSt.obj ../lib/HnSRTree.lib
fetchColoredNN.exe: fetchColoredNN.cpp RecordFileSt.obj
	cl $(CPPFLAGS) fetchColoredNN.cpp \
	   RecordFileSt.obj ../lib/HnSRTree.lib
fetchInRect.exe: fetchInRect.cpp RecordFileSt.obj
	cl $(CPPFLAGS) fetchInRect.cpp \
	   RecordFileSt.obj ../lib/HnSRTree.lib
fetchNN.exe: fetchNN.cpp RecordFileSt.obj
	cl $(CPPFLAGS) fetchNN.cpp \
	   RecordFileSt.obj ../lib/HnSRTree.lib
printIndex.exe: printIndex.cpp RecordFileSt.obj
	cl $(CPPFLAGS) printIndex.cpp \
	   RecordFileSt.obj ../lib/HnSRTree.lib
removeRecords.exe: removeRecords.cpp RecordFileSt.obj
	cl $(CPPFLAGS) removeRecords.cpp \
	   RecordFileSt.obj ../lib/HnSRTree.lib
staticBuild.exe: staticBuild.cpp RecordFileSt.obj
	cl $(CPPFLAGS) staticBuild.cpp \
	   RecordFileSt.obj ../lib/HnSRTree.lib
storeRecords.exe: storeRecords.cpp RecordFileSt.obj
	cl $(CPPFLAGS) storeRecords.cpp \
	   RecordFileSt.obj ../lib/HnSRTree.lib

clean:
	del *.obj
	del *.exe
