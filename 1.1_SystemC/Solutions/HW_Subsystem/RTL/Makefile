DESIGN_NAME=Xtea
ABSTRACTION_LEVEL=RTL
EXECUTABLE=bin/$(DESIGN_NAME)_$(ABSTRACTION_LEVEL).x

SYSTEMC=$$SYSTEMC_HOME
PLATFORM= $(shell uname -s)
ARCH=$(shell getconf LONG_BIT)

ifeq ($(PLATFORM),Darwin)
SYSC_LIBDIR := $(SYSTEMC)/lib-macosx
endif
ifeq ($(PLATFORM),Linux)
SYSC_LIBDIR := $(SYSTEMC)/lib-linux
endif
ifeq ($(ARCH),64)
SYSC_LIBDIR := $(SYSC_LIBDIR)64
endif

INCDIR = -I./include/ -I. -I$(SYSTEMC)/include/

LIBDIR = $(SYSC_LIBDIR)

CXX=g++
CFLAGS= -c
LD=g++


all: $(EXECUTABLE)

$(EXECUTABLE): obj/main_$(DESIGN_NAME).o obj/$(DESIGN_NAME)_testbench.o obj/$(DESIGN_NAME).o
	@echo Building $@ ...
	@$(CXX) -g $(INCDIR) -o $@ $^ $(LIBDIR)/libsystemc.a -lpthread
	@echo Done

obj/main_$(DESIGN_NAME).o: src/main_$(DESIGN_NAME).cc
	@echo Building $@ ...
	@$(CXX) -g $(CFLAGS) $(INCDIR) -o $@ $< 

obj/$(DESIGN_NAME)_testbench.o: src/$(DESIGN_NAME)_testbench.cc include/$(DESIGN_NAME)_testbench.hh
	@echo Building $@ ...
	@$(CXX) -g $(CFLAGS) $(INCDIR) -o $@ $< 

obj/$(DESIGN_NAME).o: src/$(DESIGN_NAME).cc include/$(DESIGN_NAME).hh
	@echo Building $@ ...
	@$(CXX) -g $(CFLAGS) $(INCDIR) -o $@ $<


clean:
	@echo Cleaning...
	@rm -f $(EXECUTABLE)
	@rm -f obj/*.o *~
	@echo Done


.PHONY: all clean
