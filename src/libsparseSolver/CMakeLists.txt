include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_CURRENT_SOURCE_DIR}/../libsparseMatrix
   ${CMAKE_SOURCE_DIR}/include
)

set(SOLVER_SRCS
 linearSolver.cpp
 PardisoSolver.cpp
 SPOOLESSolver.cpp
 SPOOLESSolverMT.cpp
 CGSolver.cpp
)

add_library(sparseSolver SHARED ${SOLVER_SRCS})
target_link_libraries(sparseSolver sparseMatrix)
