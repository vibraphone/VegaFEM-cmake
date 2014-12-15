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

vega_add_library(sceneObject
  SOURCES ${SCOBJ_SRCS}
)
target_include_directories(sceneObject
  PRIVATE
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_CURRENT_SOURCE_DIR}/../libobjMesh
   ${CMAKE_CURRENT_SOURCE_DIR}/../liblighting
   ${CMAKE_CURRENT_SOURCE_DIR}/../libminivector
  PUBLIC
   $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(sceneObject objMesh lighting minivector ${GLUT_LIBRARIES})
