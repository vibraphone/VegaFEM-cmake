set(MINIVEC_SRCS
   mat3d.cpp
   vec3d.cpp
   vec2d.cpp
   eig3.cpp
)

vega_add_library(minivector SOURCES ${MINIVEC_SRCS})
target_include_directories(minivector
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
