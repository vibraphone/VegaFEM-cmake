set(MINIVEC_SRCS
   mat3d.cpp
   vec3d.cpp
   vec2d.cpp
   eig3.cpp
)

include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_SOURCE_DIR}/include
)

add_library(minivector SHARED ${MINIVEC_SRCS})
