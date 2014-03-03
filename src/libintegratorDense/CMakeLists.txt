include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_SOURCE_DIR}/include
	${PARDISO_INCLUDE}
	)

set(integratordense_srcs
	centralDifferencesDense.cpp
	implicitBackwardEulerDense.cpp
	implicitNewmarkDense.cpp
	integratorBaseDense.cpp
	)

add_library(integratordense SHARED ${integratordense_srcs})
target_link_libraries(integratordense matrix integrator performanceCounter reducedForceModel)
