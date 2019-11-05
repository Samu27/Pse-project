SYSTEMC=$$SYSTEMC_HOME
SYSTEMC_AMS=$$SCAMS_HOME


EXEC = bin/Heterogeneous_Platform.x
MAIN = main
CONTRL = Controller_TLM
VALVE = Valve
WTANK = Watertank
WT_SYS = Watertank_system
XTEA = Xtea

RTL = RTL
TLM = TLM
AMS = AMS
TRANS = Transactor

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

LIBS = $(SCAMS_LIBDIR)/libsystemc-ams.a   $(SYSC_LIBDIR)/libsystemc.a -lpthread

INCDIR = -I./include/ -I. -I$(SYSTEMC)/include/ -I$(SYSTEMC_AMS)/include


CXX=g++
CFLAGS= -c
LD=g++


all: $(EXEC)

$(EXEC): obj/$(MAIN).o obj/$(WT_SYS).o obj/$(VALVE).o obj/$(WTANK).o \
	obj/$(CONTRL).o obj/$(XTEA).o \
	obj/$(TRANS)_$(AMS)_$(RTL).o  obj/$(TRANS)_$(RTL)_$(TLM).o \
	obj/$(TRANS)_$(TLM)_$(RTL).o obj/$(TRANS)_$(RTL)_$(AMS).o 

	@echo Building $@...
	@ $(LD) -o $@ $(INCDIR) $^ $(LIBS)

# Main
obj/$(MAIN).o: src/$(MAIN).cc
	@echo Building $@...
	@ $(CXX) $(CFLAGS) -o $@ $(INCDIR) $<

# Testbench
obj/$(WT_SYS).o: src/$(WT_SYS).cc include/$(WT_SYS).hh
	@echo Building $@...
	@ $(CXX) $(CFLAGS) -o $@ $(INCDIR) $<

# Controller
obj/$(CONTRL).o: src/$(CONTRL).cc include/$(CONTRL).hh
	@echo Building $@...
	@ $(CXX) $(CFLAGS) -o $@ $(INCDIR) $<

# Tlm - Rtl
obj/$(TRANS)_$(TLM)_$(RTL).o: src/$(TRANS)_$(TLM)_$(RTL).cc include/$(TRANS)_$(TLM)_$(RTL).hh
	@echo Building $@...
	@ $(CXX) $(CFLAGS) -o $@ $(INCDIR) $<

# Xtea
obj/$(XTEA).o: src/$(XTEA).cc include/$(XTEA).hh
	@echo Building $@...
	@ $(CXX) $(CFLAGS) -o $@ $(INCDIR) $<

# Rtl - Ams
obj/$(TRANS)_$(RTL)_$(AMS).o: src/$(TRANS)_$(RTL)_$(AMS).cc  include/$(TRANS)_$(RTL)_$(AMS).hh
	@echo Building $@...
	@ $(CXX) $(CFLAGS) -o $@ $(INCDIR) $<

# Valve
obj/$(VALVE).o: src/$(VALVE).cc include/$(VALVE).hh 
	@echo Building $@...
	@ $(CXX) $(CFLAGS) -o $@ $(INCDIR) $<

# Watertank
obj/$(WTANK).o: src/$(WTANK).cc include/$(WTANK).hh
	@echo Building $@...
	@ $(CXX) $(CFLAGS) -o $@ $(INCDIR) $<

# Ams - Rtl
obj/$(TRANS)_$(AMS)_$(RTL).o: src/$(TRANS)_$(AMS)_$(RTL).cc include/$(TRANS)_$(AMS)_$(RTL).hh
	@echo Building $@...
	@ $(CXX) $(CFLAGS) -o $@ $(INCDIR) $<

# Rtl - Tlm
obj/$(TRANS)_$(RTL)_$(TLM).o: src/$(TRANS)_$(RTL)_$(TLM).cc include/$(TRANS)_$(RTL)_$(TLM).hh
	@echo Building $@...
	@ $(CXX) $(CFLAGS) -o $@ $(INCDIR) $<


clean:
	@echo Cleaning...
	@rm -f $(EXEC)
	@rm -f obj/*.o *~
	@echo Done


.PHONY: all clean	
