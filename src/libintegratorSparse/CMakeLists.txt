set(integratorsparse_srcs
  centralDifferencesSparse.cpp
  eulerSparse.cpp
  implicitBackwardEulerSparse.cpp
  implicitNewmarkSparse.cpp
  integratorBaseSparse.cpp
  )

vega_add_library(integratorSparse
  SOURCES ${integratorsparse_srcs})
target_include_directories(integratorSparse
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    ${PARDISO_INCLUDE}
)
target_link_libraries(integratorSparse
  PUBLIC
    integrator
    performanceCounter
    insertRows
    sparseSolver
    forceModel
    matrixIO
  INTERFACE
    sparseMatrix
)
