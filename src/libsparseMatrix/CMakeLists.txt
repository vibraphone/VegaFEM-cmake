vega_add_library(sparseMatrix
  SOURCES
    sparseMatrix.cpp
    sparseMatrixMT.cpp
  PUBLIC_HEADERS
    sparseMatrix.h
    sparseMatrixMT.h
)
if(VegaFEM_ENABLE_OpenMP_SUPPORT)
  target_compile_definitions(sparseMatrix
    PUBLIC USE_OPENMP
  )
  target_compile_options(sparseMatrix
    PUBLIC "${OpenMP_CXX_FLAGS}"
  )
endif()
