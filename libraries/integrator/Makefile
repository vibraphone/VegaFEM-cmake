ifndef INTEGRATOR
INTEGRATOR=INTEGRATOR

ifndef CLEANFOLDER
CLEANFOLDER=INTEGRATOR
endif

include ../../Makefile-headers/Makefile-header
R ?= ../..

# NOTE: unlike the other library makefiles, this adds
# $(PARDISO_INCLUDE) to the compiler invocation

# the object files to be compiled for this library
INTEGRATOROBJECTS=centralDifferencesSparse.o eulerSparse.o implicitBackwardEulerSparse.o implicitNewmarkSparse.o integratorBase.o integratorBaseSparse.o getIntegratorSolver.o

# the libraries this library depends on
INTEGRATORLIBS=matrix performanceCounter insertRows sparseSolver forceModel

# the headers in this library
INTEGRATORHEADERS=centralDifferencesSparse.h eulerSparse.h implicitBackwardEulerSparse.h implicitNewmarkSparse.h integratorBase.h integratorBaseSparse.h getIntegratorSolver.h integrators.h integratorSolverSelection.h 

INTEGRATOROBJECTS_FILENAMES=$(addprefix $(L)/integrator/, $(INTEGRATOROBJECTS))
INTEGRATORHEADER_FILENAMES=$(addprefix $(L)/integrator/, $(INTEGRATORHEADERS))
INTEGRATORLIB_MAKEFILES=$(call GET_LIB_MAKEFILES, $(INTEGRATORLIBS))
INTEGRATORLIB_FILENAMES=$(call GET_LIB_FILENAMES, $(INTEGRATORLIBS))

include $(INTEGRATORLIB_MAKEFILES)

all: $(L)/integrator/libintegrator.a

$(L)/integrator/libintegrator.a: $(INTEGRATOROBJECTS_FILENAMES)
	ar r $@ $^; cp $@ $(L)/lib; cp $(L)/integrator/*.h $(L)/include

$(INTEGRATOROBJECTS_FILENAMES): %.o: %.cpp $(INTEGRATORLIB_FILENAMES) $(INTEGRATORHEADER_FILENAMES)
	$(CXX) $(CXXFLAGS) -c $(INCLUDE) $(PARDISO_INCLUDE) $< -o $@

ifeq ($(CLEANFOLDER), INTEGRATOR)
clean: cleanintegrator
endif

deepclean: cleanintegrator

cleanintegrator:
	$(RM) $(INTEGRATOROBJECTS_FILENAMES) $(L)/integrator/libintegrator.a

endif

