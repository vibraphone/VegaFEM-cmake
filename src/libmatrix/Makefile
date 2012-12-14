ifndef MATRIX
MATRIX=MATRIX

ifndef CLEANFOLDER
CLEANFOLDER=MATRIX
endif

include ../../Makefile-headers/Makefile-header
R ?= ../..


# the object files to be compiled for this library
MATRIX_OBJECTS=matrixIO.o

# the libraries this library depends on
MATRIX_LIBS=

# the headers in this library
MATRIX_HEADERS=matrixIO.h matrixMacros.h

MATRIX_OBJECTS_FILENAMES=$(addprefix $(L)/matrix/, $(MATRIX_OBJECTS))
MATRIX_HEADER_FILENAMES=$(addprefix $(L)/matrix/, $(MATRIX_HEADERS))
MATRIX_LIB_MAKEFILES=$(call GET_LIB_MAKEFILES, $(MATRIX_LIBS))
MATRIX_LIB_FILENAMES=$(call GET_LIB_FILENAMES, $(MATRIX_LIBS))

include $(MATRIX_LIB_MAKEFILES)

all: $(L)/matrix/libmatrix.a

$(L)/matrix/libmatrix.a: $(MATRIX_OBJECTS_FILENAMES)
	ar r $@ $^; cp $@ $(L)/lib; cp $(L)/matrix/*.h $(L)/include

$(MATRIX_OBJECTS_FILENAMES): %.o: %.cpp $(MATRIX_LIB_FILENAMES) $(MATRIX_HEADER_FILENAMES)
	$(CXX) $(CXXFLAGS) -c $(INCLUDE) $< -o $@

ifeq ($(CLEANFOLDER), MATRIX)
clean: cleanmatrix
endif

deepclean: cleanmatrix

cleanmatrix:
	$(RM) $(MATRIX_OBJECTS_FILENAMES) $(L)/matrix/libmatrix.a

endif
