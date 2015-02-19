LIB_NAME = HnSRTree
APP_NAME = testHnSRTree
DAG_MATCHER_BASE = ..

include $(DAG_MATCHER_BASE)/common.mk


all:
	cd HnObject; $(MAKE) MAKEFLAGS="$(MAKEFLAGS)"
	cd HnRect; $(MAKE) MAKEFLAGS="$(MAKEFLAGS)"
	cd HnSRTree; $(MAKE) MAKEFLAGS="$(MAKEFLAGS)"
	cd lib; $(MAKE) MAKEFLAGS="$(MAKEFLAGS)"

clean:
	cd HnObject; $(MAKE) RM="$(RM)" clean
	cd HnRect; $(MAKE) RM="$(RM)" clean
	cd HnSRTree; $(MAKE) RM="$(RM)" clean
	cd lib; $(MAKE) RM="$(RM)" clean