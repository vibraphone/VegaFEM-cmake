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
  #define DGEEV LAPACK_dgeev
  #define DGELSY LAPACK_dgelsy
  #define DGESVD LAPACK_dgesvd
  #define DGESVD LAPACK_dgesvd
  #define DGESV LAPACK_dgesv
  #define DGETRF LAPACK_dgetrf
  #define DGETRI LAPACK_dgetri
  #define DGETRS LAPACK_dgetrs
  #define DPOSV LAPACK_dposv
  #define DSYEV LAPACK_dsyev
  #define DSYGV LAPACK_dsygv
  #define DSYSV LAPACK_dsysv
  #define SGEEV LAPACK_sgeev
  #define SGELSY LAPACK_sgelsy
  #define SGESVD LAPACK_sgesvd
  #define SGESV LAPACK_sgesv
  #define SGETRF LAPACK_sgetrf
  #define SGETRI LAPACK_sgetri
  #define SSYEV LAPACK_ssyev
  #define SSYGV LAPACK_ssygv
#endif