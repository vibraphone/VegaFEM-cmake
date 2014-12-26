set(sceneobjreduced_srcs
  sceneObjectReduced.cpp
  sceneObjectReduced6DOF.cpp
  sceneObjectReducedCPU.cpp
  sceneObjectReducedCPU6DOF.cpp
)
set(sceneobjreduced_hdrs
  sceneObjectReduced.h
  sceneObjectReduced6DOF.h
  sceneObjectReducedCPU.h
  sceneObjectReducedCPU6DOF.h
)
if(VegaFEM_ENABLE_Cg_SUPPORT)
  set(sceneobjreduced_srcs
    ${sceneobjreduced_srcs}
    sceneObjectReducedGPU.cpp
    sceneObjectReducedGPU6DOF.cpp
  )
  set(sceneobjreduced_hdrs
    ${sceneobjreduced_hdrs}
    sceneObjectReducedGPU.h
    sceneObjectReducedGPU6DOF.h
  )
endif(VegaFEM_ENABLE_Cg_SUPPORT)

vega_add_library(sceneObjectReduced
  SOURCES ${sceneobjreduced_srcs}
  PUBLIC_HEADERS ${sceneobjreduced_hdrs}
)
target_link_libraries(sceneObjectReduced
  PUBLIC
    objMesh
    lighting
    minivector
    modalMatrix
    sceneObject
)
if (VegaFEM_ENABLE_Cg_SUPPORT)
  target_link_libraries(sceneObjectReduced
    PUBLIC
      objMeshGPUDeformer
  )
endif()
