set(SOLVER_SRCS
 linearSolver.cpp
 #ARPACKSolver.cpp
 PardisoSolver.cpp
 SPOOLESSolver.cpp
 SPOOLESSolverMT.cpp
 CGSolver.cpp
 invMKSolver.cpp
)

vega_add_library(sparseSolver SOURCES ${SOLVER_SRCS})
target_include_directories(sparseSolver
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(sparseSolver
  PUBLIC
    sparseMatrix
)
