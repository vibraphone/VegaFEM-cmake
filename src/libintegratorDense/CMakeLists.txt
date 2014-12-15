set(integratordense_srcs
	centralDifferencesDense.cpp
	implicitBackwardEulerDense.cpp
	implicitNewmarkDense.cpp
	integratorBaseDense.cpp
)

vega_add_library(integratordense
  SOURCES ${integratordense_srcs})
target_include_directories(integratordense
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
	  ${PARDISO_INCLUDE}
)
target_link_libraries(integratordense
  PUBLIC
    matrix
    integrator
    performanceCounter
    reducedForceModel
)
