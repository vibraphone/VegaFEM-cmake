set(SM_SRCS
  sparseMatrix.cpp
  sparseMatrixMT.cpp
)

vega_add_library(sparseMatrix SOURCES ${SM_SRCS})
target_include_directories(sparseMatrix
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
if(VegaFEM_ENABLE_OpenMP_SUPPORT)
  target_compile_definitions(sparseMatrix
    PUBLIC USE_OPENMP
  )
  target_compile_options(sparseMatrix
    PUBLIC "${OpenMP_CXX_FLAGS}"
  )
endif()
