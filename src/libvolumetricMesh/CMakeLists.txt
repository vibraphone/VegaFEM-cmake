include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_CURRENT_SOURCE_DIR}/../libsparseMatrix
   ${CMAKE_CURRENT_SOURCE_DIR}/../libgraph
   ${CMAKE_CURRENT_SOURCE_DIR}/../libmatrix
   ${CMAKE_CURRENT_SOURCE_DIR}/../libobjMesh
   ${CMAKE_CURRENT_SOURCE_DIR}/../libminivector
   ${CMAKE_SOURCE_DIR}/include
)


set(VOL_SRCS
volumetricMeshParser.cpp 
generateInterpolationMatrix.cpp 
generateMassMatrix.cpp 
generateSurfaceMesh.cpp 
generateMeshGraph.cpp 
cubicMesh.cpp 
tetMesh.cpp 
volumetricMeshLoader.cpp 
volumetricMesh.cpp 
volumetricMeshENuMaterial.cpp 
volumetricMeshMooneyRivlinMaterial.cpp 
volumetricMeshExtensions.cpp
)

add_library(volumetricMesh SHARED ${VOL_SRCS})
target_link_libraries(volumetricMesh sparseMatrix graph matrix objMesh minivector)
