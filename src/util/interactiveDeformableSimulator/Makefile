ifndef SIMULATOR
SIMULATOR=SIMULATOR

ifndef CLEANFOLDER
CLEANFOLDER=SIMULATOR
endif

include ../../Makefile-headers/Makefile-header
R ?= ../..

# the object files to be compiled for this library
SIMULATOR_OBJECTS=interactiveDeformableSimulator.o initGraphics.o

# the libraries this library depends on
SIMULATOR_LIBS=sceneObject integrator elasticForceModel forceModel matrix sparseMatrix loadList insertRows lighting configFile volumetricMesh getopts camera graph isotropicHyperelasticFEM minivector stvk corotationalLinearFEM polarDecomposition massSpringSystem objMesh sparseSolver

# the headers in this library
SIMULATOR_HEADERS=initGraphics.h

SIMULATOR_LINK=$(addprefix -l, $(SIMULATOR_LIBS)) $(SPOOLES_LIB) $(PARDISO_LIB) $(GLUI_LIB) $(STANDARD_LIBS)

SIMULATOR_OBJECTS_FILENAMES=$(addprefix $(R)/utilities/interactiveDeformableSimulator/, $(SIMULATOR_OBJECTS))
SIMULATOR_HEADER_FILENAMES=$(addprefix $(R)/utilities/interactiveDeformableSimulator/, $(SIMULATOR_HEADERS))
SIMULATOR_LIB_MAKEFILES=$(call GET_LIB_MAKEFILES, $(SIMULATOR_LIBS))
SIMULATOR_LIB_FILENAMES=$(call GET_LIB_FILENAMES, $(SIMULATOR_LIBS))

include $(SIMULATOR_LIB_MAKEFILES) ../../libraries/glui/Makefile

all: $(R)/utilities/interactiveDeformableSimulator/interactiveDeformableSimulator

$(R)/utilities/interactiveDeformableSimulator/interactiveDeformableSimulator: $(SIMULATOR_OBJECTS_FILENAMES)
	$(CXXLD) $(LDFLAGS) $(SIMULATOR_OBJECTS) $(SIMULATOR_LINK) -Wl,-rpath,$(GLUI_DIR)/lib -o $@; cp $@ $(R)/utilities/bin/

$(SIMULATOR_OBJECTS_FILENAMES): %.o: %.cpp $(SIMULATOR_LIB_FILENAMES) $(SIMULATOR_HEADER_FILENAMES)
	$(CXX) $(CXXFLAGS) -c $(INCLUDE) $(GLUI_INCLUDE) $< -o $@

ifeq ($(CLEANFOLDER), SIMULATOR)
clean: cleaninteractiveDeformableSimulator
endif

deepclean: cleaninteractiveDeformableSimulator

cleaninteractiveDeformableSimulator:
	$(RM) $(SIMULATOR_OBJECTS_FILENAMES) $(R)/utilities/interactiveDeformableSimulator/interactiveDeformableSimulator

endif

