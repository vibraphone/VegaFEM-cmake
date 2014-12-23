set(MSS_SRCS
  massSpringSystemFromObjMeshConfigFile.cpp
  massSpringSystemFromObjMesh.cpp
  massSpringSystemFromTetMeshConfigFile.cpp
  massSpringSystemFromTetMesh.cpp
  massSpringSystemMT.cpp
  massSpringSystem.cpp
  massSpringSystemFromCubicMesh.cpp
  massSpringSystemFromCubicMeshConfigFile.cpp
)
if (VegaFEM_ENABLE_OpenGL_SUPPORT)
  set(MSS_SRCS
    ${MSS_SRCS}
    renderSprings.cpp
  )
endif()

vega_add_library(massSpringSystem
  SOURCES ${MSS_SRCS})
target_include_directories(massSpringSystem
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(massSpringSystem
  PUBLIC
    objMesh
    volumetricMesh
    configFile
    sparseMatrix
  INTERFACE
    minivector
)
if (VegaFEM_ENABLE_OpenGL_SUPPORT)
  target_link_libraries(massSpringSystem
    PUBLIC
      ${OPENGL_gl_LIBRARY}
      ${OPENGL_glu_LIBRARY}
  )
endif()
