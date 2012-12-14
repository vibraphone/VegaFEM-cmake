ifndef SCENEOBJECT
SCENEOBJECT=SCENEOBJECT

ifndef CLEANFOLDER
CLEANFOLDER=SCENEOBJECT
endif

include ../../Makefile-headers/Makefile-header
R ?= ../..

# the object files to be compiled for this library
SCENEOBJECT_OBJECTS=sceneObjectDeformable.o sceneObject.o sceneObjectWithRestPosition.o

# the libraries this library depends on
SCENEOBJECT_LIBS=objMesh lighting minivector

# the headers in this library
SCENEOBJECT_HEADERS=sceneObjectDeformable.h sceneObject.h sceneObjects.h sceneObjectWithRestPosition.h

SCENEOBJECT_OBJECTS_FILENAMES=$(addprefix $(L)/sceneObject/, $(SCENEOBJECT_OBJECTS))
SCENEOBJECT_HEADER_FILENAMES=$(addprefix $(L)/sceneObject/, $(SCENEOBJECT_HEADERS))
SCENEOBJECT_LIB_MAKEFILES=$(call GET_LIB_MAKEFILES, $(SCENEOBJECT_LIBS))
SCENEOBJECT_LIB_FILENAMES=$(call GET_LIB_FILENAMES, $(SCENEOBJECT_LIBS))

include $(SCENEOBJECT_LIB_MAKEFILES)

all: $(L)/sceneObject/libsceneObject.a

$(L)/sceneObject/libsceneObject.a: $(SCENEOBJECT_OBJECTS_FILENAMES)
	ar r $@ $^; cp $@ $(L)/lib; cp $(L)/sceneObject/*.h $(L)/include

$(SCENEOBJECT_OBJECTS_FILENAMES): %.o: %.cpp $(SCENEOBJECT_LIB_FILENAMES) $(SCENEOBJECT_HEADER_FILENAMES)
	$(CXX) $(CXXFLAGS) -c $(INCLUDE) $< -o $@

ifeq ($(CLEANFOLDER), SCENEOBJECT)
clean: cleansceneObject
endif

deepclean: cleansceneObject

cleansceneObject:
	$(RM) $(SCENEOBJECT_OBJECTS_FILENAMES) $(L)/sceneObject/libsceneObject.a

endif
