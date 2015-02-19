LIB_NAME = HnSRTree
APP_NAME = testHnSRTree
DAG_MATCHER_BASE = ../..

include $(DAG_MATCHER_BASE)/common.mk

CPPFLAGS = /nologo $(BASIC_CXXFLAGS) /I../include -D_CRT_SECURE_NO_DEPRECATE #/D_HNSRTIMP=_declspec(dllexport)
CFLAGS = $(CPPFLAGS)
CXXFLAGS = $(CPPFLAGS)

CPP_SRCS = \
        HnAbort.cpp \
        HnStringBufferSt.cpp \
        HnStringSt.cpp \
        HnTimesSt.cpp \
        HnTimesStatsSt.cpp \
        HnPropertiesSt.cpp \
        HnStatisticsSt.cpp \
        HnGetOpt.cpp \
        HnMalloc.cpp \
        HnDataItemSt.cpp \
        HnBinarySearch.cpp \
        HnBlockFile.cpp \
        HnBlockFileObj.cpp \
        HnBlockStream.cpp \
        HnBlockStreamObj.cpp \
        HnDataItem.cpp \
        HnDataItemObj.cpp \
        HnFTcharArray.cpp \
        HnFTdoubleArray.cpp \
        HnFTfloatArray.cpp \
        HnFTintArray.cpp \
        HnFTlongArray.cpp \
        HnFTucharArray.cpp \
        HnFTuintArray.cpp \
        HnFTulongArray.cpp \
        HnGenericArray.cpp \
        HnObject.cpp \
        HnPointer.cpp \
        HnProperties.cpp \
        HnPropertiesObj.cpp \
        HnQuickSelect.cpp \
        HnQuickSort.cpp \
        HnString.cpp \
        HnStatistics.cpp \
        HnStatisticsObj.cpp \
        HnStringBuffer.cpp \
        HnStringBufferObj.cpp \
        HnStringObj.cpp \
        HnTimes.cpp \
        HnTimesObj.cpp
    
OBJECTS = $(CPP_SRCS:%.cpp=%.obj)
        
all: $(OBJECTS)
	
	
%.obj: %.cpp
	$(CXX) -c $(CXXFLAGS) $(INCS) $<

clean:
	$(RM) *.obj
