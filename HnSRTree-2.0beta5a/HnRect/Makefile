LIB_NAME = HnSRTree
APP_NAME = testHnSRTree
DAG_MATCHER_BASE = ../..

include $(DAG_MATCHER_BASE)/common.mk

CPPFLAGS = /nologo $(BASIC_CXXFLAGS) /I../include -D_CRT_SECURE_NO_DEPRECATE #/D_HNSRTIMP=_declspec(dllexport)
CFLAGS = $(CPPFLAGS)
CXXFLAGS = $(CPPFLAGS)

CPP_SRCS = \
		HnRectSt.cpp \
		HnSphereSt.cpp \
		HnPointSt.cpp \
		HnRangeSt.cpp \
        HnPoint.cpp \
        HnPointObj.cpp \
        HnRange.cpp \
        HnRect.cpp \
        HnRectObj.cpp \
        HnSphere.cpp \
        HnSphereObj.cpp

OBJECTS = $(CPP_SRCS:%.cpp=%.obj)
        
all: $(OBJECTS)
	
	
%.obj: %.cpp
	$(CXX) -c $(CXXFLAGS) $(INCS) $<

clean:
	$(RM) *.obj
