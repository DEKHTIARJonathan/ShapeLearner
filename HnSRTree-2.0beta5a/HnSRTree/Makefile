LIB_NAME = HnSRTree
APP_NAME = testHnSRTree
DAG_MATCHER_BASE = ../..

include $(DAG_MATCHER_BASE)/common.mk

CPPFLAGS = /nologo $(BASIC_CXXFLAGS) /I../include -D_CRT_SECURE_NO_DEPRECATE #/D_HNSRTIMP=_declspec(dllexport)
CFLAGS = $(CPPFLAGS)
CXXFLAGS = $(CPPFLAGS)

CPP_SRCS = \
		HnSRTreeStatsSt.cpp \
		HnSRTreeProfileSt.cpp \
        HnSRTreeBlock.cpp \
        HnSRTreeBlockObj.cpp \
        HnSRTreeCluster.cpp \
        HnSRTreeClusterObj.cpp \
        HnSRTreeFile.cpp \
        HnSRTreeFileObj.cpp \
        HnSRTreeFileObj1.cpp \
        HnSRTreeFileObj2.cpp \
        HnSRTreeFileObj3.cpp \
        HnSRTreeFileSt.cpp \
        HnSRTreeInfo.cpp \
        HnSRTreeInfoObj.cpp \
        HnSRTreeLeaf.cpp \
        HnSRTreeLeafObj.cpp \
        HnSRTreeNeighbor.cpp \
        HnSRTreeNeighborObj.cpp \
        HnSRTreeNode.cpp \
        HnSRTreeNodeObj.cpp \
        HnSRTreeReinsert.cpp \
        HnSRTreeReinsertObj.cpp \
        HnSRTreeStack.cpp \
        HnSRTreeStackObj.cpp
        
OBJECTS = $(CPP_SRCS:%.cpp=%.obj)
        
all: $(OBJECTS)
	
	
%.obj: %.cpp
	$(CXX) -c $(CXXFLAGS) $(INCS) $<

clean:
	$(RM) *.obj

