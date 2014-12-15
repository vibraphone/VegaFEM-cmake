set(MSS_SRCS
 massSpringSystemFromObjMeshConfigFile.cpp
 massSpringSystemFromObjMesh.cpp
 massSpringSystemFromTetMeshConfigFile.cpp
 massSpringSystemFromTetMesh.cpp
 massSpringSystemMT.cpp
 massSpringSystem.cpp
 renderSprings.cpp
 massSpringSystemFromCubicMesh.cpp
 massSpringSystemFromCubicMeshConfigFile.cpp
)

vega_add_library(massSpringSystem
  SOURCES ${MSS_SRCS})
target_include_directories(massSpringSystem
  PRIVATE
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_CURRENT_SOURCE_DIR}/../libminivector
   ${CMAKE_CURRENT_SOURCE_DIR}/../libobjMesh
   ${CMAKE_CURRENT_SOURCE_DIR}/../libvolumetricMesh
   ${CMAKE_CURRENT_SOURCE_DIR}/../libconfigFile
   ${CMAKE_CURRENT_SOURCE_DIR}/../libsparseMatrix
  PUBLIC
   $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(massSpringSystem objMesh volumetricMesh configFile sparseMatrix)
