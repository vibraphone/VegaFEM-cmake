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

vega_add_library(reducedElasticForceModel SOURCES ${reducedelasticforcemodel_srcs})
target_include_directories(reducedElasticForceModel
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(reducedElasticForceModel
  PUBLIC
    reducedForceModel
    reducedStvk
    stvk
    massSpringSystem
)
