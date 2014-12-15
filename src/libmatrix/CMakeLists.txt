set(matrix_srcs
  matrixProjection.cpp
  matrixPCA.cpp
  matrixBLAS.cpp
  matrixLAPACK.cpp
  matrix.cpp
  matrixExp.cpp
)

vega_add_library(matrix SOURCES ${matrix_srcs})
target_include_directories(matrix
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(matrix matrixIO)
