include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_CURRENT_SOURCE_DIR}/../libsparseMatrix
   ${CMAKE_CURRENT_SOURCE_DIR}/../libperformanceCounter
   ${CMAKE_CURRENT_SOURCE_DIR}/../libinsertRows
   ${CMAKE_CURRENT_SOURCE_DIR}/../libsparseSolver
   ${CMAKE_CURRENT_SOURCE_DIR}/../libforceModel
   ${CMAKE_SOURCE_DIR}/include
   ${PARDISO_INCLUDE_DIRS}
)

set(INTEGRATOR_SRCS
 integratorBase.cpp
 getIntegratorSolver.cpp
)

add_library(integrator SHARED ${INTEGRATOR_SRCS})
target_link_libraries(integrator performanceCounter insertRows sparseSolver forceModel)
