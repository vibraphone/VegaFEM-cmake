include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_CURRENT_SOURCE_DIR}/../libminivector
   ${CMAKE_SOURCE_DIR}/include
)

set(PD_SRCS
polarDecomposition.cpp  
polarDecompositionGradient.cpp  
)

add_library(polarDecomposition SHARED ${PD_SRCS})
