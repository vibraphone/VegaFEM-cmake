set(SCOBJ_SRCS
  sceneObject.cpp
  sceneObject6DOF.cpp
  sceneObjectDeformable.cpp
  sceneObjectDeformable6DOF.cpp
  sceneObjectWithRestPosition.cpp
)
set(SCOBJ_HDRS
  sceneObject.h
  sceneObject6DOF.h
  sceneObjectDeformable.h
  sceneObjectDeformable6DOF.h
  sceneObjectWithRestPosition.h
)
if(CG_LIBS)
	set(SCOBJS_SRCS
		${SCOBJS_SRCS}
		sceneObjectDeformableGPU.cpp
		sceneObjectDeformableGPU6DOF.cpp
  )
  set(SCOBJS_HDRS
    ${SCOBJS_HDRS}
    sceneObjectDeformableGPU.h
    sceneObjectDeformableGPU6DOF.h
  )
endif(CG_LIBS)

vega_add_library(sceneObject
  SOURCES ${SCOBJ_SRCS}
  PUBLIC_HEADERS ${SCOBJ_HDRS}
)
target_link_libraries(sceneObject
  PUBLIC
    objMesh
    lighting
    minivector
    ${GLUT_LIBRARIES}
)
