include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_CURRENT_SOURCE_DIR}/../libsparseMatrix
   ${CMAKE_CURRENT_SOURCE_DIR}/../libpolarDecomposition
   ${CMAKE_CURRENT_SOURCE_DIR}/../libminivector
   ${CMAKE_CURRENT_SOURCE_DIR}/../libvolumetricMesh
   ${CMAKE_SOURCE_DIR}/include
)

add_library(corotationalLinearFEM SHARED corotationalLinearFEM.cpp corotationalLinearFEMMT.cpp)
target_link_libraries(corotationalLinearFEM polarDecomposition volumetricMesh sparseMatrix)
