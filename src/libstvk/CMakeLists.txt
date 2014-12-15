set(STVK_SRCS
 StVKCubeABCD.cpp
 StVKElementABCD.cpp
 StVKElementABCDLoader.cpp
 StVKHessianTensor.cpp
 StVKInternalForces.cpp
 StVKInternalForcesMT.cpp
 StVKStiffnessMatrix.cpp
 StVKStiffnessMatrixMT.cpp
 StVKTetABCD.cpp
 StVKTetHighMemoryABCD.cpp
)

vega_add_library(stvk SOURCES ${STVK_SRCS})
target_include_directories(stvk
  PRIVATE
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_CURRENT_SOURCE_DIR}/../libsparseMatrix
   ${CMAKE_CURRENT_SOURCE_DIR}/../libvolumetricMesh
   ${CMAKE_CURRENT_SOURCE_DIR}/../libminivector
  PUBLIC
   $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(stvk minivector volumetricMesh sparseMatrix)
