vega_add_library(forceModel
  SOURCES forceModel.cpp)
target_include_directories(forceModel
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../libsparseMatrix
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(forceModel
  PUBLIC
    sparseMatrix
)
