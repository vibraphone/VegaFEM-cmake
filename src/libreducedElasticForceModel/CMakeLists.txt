include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/include
	)

set(reducedelasticforcemodel_srcs
	reducedLinearForceModel.cpp
	reducedLinearStVKForceModel.cpp
	reducedMassSpringSystemForceModel.cpp
	reducedMassSpringSystemForceModelWithHessian.cpp
	reducedSpringForceModel.cpp
	reducedStVKForceModel.cpp
	reducedStVKForceModelWithHessian.cpp
	reducedSubspaceStVKForceModel.cpp
	)

add_library(reducedElasticForceModel SHARED ${reducedelasticforcemodel_srcs})
target_link_libraries(reducedElasticForceModel reducedForceModel reducedStvk stvk massSpringSystem)
