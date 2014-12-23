vega_add_library(graph
  SOURCES graph.cpp)
target_include_directories(graph
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(graph
  PUBLIC
    sparseMatrix
    matrixIO
)
