set(stvk_srcs
    StVKCubeABCD.cpp
    StVKElementABCD.cpp
    StVKElementABCDLoader.cpp
    StVKHessianTensor.cpp
    StVKInternalForces.cpp
    StVKStiffnessMatrix.cpp
    StVKTetABCD.cpp
    StVKTetHighMemoryABCD.cpp
)
set(stvk_hdrs
    StVKCubeABCD.h
    StVKElementABCD.h
    StVKElementABCDLoader.h
    StVKHessianTensor.h
    StVKInternalForces.h
    StVKStiffnessMatrix.h
    StVKTetABCD.h
    StVKTetHighMemoryABCD.h
)

if(VegaFEM_ENABLE_PTHREADS_SUPPORT)
  list(APPEND stvk_srcs
    StVKInternalForcesMT.cpp
    StVKStiffnessMatrixMT.cpp)
  list(APPEND stvk_hdrs 
    StVKInternalForcesMT.h
    StVKStiffnessMatrixMT.h)
endif()

vega_add_library(stvk
  SOURCES
    ${stvk_srcs}
  PUBLIC_HEADERS
    ${stvk_hdrs}
)
target_link_libraries(stvk
  PUBLIC
    minivector
    volumetricMesh
    sparseMatrix
)
