include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_SOURCE_DIR}/include
)

set(SM_SRCS
  sparseMatrix.cpp
  sparseMatrixMT.cpp
)

add_library(sparseMatrix SHARED ${SM_SRCS})
