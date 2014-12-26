#if defined(__INTEL_MKL__)
  #include "mkl_cblas.h"
  #include "mkl_types.h"
  #include "mkl_lapack.h"
  #include "mkl_blas.h"
#elif defined(__APPLE__)
  #include <Accelerate/Accelerate.h>
#else // Requires Lapacke & cblas
  #include<lapacke.h>
  #include<cblas.h>
  #define DGESVD LAPACK_dgesvd
  #define DGETRF LAPACK_dgetrf
  #define DGETRI LAPACK_dgetri
  #define SGETRF LAPACK_sgetrf
  #define SGETRI LAPACK_sgetri
  #define DGESVD LAPACK_dgesvd
  #define SGESVD LAPACK_sgesvd
  #define DGELSY LAPACK_dgelsy
  #define SGELSY LAPACK_sgelsy
  #define DGESV LAPACK_dgesv
  #define SGESV LAPACK_sgesv
  #define DSYEV LAPACK_dsyev
  #define SSYEV LAPACK_ssyev
  #define DSYGV LAPACK_dsygv
  #define SSYGV LAPACK_ssygv
  #define SGEEV LAPACK_sgeev
  #define DGEEV LAPACK_dgeev
  #define DGETRS LAPACK_dgetrs
  #define DSYSV LAPACK_dsysv
  #define DPOSV LAPACK_dposv
#endif