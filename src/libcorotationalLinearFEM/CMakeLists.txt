vega_add_library(corotationalLinearFEM
  SOURCES corotationalLinearFEM.cpp corotationalLinearFEMMT.cpp)
target_include_directories(corotationalLinearFEM
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(corotationalLinearFEM
  PUBLIC
    polarDecomposition
    volumetricMesh
    sparseMatrix
  INTERFACE
    minivector
)
