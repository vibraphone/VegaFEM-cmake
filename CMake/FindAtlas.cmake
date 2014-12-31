
find_path(ATLAS_INCLUDE_DIR
  cblas.h
  /usr/include
  /usr/local/include
  /usr/local/include/atlas
  )

find_library( ATLAS_LIBRARY
  NAMES atlas
  PATHS
  /usr/local/lib/atlas
  /usr/lib64/atlas
  )

find_library( CBLAS_LIBRARY
  NAMES cblas
  PATHS
  /usr/local/lib/atlas
  /usr/lib64/atlas
  )

find_library( LAPACK_LIBRARY
  NAMES lapack
  PATHS
  /usr/local/lib/atlas
  /usr/lib64/atlas
  )

set(ATLAS_LIBRARIES "${ATLAS_LIBRARY};${CBLAS_LIBRARY};${LAPACK_LIBRARY}")
set(CBLAS_LIBRARIES "${CBLAS_LIBRARY}")
set(LAPACK_LIBRARIES "${LAPACK_LIBRARY}")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ATLAS
  REQUIRED_VARS
    ATLAS_INCLUDE_DIR
    ATLAS_LIBRARY
    CBLAS_LIBRARY
    LAPACK_LIBRARY
    )

mark_as_advanced(
    ATLAS_INCLUDE_DIR
    ATLAS_LIBRARY
    CBLAS_LIBRARY
    LAPACK_LIBRARY
    )
