#

# INTEL_MKL_INCLUDE_DIR - where to find autopack.h
# MKLSOLVER_LIBRARIES   - List of fully qualified libraries to link against.
# MKLSOLVER_FOUND       - Do not attempt to use if "no" or undefined.

set(INTEL_MKL_YEAR 2011)
set(INTEL_MKL_ARCH intel64)
find_path(INTEL_MKL_INCLUDE_DIR 
  mkl_blas.h
  "C:/Program Files/Intel/ComposerXE-${INTEL_MKL_YEAR}/mkl/include"
  /opt/intel/composerxe-${INTEL_MKL_YEAR}/mkl/include
)

# set(LIB_MKL_LIST mkl_intel_ilp64 mkl_intel_thread mkl_core mkl_solver_ilp64 mkl_mc mkl_mc3 mkl_lapack PTHREAD mkl_p4n iomp5)

find_library(MKL_INTEL_ILP64 mkl_intel_ilp64
  "C:/Program Files/Intel/ComposerXE-${INTEL_MKL_YEAR}/mkl/lib/${INTEL_MKL_ARCH}"
  /opt/intel/composerxe-${INTEL_MKL_YEAR}/mkl/lib/${INTEL_MKL_ARCH}
)

if(CMAKE_COMPILER_IS_GNUCXX )
    find_library(MKL_THREAD mkl_gnu_thread
    "C:/Program Files/Intel/ComposerXE-${INTEL_MKL_YEAR}/mkl/lib/${INTEL_MKL_ARCH}"
    /opt/intel/composerxe-${INTEL_MKL_YEAR}/mkl/lib/${INTEL_MKL_ARCH}
    )
else(CMAKE_COMPILER_IS_GNUCXX )
    find_library(MKL_THREAD mkl_intel_thread
    "C:/Program Files/Intel/ComposerXE-${INTEL_MKL_YEAR}/mkl/lib/${INTEL_MKL_ARCH}"
    /opt/intel/composerxe-${INTEL_MKL_YEAR}/mkl/lib/${INTEL_MKL_ARCH}
    )
endif(CMAKE_COMPILER_IS_GNUCXX )

find_library(MKL_CORE mkl_core
  "C:/Program Files/Intel/ComposerXE-${INTEL_MKL_YEAR}/mkl/lib/${INTEL_MKL_ARCH}"
  /opt/intel/composerxe-${INTEL_MKL_YEAR}/mkl/lib/${INTEL_MKL_ARCH}
)

find_library(MKL_SOLVER_ILP64 mkl_solver_ilp64
  "C:/Program Files/Intel/ComposerXE-${INTEL_MKL_YEAR}/mkl/lib/${INTEL_MKL_ARCH}"
  /opt/intel/composerxe-${INTEL_MKL_YEAR}/mkl/lib/${INTEL_MKL_ARCH}
)

find_library(MKL_MC mkl_mc
  "C:/Program Files/Intel/ComposerXE-${INTEL_MKL_YEAR}/mkl/lib/${INTEL_MKL_ARCH}"
  /opt/intel/composerxe-${INTEL_MKL_YEAR}/mkl/lib/${INTEL_MKL_ARCH}
)

find_library(MKL_MC3 mkl_mc3
  "C:/Program Files/Intel/ComposerXE-${INTEL_MKL_YEAR}/mkl/lib/${INTEL_MKL_ARCH}"
  /opt/intel/composerxe-${INTEL_MKL_YEAR}/mkl/lib/${INTEL_MKL_ARCH}
)

find_library(MKL_LAPACK mkl_lapack
  "C:/Program Files/Intel/ComposerXE-${INTEL_MKL_YEAR}/mkl/lib/${INTEL_MKL_ARCH}"
  /opt/intel/composerxe-${INTEL_MKL_YEAR}/mkl/lib/${INTEL_MKL_ARCH}
)

find_library(MKL_P4N mkl_p4n
  "C:/Program Files/Intel/ComposerXE-${INTEL_MKL_YEAR}/mkl/lib/${INTEL_MKL_ARCH}"
  /opt/intel/composerxe-${INTEL_MKL_YEAR}/mkl/lib/${INTEL_MKL_ARCH}
)

set(INTEL_MKL_LIBRARIES ${MKL_P4N} ${MKL_MC3} ${MKL_MC} ${MKL_LAPACK} ${MKL_SOLVER_ILP64} ${MKL_CORE} ${MKL_THREAD} ${MKL_INTEL_ILP64})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(MKL
  REQUIRED_VARS
    INTEL_MKL_INCLUDE_DIR
    MKL_P4N
    MKL_MC3 
    MKL_MC 
    MKL_LAPACK 
    MKL_SOLVER_ILP64 
    MKL_CORE 
    MKL_THREAD 
    MKL_INTEL_ILP64
    )
    
mark_as_advanced(
    INTEL_MKL_INCLUDE_DIR
    MKL_P4N
    MKL_MC3 
    MKL_MC 
    MKL_LAPACK 
    MKL_SOLVER_ILP64 
    MKL_CORE 
    MKL_THREAD 
    MKL_INTEL_ILP64
)
