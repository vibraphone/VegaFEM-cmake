vega_add_library(modalMatrix
  SOURCES modalMatrix.cpp
)
target_include_directories(modalMatrix
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(modalMatrix
  PUBLIC
    matrixIO
    ${LAPACK_LIBRARIES}
    ${BLAS_LIBRARY}
)
