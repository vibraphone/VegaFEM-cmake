set(massspringsystem_srcs
  massSpringSystemFromObjMeshConfigFile.cpp
  massSpringSystemFromObjMesh.cpp
  massSpringSystemFromTetMeshConfigFile.cpp
  massSpringSystemFromTetMesh.cpp
  massSpringSystem.cpp
  massSpringSystemFromCubicMesh.cpp
  massSpringSystemFromCubicMeshConfigFile.cpp
)
set(massspringsystem_hdrs
    massSpringSystem.h
    massSpringSystemFromCubicMesh.h
    massSpringSystemFromCubicMeshConfigFile.h
    massSpringSystemFromObjMesh.h
    massSpringSystemFromObjMeshConfigFile.h
    massSpringSystemFromTetMesh.h
    massSpringSystemFromTetMeshConfigFile.h
)

if(VegaFEM_ENABLE_PTHREADS_SUPPORT)
  list(APPEND massspringsystem_srcs massSpringSystemMT.cpp)
  list(APPEND massspringsystem_hdrs massSpringSystemMT.h)
endif()

if (VegaFEM_ENABLE_OpenGL_SUPPORT)
  list(APPEND massspringsystem_srcs renderSprings.cpp)
  list(APPEND massspringsystem_hdrs renderSprings.h)
endif()

vega_add_library(massSpringSystem
  SOURCES
    ${massspringsystem_srcs}
  PUBLIC_HEADERS
    ${massspringsystem_hdrs}
)
target_link_libraries(massSpringSystem
  PUBLIC
    objMesh
    volumetricMesh
    configFile
    sparseMatrix
  INTERFACE
    minivector
    ${CMAKE_THREAD_LIBS_INIT}
)
if (VegaFEM_ENABLE_OpenGL_SUPPORT)
  target_link_libraries(massSpringSystem
    PUBLIC
      ${OPENGL_gl_LIBRARY}
      ${OPENGL_glu_LIBRARY}
  )
endif()
