set(EFM_SRCS
  corotationalLinearFEMForceModel.cpp
  massSpringSystemForceModel.cpp
  StVKForceModel.cpp
  isotropicHyperelasticFEMForceModel.cpp
  linearFEMForceModel.cpp
)

vega_add_library(elasticForceModel
  SOURCES ${EFM_SRCS})
target_include_directories(elasticForceModel
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(elasticForceModel
  PUBLIC
    forceModel
    corotationalLinearFEM
    massSpringSystem
    stvk
    isotropicHyperelasticFEM
  INTERFACE
    sparseMatrix
    minivector
    volumetricMesh
)
