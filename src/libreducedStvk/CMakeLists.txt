set(rstvk_srcs
  StVKReducedInternalForces.cpp
  StVKReducedInternalForcesMT.cpp
  StVKReducedStiffnessMatrix.cpp
  StVKReducedHessianTensor.cpp
)

vega_add_library(reducedStvk SOURCES ${rstvk_srcs})
target_include_directories(reducedStvk
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(reducedStvk
  PUBLIC
    stvk
    modalMatrix
    matrix
)
