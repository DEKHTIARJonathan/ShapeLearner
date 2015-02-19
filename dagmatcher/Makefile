# Makefile for the DAG Matcher Library (DML)

LIB_NAME = DAGMatch
APP_NAME = testDML
DAG_MATCHER_BASE = ..

include $(DAG_MATCHER_BASE)/common.mk

# Set INCLUDES and LIBRARIES flags

INCS = -I$(DAG_MATCHER_BASE) \
       -I$(NEWMAT_ROOT) \
       -I$(LEDAROOT)/incl/ \
       -I$(FLUX_SKELETON_ROOT)/include \
       -I$(HNSRTREE_ROOT)/include \
       -I$(AFMM_SKELETON_ROOT)/include \
       -I$(GLUT_ROOT)
       
# Collect all files

CPP_SRCS = $(wildcard *.cpp)
HEADERS = $(wildcard *.h)

OBJECTS = $(CPP_SRCS:%.cpp=$(OBJFILE))

# Set up the file dependecies such that any change on a header file implies
# a full compilation (this is slow but safe)

$(LIB_NAME): $(OBJECTS)
	$(AR) $(ARFLAGS) $(OBJECTS)

$(OBJFILE): %.cpp $(HEADERS)
	$(CXX) -c $(CXXFLAGS) $(INCS) $<

clean:
	$(RM) $(OBJECTS) $(LIB_FULL_NAME) $(APP_NAME) vc80.pdb
