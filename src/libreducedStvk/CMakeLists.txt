set(reducedstvk_srcs
    StVKReducedInternalForces.cpp
    StVKReducedStiffnessMatrix.cpp
    StVKReducedHessianTensor.cpp
)
set(reducedstvk_hdrs
    StVKReducedHessianTensor.h
    StVKReducedInternalForces.h
    StVKReducedStiffnessMatrix.h
)

if(VegaFEM_ENABLE_PTHREADS_SUPPORT)
  list(APPEND reducedstvk_srcs StVKReducedInternalForcesMT.cpp)
  list(APPEND reducedstvk_hdrs StVKReducedInternalForcesMT.h)
endif()

vega_add_library(reducedStvk
  SOURCES
    ${reducedstvk_srcs}
  PUBLIC_HEADERS
    ${reducedstvk_hdrs}
)
target_link_libraries(reducedStvk
  PUBLIC
    stvk
    modalMatrix
    matrix
    ${BLAS}
    ${CBLAS_LIBRARY}
    ${LAPACK_LIBRARIES}
)
