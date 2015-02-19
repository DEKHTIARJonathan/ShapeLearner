CFLAGS = /nologo $(BASIC_CXXFLAGS) /I../include -D_CRT_SECURE_NO_DEPRECATE

all: extans.exe cmp.exe HnSRTree.dll

libHnSRTree.dll: ..\lib\HnSRTree.dll
	copy ..\lib\HnSRTree.dll .

clean:
	del *.obj
	del *.exe
	del HnSRTree.dll
	del *.tmp
	del *.idx
	del *.log
