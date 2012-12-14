include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_CURRENT_SOURCE_DIR}/../libminivector
   ${CMAKE_CURRENT_SOURCE_DIR}/../libobjMesh
   ${CMAKE_CURRENT_SOURCE_DIR}/../libvolumetricMesh
   ${CMAKE_CURRENT_SOURCE_DIR}/../libconfigFile
   ${CMAKE_CURRENT_SOURCE_DIR}/../libsparseMatrix
   ${CMAKE_SOURCE_DIR}/include
)

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

add_library(massSpringSystem SHARED ${MSS_SRCS})
target_link_libraries(massSpringSystem objMesh volumetricMesh configFile sparseMatrix)
