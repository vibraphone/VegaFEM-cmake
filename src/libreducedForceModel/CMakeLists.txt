include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/include
	)

add_library(reducedForceModel SHARED reducedForceModel.cpp reducedForceModelWithHessian.cpp)
