vega_add_library(modalMatrix
  SOURCES modalMatrix.cpp
)
target_include_directories(modalMatrix
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/libmodalMatrix>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(modalMatrix
  PUBLIC
    ${LAPACK_LIBRARIES}
    ${BLAS_LIBRARY}
)
