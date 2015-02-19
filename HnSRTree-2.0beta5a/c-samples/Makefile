CFLAGS = /nologo $(BASIC_CXXFLAGS) /I../include -D_CRT_SECURE_NO_DEPRECATE

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

checkIndex.exe: checkIndex.c RecordFileSt.obj
	cl $(CFLAGS) checkIndex.c \
	   RecordFileSt.obj ../lib/HnSRTree.lib
dynamicBuild.exe: dynamicBuild.c RecordFileSt.obj
	cl $(CFLAGS) dynamicBuild.c \
	   RecordFileSt.obj ../lib/HnSRTree.lib
fetchColoredNN.exe: fetchColoredNN.c RecordFileSt.obj
	cl $(CFLAGS) fetchColoredNN.c \
	   RecordFileSt.obj ../lib/HnSRTree.lib
fetchInRect.exe: fetchInRect.c RecordFileSt.obj
	cl $(CFLAGS) fetchInRect.c \
	   RecordFileSt.obj ../lib/HnSRTree.lib
fetchNN.exe: fetchNN.c RecordFileSt.obj
	cl $(CFLAGS) fetchNN.c \
	   RecordFileSt.obj ../lib/HnSRTree.lib
printIndex.exe: printIndex.c RecordFileSt.obj
	cl $(CFLAGS) printIndex.c \
	   RecordFileSt.obj ../lib/HnSRTree.lib
removeRecords.exe: removeRecords.c RecordFileSt.obj
	cl $(CFLAGS) removeRecords.c \
	   RecordFileSt.obj ../lib/HnSRTree.lib
staticBuild.exe: staticBuild.c RecordFileSt.obj
	cl $(CFLAGS) staticBuild.c \
	   RecordFileSt.obj ../lib/HnSRTree.lib
storeRecords.exe: storeRecords.c RecordFileSt.obj
	cl $(CFLAGS) storeRecords.c \
	   RecordFileSt.obj ../lib/HnSRTree.lib

clean:
	del *.obj
	del *.exe
