include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_SOURCE_DIR}/include
)

set(matrix_srcs
	matrixProjection.cpp
	matrixPCA.cpp
	matrixBLAS.cpp
	matrixLAPACK.cpp
	matrix.cpp
	matrixExp.cpp
	)

add_library(matrix SHARED ${matrix_srcs})
target_link_libraries(matrix matrixIO)
