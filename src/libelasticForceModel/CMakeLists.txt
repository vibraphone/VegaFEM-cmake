include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_CURRENT_SOURCE_DIR}/../libsparseMatrix
   ${CMAKE_CURRENT_SOURCE_DIR}/../libforceModel
   ${CMAKE_CURRENT_SOURCE_DIR}/../libminivector
   ${CMAKE_CURRENT_SOURCE_DIR}/../libvolumetricMesh
   ${CMAKE_CURRENT_SOURCE_DIR}/../libcorotationalLinearFEM
   ${CMAKE_CURRENT_SOURCE_DIR}/../libmassSpringSystem
   ${CMAKE_CURRENT_SOURCE_DIR}/../libstvk
   ${CMAKE_CURRENT_SOURCE_DIR}/../libisotropicHyperelasticFEM
   ${CMAKE_SOURCE_DIR}/include
)

set(EFM_SRCS
 corotationalLinearFEMForceModel.cpp
 massSpringSystemForceModel.cpp
 StVKForceModel.cpp
 isotropicHyperelasticFEMForceModel.cpp
 linearFEMForceModel.cpp
)

add_library(elasticForceModel SHARED ${EFM_SRCS})
target_link_libraries(elasticForceModel forceModel corotationalLinearFEM massSpringSystem stvk isotropicHyperelasticFEM)
