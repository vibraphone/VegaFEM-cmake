include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_CURRENT_SOURCE_DIR}/../libsparseMatrix
   ${CMAKE_CURRENT_SOURCE_DIR}/../libvolumetricMesh
   ${CMAKE_CURRENT_SOURCE_DIR}/../libminivector
   ${CMAKE_SOURCE_DIR}/include
)

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

add_library(stvk SHARED ${STVK_SRCS})
target_link_libraries(stvk minivector volumetricMesh sparseMatrix)
