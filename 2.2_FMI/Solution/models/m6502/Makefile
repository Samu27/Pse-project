# -----------------------------------------------------------------------------
# VARIABLES
# -----------------------------------------------------------------------------
NAME="m6502"
FOLDER=verilog
DESIGN=*.v
FRONT_END_TOOL=verilog2hif
SILENCED=>> manipulation.log 2>&1
INCDIR=-I./cpp/inc -I${FMI_HEADERS}
CFLAGS = -fPIC -lstdc++
# -----------------------------------------------------------------------------
# TARGET
# -----------------------------------------------------------------------------
all: fmu_packaging fmu_compilation fmu_generation
	

# -----------------------------------------------------------------------------
# FMU 
# -----------------------------------------------------------------------------

fmu_packaging: fmu_compilation
	@echo "3) FMU PACKAGING:"
	@cp cpp/lib/${NAME}.so fmu/binaries/linux64/ 
	@cd fmu; zip -r -D ../${NAME}.fmu  *; cd ..

fmu_compilation: fmu_generation
	@echo "2) FMU COMPILATION:"
	@mkdir -p cpp/obj cpp/lib
	g++ -c ${CFLAGS} ${INCDIR} cpp/src/${NAME}.cpp -o cpp/obj/${NAME}.o
	g++ -c ${CFLAGS} ${INCDIR} cpp/src/fmuInterface/fmuInterface_dataTypes.cpp -o cpp/obj/fmuInterface.o 
	g++  -shared cpp/obj/*.o  -o cpp/lib/${NAME}.so

fmu_generation:
	@echo "1) FMU GENERATION:"
	@mkdir -p fmu/binaries/linux64
	@make --no-print-directory hif/design.hif.xml
	@make --no-print-directory hif/design.ddt.hif.xml
	@make --no-print-directory hif/design.ddt.a2t.hif.xml
	@make --no-print-directory hif/design.ddt.a2t.fmi.hif.xml
	@make --no-print-directory cpp
	

# -----------------------------------------------------------------------------
# HIF  
# -----------------------------------------------------------------------------

hif/design.hif.xml:
	@echo "		Running front end.................[Verilog->HIF]"
	@${FRONT_END_TOOL} ${FOLDER}/${DESIGN} -o hif/design ${SILENCED}
# -----------------------------------------------------------------------------
# HIF TOOLCHAIN
# -----------------------------------------------------------------------------
hif/design.ddt.hif.xml:
	@echo "		Running DataType Abstraction......[DDT]"
	@ddt hif/design.hif.xml -o hif/design.ddt.hif.xml  ${SILENCED}

hif/design.ddt.a2t.hif.xml:
	@echo "		Running Protocol Abstraction......[A2TOOL-CPP]"
	@a2tool hif/design.ddt.hif.xml -o hif/design.ddt.a2t.hif.xml --protocol CPP ${SILENCED}

hif/design.ddt.a2t.fmi.hif.xml:
	@echo "		Running FMI Wrapping..............[HIF2VP-FMI]"
	@hif2vp hif/design.ddt.a2t.hif.xml -o hif/design.ddt.a2t.fmi.hif.xml -p fmi ${SILENCED}
	@mv modelDescription.xml fmu/

cpp:
	@echo "		Running Back-End code generator...[HIF2SC-CPP]"
	@hif2sc hif/design.ddt.a2t.fmi.hif.xml --directory cpp ${SILENCED} --extensions cpp_hpp
# -----------------------------------------------------------------------------
# CLEAN
# -----------------------------------------------------------------------------
clean:
	@echo "        Cleaning ${NAME}..."
	@rm -rf hif
	@rm -rf cpp
	@rm -rf fmu
	@rm ${NAME}.fmu
	@rm -rf manipulation.log
