include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_CURRENT_SOURCE_DIR}/../libmatrix
   ${CMAKE_CURRENT_SOURCE_DIR}/../libsparseMatrix
   ${CMAKE_CURRENT_SOURCE_DIR}/../libminivector
   ${CMAKE_CURRENT_SOURCE_DIR}/../libvolumetricMesh
   ${CMAKE_SOURCE_DIR}/include
)

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
)

add_library(isotropicHyperelasticFEM SHARED ${IHFEM_SRCS})
target_link_libraries(isotropicHyperelasticFEM minivector volumetricMesh sparseMatrix)
