ifndef OBJMESH
OBJMESH=OBJMESH

ifndef CLEANFOLDER
CLEANFOLDER=OBJMESH
endif

include ../../Makefile-headers/Makefile-header
R ?= ../..


# the object files to be compiled for this library
OBJMESH_OBJMESHECTS=objMeshBinaryLoader.o objMesh.o objMeshRender.o objMeshEncode.o

# the libraries this library depends on
OBJMESH_LIBS=minivector matrix

# the headers in this library
OBJMESH_HEADERS=objMeshBinaryLoader.h objMesh.h objMeshRender.h objMeshEncode.h 


OBJMESH_OBJMESHECTS_FILENAMES=$(addprefix $(L)/objMesh/, $(OBJMESH_OBJMESHECTS))
OBJMESH_HEADER_FILENAMES=$(addprefix $(L)/objMesh/, $(OBJMESH_HEADERS))
OBJMESH_LIB_MAKEFILES=$(call GET_LIB_MAKEFILES, $(OBJMESH_LIBS))
OBJMESH_LIB_FILENAMES=$(call GET_LIB_FILENAMES, $(OBJMESH_LIBS))

include $(OBJMESH_LIB_MAKEFILES)

all: $(L)/objMesh/libobjMesh.a

$(L)/objMesh/libobjMesh.a: $(OBJMESH_OBJMESHECTS_FILENAMES)
	ar r $(L)/objMesh/libobjMesh.a $^; cp $(L)/objMesh/libobjMesh.a $(L)/lib; cp $(L)/objMesh/*.h $(L)/include

$(OBJMESH_OBJMESHECTS_FILENAMES): %.o: %.cpp $(OBJMESH_LIB_FILENAMES) $(OBJMESH_HEADER_FILENAMES)
	$(CXX) $(CXXFLAGS) -c $(INCLUDE) $< -o $@

ifeq ($(CLEANFOLDER), OBJMESH)
clean: cleanobjMesh
endif

deepclean: cleanobjMesh

cleanobjMesh:
	$(RM) $(OBJMESH_OBJMESHECTS_FILENAMES) $(L)/objMesh/libobjMesh.a

endif
