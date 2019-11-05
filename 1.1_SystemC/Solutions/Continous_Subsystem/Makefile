SYSTEMC=$$SYSTEMC_HOME
SYSTEMC_AMS=$$SCAMS_HOME

BASE_COMPONENTS=.

MAIN=main_AMS
VALVE=Valve_AMS_TDF
WATERTANK=Watertank_AMS_LSF
CONTROLLER=Controller_AMS_TDF

PLATFORM= $(shell uname -s)
ARCH=$(shell getconf LONG_BIT)

ifeq ($(PLATFORM),Darwin)
SYSC_LIBDIR := $(SYSTEMC)/lib-macosx
SCAMS_LIBDIR := $(SYSTEMC_AMS)/lib-macosx
endif
ifeq ($(PLATFORM),Linux)
SYSC_LIBDIR := $(SYSTEMC)/lib-linux
SCAMS_LIBDIR := $(SYSTEMC_AMS)/lib-linux
endif
ifeq ($(ARCH),64)
SYSC_LIBDIR := $(SYSC_LIBDIR)64
SCAMS_LIBDIR := $(SCAMS_LIBDIR)64
endif

LIBS = $(SCAMS_LIBDIR)/libsystemc-ams.a \
	   $(SYSC_LIBDIR)/libsystemc.a \
	   -lpthread

INCDIR = -I./include/ -I. -I$(SYSTEMC)/include/ -I$(SYSTEMC_AMS)/include

CXX=g++
CFLAGS= -c
LD=g++
EXECUTABLE=./bin/Continous_Subsystem.x


all: $(EXECUTABLE)

$(EXECUTABLE): obj/main.o obj/controller.o obj/valve.o obj/water.o
	@echo Building $@...
	@ $(LD) -o $@ $(INCDIR) $^ $(LIBS)
	@echo Done

obj/main.o: src/$(MAIN).cc
	@echo Building $@...
	@ $(CXX) $(CFLAGS) -o $@ $(INCDIR) $<

obj/valve.o: src/$(VALVE).cc include/$(VALVE).hh
	@echo Building $@...
	@ $(CXX) $(CFLAGS) -o $@ $(INCDIR) $<

obj/controller.o: src/$(CONTROLLER).cc include/$(CONTROLLER).hh
	@echo Building $@...
	@ $(CXX) $(CFLAGS) -o $@ $(INCDIR) $<

obj/water.o: src/$(WATERTANK).cc include/$(WATERTANK).hh
	@echo Building $@...
	@ $(CXX) $(CFLAGS) -o $@ $(INCDIR) $<



clean:
	@echo Cleaning...
	@rm -f obj/*.o *~
	@rm -f $(EXECUTABLE)
	@echo Done


.PHONY: all clean	
