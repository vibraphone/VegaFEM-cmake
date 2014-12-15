set(INTEGRATOR_SRCS
  integratorBase.cpp
  getIntegratorSolver.cpp
)

vega_add_library(integrator
  SOURCES ${INTEGRATOR_SRCS})
target_include_directories(integrator
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../libsparseMatrix
    ${CMAKE_CURRENT_SOURCE_DIR}/../libperformanceCounter
    ${CMAKE_CURRENT_SOURCE_DIR}/../libinsertRows
    ${CMAKE_CURRENT_SOURCE_DIR}/../libsparseSolver
    ${CMAKE_CURRENT_SOURCE_DIR}/../libforceModel
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    ${PARDISO_INCLUDE_DIRS}
)
target_link_libraries(integrator
  PUBLIC
    performanceCounter
    insertRows
    sparseSolver
    forceModel
)
