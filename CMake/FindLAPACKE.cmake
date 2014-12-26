
find_path(LAPACKE_INCLUDE_DIR
  lapacke.h
  /usr/include
  /usr/local/include
  )
  
find_path(CBLAS_INCLUDE_DIR
  cblas.h
  /usr/include
  /usr/local/include
  PATH_SUFFIXES
  atlas
  atlas-base
  libblas
  )

find_library(CBLAS_LIBRARY
  NAMES cblas
  PATHS
  /usr/lib64
  /usr/lib
  /usr/local/lib64
  /usr/local/lib
  PATH_SUFFIXES
  atlas
  atlas-base
  libblas
  )

# find_library( BLAS_LIBRARY
#   NAMES blas
#   PATHS
#   /usr/lib64
#   )


find_library(LAPACKE_LIBRARY
  NAMES lapacke
  PATHS
  /usr/lib64  
  )

set(LAPACKE_LIBRARIES "${LAPACKE_LIBRARY}")
set(CBLAS_LIBRARIES ${CBLAS_LIBRARY} 
#   ${BLAS_LIBRARY}
  )

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LAPACKE
  REQUIRED_VARS
    LAPACKE_INCLUDE_DIR
    LAPACKE_LIBRARY
    CBLAS_LIBRARY
#     BLAS_LIBRARY
    )

mark_as_advanced(
    LAPACKE_INCLUDE_DIR
    LAPACKE_LIBRARY
    CBLAS_LIBRARY
#     BLAS_LIBRARY
    )
