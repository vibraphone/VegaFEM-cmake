include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_CURRENT_SOURCE_DIR}/../libsparseMatrix
   ${CMAKE_SOURCE_DIR}/include
)

add_library(graph SHARED graph.cpp)
target_link_libraries(graph sparseMatrix)
