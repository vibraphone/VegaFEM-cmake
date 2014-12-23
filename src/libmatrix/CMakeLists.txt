option(VegaFEM_ENABLE_EXPOKIT "Does your system provide ExpoKit?" OFF)

set(matrix_srcs
  matrixProjection.cpp
  matrixPCA.cpp
  matrixBLAS.cpp
  matrixLAPACK.cpp
  matrix.cpp
)
if (VegaFEM_ENABLE_EXPOKIT)
  set(matrix_srcs
    ${matrix_srcs}
    matrixExp.cpp
  )
endif()

vega_add_library(matrix SOURCES ${matrix_srcs})
target_include_directories(matrix
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(matrix
  PUBLIC
    matrixIO
    ${LAPACK_LIBRARIES}
)
