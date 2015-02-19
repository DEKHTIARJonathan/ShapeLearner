# Makefile for the header files of the library
#---------------------------------------------------------------------------
#
# COMMON DEFINITIONS
#
include ../Common
#
# FILES TO BE TESTED
#
objects := $(patsubst %.w,%.o,$(wildcard *.w))
dirheaders := $(objects:.o=.hpp)
#objects := $(objects) $(patsubst %.w,%.o,$(wildcard barrier/*.w))
#objects := $(objects) $(patsubst %.w,%.o,$(wildcard function/*.w))
#objects := $(objects) $(patsubst %.w,%.o,$(wildcard functional/*.w))
#objects := $(objects) $(patsubst %.w,%.o,$(wildcard linesearch/*.w))
#objects := $(objects) $(patsubst %.w,%.o,$(wildcard searchdir/*.w))
#objects := $(objects) $(patsubst %.w,%.o,$(wildcard searchdir/equality/*.w))
sources := $(objects:.o=.cc)
headers := $(objects:.o=.hpp)
#
# COMPILATION RULES
#
all: src

lib: libsg.a

.PHONY: all lib clean makesrc

libsg.a: $(objects)
	@echo "Making libsg.a..."
	@$(RM) libsg.a
	@$(AR) cru libsg.a $(objects)
	@$(RANLIB) libsg.a

$(objects): %.o: %.cc

$(headers): %.hpp: %.w
	@echo "CTANGLE $<..."
	@$(CTANGLE) $< - ../src/$*.cc
	mv $*.hpp ../include

src: $(sources)

$(sources): %.cc: %.w
	@echo "CTANGLE $<..."
	@$(CTANGLE) -p $< - $@
	mv $*.hpp ../include
	cp $*.cc ../src


clean:
	@echo "Cleaning web directory..."
	@$(RM) *.a *~ */*~ $(headers) *.ch $(sources) $(objects)
	@$(RM) *.log *.idx *.scn *.toc *.tex *.dvi *.ps
	@$(MAKE) -s -C ../include clean
	@$(MAKE) -s -C ../src tidy
#	@$(MAKE) -s -C searchdir clean

install: lib
	@echo "Installing libsg.a in $(libdir)..."
	@install libsg.a $(libdir)
	@echo "Installing headers in $(includedir)/sg..."
	@install -d $(includedir)/sg
	@$(foreach x,$(dirheaders),\
	sed -e /^$$/d -e /^#line/d -e /^\\//d < $(x) > $(includedir)/sg/$(x);)
	@$(MAKE) -s -C private install
	@$(MAKE) -s -C functional install
	@$(MAKE) -s -C function install
	@$(MAKE) -s -C linesearch install
	@$(MAKE) -s -C searchdir install
	@$(MAKE) -s -C barrier install

uninstall:
	@$(MAKE) -s -C barrier uninstall
	@$(MAKE) -s -C searchdir uninstall
	@$(MAKE) -s -C linesearch uninstall
	@$(MAKE) -s -C function uninstall
	@$(MAKE) -s -C functional uninstall
	@$(MAKE) -s -C private uninstall
	@echo "Uninstalling $(includedir)/sg...";
	@$(RM) -rf $(includedir)/sg/*.h
	@echo "Uninstalling $(libdir)/libsg.a...";
	@$(RM) $(libdir)/libsg.a

%:
	$(MAKE) -C .. $@


#
# Dependencies
#
