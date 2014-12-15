set(PD_SRCS
polarDecomposition.cpp
polarDecompositionGradient.cpp
)

vega_add_library(polarDecomposition SOURCES ${PD_SRCS})
target_include_directories(polarDecomposition
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../libminivector
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
