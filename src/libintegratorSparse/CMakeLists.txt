include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../libinsertRows
	${CMAKE_CURRENT_SOURCE_DIR}/../libsparseMatrix
	${CMAKE_CURRENT_SOURCE_DIR}/../libforceModel
	${CMAKE_CURRENT_SOURCE_DIR}/../libintegrator
	${CMAKE_CURRENT_SOURCE_DIR}/../libperformanceCounter
   ${CMAKE_SOURCE_DIR}/include
	${PARDISO_INCLUDE}
	)

set(integratorsparse_srcs
	centralDifferencesSparse.cpp
	eulerSparse.cpp
	implicitBackwardEulerSparse.cpp
	implicitNewmarkSparse.cpp
	integratorBaseSparse.cpp
	)

add_library(integratorSparse SHARED ${integratorsparse_srcs})
target_link_libraries(integratorSparse integrator performanceCounter insertRows sparseSolver forceModel)
