set(PD_SRCS
polarDecomposition.cpp
polarDecompositionGradient.cpp
)

vega_add_library(polarDecomposition SOURCES ${PD_SRCS})
target_include_directories(polarDecomposition
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(polarDecomposition
  PUBLIC
    minivector
)
