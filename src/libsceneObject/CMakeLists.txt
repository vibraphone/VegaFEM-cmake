include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_CURRENT_SOURCE_DIR}/../libobjMesh
   ${CMAKE_CURRENT_SOURCE_DIR}/../liblighting
   ${CMAKE_CURRENT_SOURCE_DIR}/../libminivector
   ${CMAKE_SOURCE_DIR}/include
)

set(SCOBJ_SRCS
   sceneObject.cpp
	sceneObject6DOF.cpp
   sceneObjectDeformable.cpp
	sceneObjectDeformable6DOF.cpp
   sceneObjectWithRestPosition.cpp
)
if(CG_LIBS)
	set(SCOBJS_SRCS
		${SCOBJS_SRCS}
		sceneObjectDeformableGPU.cpp
		sceneObjectDeformableGPU6DOF.cpp
		)
endif(CG_LIBS)

add_library(sceneObject SHARED ${SCOBJ_SRCS})
target_link_libraries(sceneObject objMesh lighting minivector)
