set(IHFEM_SRCS
  isotropicMaterial.cpp
  MooneyRivlinIsotropicMaterial.cpp
  neoHookeanIsotropicMaterial.cpp
  StVKIsotropicMaterial.cpp
  homogeneousMooneyRivlinIsotropicMaterial.cpp
  homogeneousStVKIsotropicMaterial.cpp
  homogeneousNeoHookeanIsotropicMaterial.cpp
  isotropicHyperelasticFEM.cpp
  isotropicHyperelasticFEMMT.cpp
  isotropicMaterialWithCompressionResistance.cpp
)

vega_add_library(isotropicHyperelasticFEM
  SOURCES ${IHFEM_SRCS})
target_include_directories(isotropicHyperelasticFEM
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(isotropicHyperelasticFEM
  PUBLIC
    minivector
    volumetricMesh
    sparseMatrix
)
