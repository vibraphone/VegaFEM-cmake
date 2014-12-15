set(VOL_SRCS
volumetricMeshParser.cpp
generateInterpolationMatrix.cpp
generateMassMatrix.cpp
generateSurfaceMesh.cpp
generateMeshGraph.cpp
computeStiffnessMatrixNullspace.cpp
cubicMesh.cpp
tetMesh.cpp
volumetricMeshLoader.cpp
volumetricMesh.cpp
volumetricMeshENuMaterial.cpp
volumetricMeshMooneyRivlinMaterial.cpp
volumetricMeshExtensions.cpp
)

vega_add_library(volumetricMesh SOURCES ${VOL_SRCS})
target_include_directories(volumetricMesh
  PRIVATE
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_CURRENT_SOURCE_DIR}/../libsparseMatrix
   ${CMAKE_CURRENT_SOURCE_DIR}/../libgraph
   ${CMAKE_CURRENT_SOURCE_DIR}/../libobjMesh
   ${CMAKE_CURRENT_SOURCE_DIR}/../libminivector
  PUBLIC
   $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(volumetricMesh sparseMatrix graph objMesh minivector)
