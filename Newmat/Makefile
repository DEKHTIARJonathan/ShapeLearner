# Make file for the Newmat Library

LIB_NAME = newmat
APP_NAME = testNewmat
DAG_MATCHER_BASE = ..

include $(DAG_MATCHER_BASE)/common.mk

ifeq "$(ARCH)" "Windows"
	MKFILE = nm_m6.mak
	ARCH_MAKE = nmake
else
	MKFILE = nm_gnu.mak
	ARCH_MAKE = make
endif

all:
	$(ARCH_MAKE) -f $(MKFILE)
	
clean:
	$(ARCH_MAKE) -f $(MKFILE) clean