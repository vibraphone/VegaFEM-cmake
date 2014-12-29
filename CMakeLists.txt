# *******************************************************************
# ***                  VegaFEM CMakeLists.txt                     ***
# *******************************************************************

# This file contains the top level CMakeLists.txt logic for the
# Vega FEM software package.

project(VegaFEM)
set(VegaFEM_VERSION 2.0)

# Minimum required version of CMake
cmake_minimum_required(VERSION 2.8)
if(COMMAND CMAKE_POLICY)
  cmake_policy(SET CMP0003 NEW)
  if (POLICY CMP0042)
    cmake_policy(SET CMP0042 NEW)
  endif()
endif(COMMAND CMAKE_POLICY)

if(NOT IS_SUBBUILD)
  set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake;${CMAKE_MODULE_PATH};")
else(NOT IS_SUBBUILD)
  set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/CMake")
endif(NOT IS_SUBBUILD)

include(CMakePackageConfigHelpers)
include(VegaFEMMacros)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE OFF)
option(VegaFEM_BUILD_MODEL_REDUCTION "Should model-reduction techniques be included?" ON)
option(VegaFEM_ENABLE_ExpoKit_SUPPORT "Should matrix classes use ExpoKit for exponentiation?" OFF) 
# TODO: define USE_EXPOKIT if enabled
option(VegaFEM_ENABLE_OpenGL_SUPPORT "Should modules that require OpenGL be enabled?" ON)
option(VegaFEM_ENABLE_PTHREADS_SUPPORT "Use multithreading (pthread)" ON)
# TODO: add option for arpack++ and define USEARPACKPLUSPLUS
# TODO: add option for spooles and define SPOOLES_SOLVER_IS_AVAILABLE

if (VegaFEM_ENABLE_OpenGL_SUPPORT)
  # Only add Cg support if OpenGL is turned on. This may change in the future.
  option(VegaFEM_ENABLE_Cg_SUPPORT "Should modules that require Cg be enabled?" OFF)
endif()

#---------------------------------------------------------------------
# The following logic is what allows binaries to run successfully in
# the build directory AND install directory.  Thanks to plplot for
# identifying the necessity of setting CMAKE_INSTALL_NAME_DIR on OSX.
# Documentation of these options is available at
# http://www.cmake.org/Wiki/CMake_RPATH_handling

# use, i.e. don't skip the full RPATH for the build tree
set(CMAKE_SKIP_BUILD_RPATH  FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)

# the RPATH/INSTALL_NAME_DIR to be used when installing
if (NOT APPLE)
  set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib:\$ORIGIN/../lib")
endif(NOT APPLE)
# On OSX, we need to set INSTALL_NAME_DIR instead of RPATH
# http://www.cmake.org/cmake/help/cmake-2-8-docs.html#variable:CMAKE_INSTALL_NAME_DIR
set(CMAKE_INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib")

# add the automatically determined parts of the RPATH which point to
# directories outside the build tree to the install RPATH
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# build utilities
option(BUILD_UTILITIES "Build utility applications." ON)

# Set directory variables
if(NOT BIN_DIR)
  set(BIN_DIR bin)
endif(NOT BIN_DIR)

if(NOT LIB_DIR)
  set(LIB_DIR lib)
endif(NOT LIB_DIR)

# Output directories - this is where built library and executable
# files will be placed after building but prior to install.  The
# necessary variables change between single and multi configuration
# build systems, so it is necessary to handle both cases on a
# conditional basis.

if(NOT CMAKE_CONFIGURATION_TYPES)
  # If we're not doing multi-configuration, just set the three main
  # variables to the correct values.
  if(NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${VegaFEM_BINARY_DIR}/${LIB_DIR} CACHE INTERNAL "Single output directory for building all libraries.")
  endif(NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
  if(NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${VegaFEM_BINARY_DIR}/${LIB_DIR} CACHE INTERNAL "Single output directory for building all archives.")
  endif(NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
  if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${VegaFEM_BINARY_DIR}/${BIN_DIR} CACHE INTERNAL "Single output directory for building all executables.")
  endif(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
else(NOT CMAKE_CONFIGURATION_TYPES)
  # Multi-configuration is more difficult.  Not only do we need to
  # properly set the output directories, but we also need to
  # identify the "toplevel" directory for each configuration so
  # we can place files, documentation, etc. in the correct
  # relative positions.  Because files may be placed by CMake
  # without a build target to put them in their proper relative build
  # directory position using these paths, we must fully qualify them
  # without using CMAKE_CFG_INTDIR.
  #
  # We define directories that may not be quite "standard"
  # for a particular build tool - for example, native VS2010 projects use
  # another directory to denote CPU type being compiled for - but CMake only
  # supports multi-configuration setups having multiple configurations,
  # not multiple compilers.
  #
  # One additional wrinkle we must watch for here is the case where
  # a multi-configuration setup uses "." for its internal directory -
  # if that's the case, we need to just set the various config output
  # directories to the same value.
  set(CFG_ROOT ${VegaFEM_BINARY_DIR})
  foreach(CFG_TYPE ${CMAKE_CONFIGURATION_TYPES})
    if(NOT "${CMAKE_CFG_INTDIR}" STREQUAL ".")
      set(CFG_ROOT ${VegaFEM_BINARY_DIR}/${CFG_TYPE})
    endif(NOT "${CMAKE_CFG_INTDIR}" STREQUAL ".")
    string(TOUPPER "${CFG_TYPE}" CFG_TYPE_UPPER)
    if(NOT "CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CFG_TYPE_UPPER}")
      set("CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CFG_TYPE_UPPER}" ${CFG_ROOT}/${LIB_DIR} CACHE INTERNAL "Single output directory for building ${CFG_TYPE} libraries.")
    endif(NOT "CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CFG_TYPE_UPPER}")
    if(NOT "CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CFG_TYPE_UPPER}")
      set("CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CFG_TYPE_UPPER}" ${CFG_ROOT}/${LIB_DIR} CACHE INTERNAL "Single output directory for building ${CFG_TYPE} archives.")
    endif(NOT "CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CFG_TYPE_UPPER}")
    if(NOT "CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CFG_TYPE_UPPER}")
      set("CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CFG_TYPE_UPPER}" ${CFG_ROOT}/${BIN_DIR} CACHE INTERNAL "Single output directory for building ${CFG_TYPE} executables.")
    endif(NOT "CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CFG_TYPE_UPPER}")
    if(NOT "CMAKE_BINARY_DIR_${CFG_TYPE_UPPER}")
      set("CMAKE_BINARY_DIR_${CFG_TYPE_UPPER}" ${CFG_ROOT} CACHE INTERNAL "Toplevel binary dir for ${CFG_TYPE} building.")
    endif(NOT "CMAKE_BINARY_DIR_${CFG_TYPE_UPPER}")
    if(NOT "VegaFEM_BINARY_DIR_${CFG_TYPE_UPPER}")
      set("VegaFEM_BINARY_DIR_${CFG_TYPE_UPPER}" ${CFG_ROOT} CACHE INTERNAL "Toplevel binary dir for ${CFG_TYPE} building.")
    endif(NOT "VegaFEM_BINARY_DIR_${CFG_TYPE_UPPER}")
  endforeach()
endif(NOT CMAKE_CONFIGURATION_TYPES)


if (VegaFEM_ENABLE_OpenGL_SUPPORT)
  find_package(OpenGL)
  find_package(GLUT)
  find_package(GLEW)
  find_package(GLUI)
  if (GLUT_FOUND)
    mark_as_advanced(GLUT_cocoa_LIBRARY)
  endif()
  if (VegaFEM_ENABLE_Cg_SUPPORT)
    find_package(Cg REQUIRED)
    mark_as_advanced(
      CG_COMPILER
      CG_GL_LIBRARY
      CG_INCLUDE_PATH
      CG_LIBRARY
    )
  endif()
endif()

if(VegaFEM_ENABLE_PTHREADS_SUPPORT)
  #FindThreads
  if(WIN32)
    set(THREADS_USE_PTHREADS_WIN32 1)
  endif(WIN32)
  find_package(PThreads REQUIRED)
  if(THREADS_FOUND)
    include_directories(${THREADS_PTHREADS_INCLUDE_DIR})
  endif()
endif()

if (VegaFEM_BUILD_MODEL_REDUCTION)
  if (NOT APPLE)
    find_package(MKL)
    if(MKL_FOUND)
      set(BLA_VENDOR "Intel mkl")
      include_directories(${INTEL_MKL_INCLUDE_DIR})
      # TODO: set option for the paradiso solvers and define PARDISO_SOLVER_IS_AVAILABLE
    else()
      find_package(LAPACKE REQUIRED) # Also searches for cblas
      include_directories(${LAPACKE_INCLUDE_DIR})
      include_directories(${CBLAS_INCLUDE_DIR})
    endif() 
  endif()
  find_package(LAPACK) # Also searches for blas
endif()

if (VegaFEM_ENABLE_OpenGL_SUPPORT)
  if(NOT GLUI_INCLUDE_DIR OR NOT GLUI_LIBRARY)
    add_subdirectory(thirdparty/glui/2.36/src)
    set(GLUI_LIBRARY glui)
    set(GLUI_INCLUDE_DIR) # Empty this as library will export its own includes.
  endif()
endif()

set(VegaFEM_Modules
  minivector
  matrixIO
  sparseMatrix
  graph
  imageIO
  configFile
  integrator
  performanceCounter
  insertRows
  sparseSolver
  forceModel
  integratorSparse
  objMesh
  polarDecomposition
  volumetricMesh
  corotationalLinearFEM
  massSpringSystem
  stvk
  quaternion
  isotropicHyperelasticFEM
  elasticForceModel
  loadList
  vega-getopts
  clothBW
  hashTable
  rigidBodyDynamics
)
if (VegaFEM_BUILD_MODEL_REDUCTION)
  set(VegaFEM_Modules
    ${VegaFEM_Modules}
    integratorDense
    matrix
    modalMatrix
    reducedElasticForceModel
    reducedForceModel
    reducedStvk
  )
endif()
if (VegaFEM_ENABLE_OpenGL_SUPPORT)
  set(VegaFEM_Modules
    ${VegaFEM_Modules}
    camera
    lighting
    sceneObject
    glslPhong
  )
  if (VegaFEM_ENABLE_Cg_SUPPORT)
    set(VegaFEM_Modules
      ${VegaFEM_Modules}
      objMeshGPUDeformer
    )
  endif()
  if (VegaFEM_BUILD_MODEL_REDUCTION)
    set(VegaFEM_Modules
      ${VegaFEM_Modules}
      openGLHelper
      sceneObjectReduced
      renderVolumetricMesh
    )
  endif()
endif()

add_subdirectory(src)
export(PACKAGE VegaFEM)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/VegaFEM/VegaFEMConfigVersion.cmake"
  VERSION ${VegaFEM_VERSION}
  COMPATIBILITY AnyNewerVersion
)
configure_file(
  CMake/VegaFEMConfig.cmake
  "${CMAKE_CURRENT_BINARY_DIR}/VegaFEM/VegaFEMConfig.cmake"
  COPYONLY
)

set(ConfigPackageLocation lib/cmake/VegaFEM)
install(EXPORT VegaFEMTargets
  FILE
    VegaFEMTargets.cmake
    #NAMESPACE VegaFEM::
  DESTINATION
    ${ConfigPackageLocation}
)

install(
  FILES
    CMake/VegaFEMConfig.cmake
    "${CMAKE_CURRENT_BINARY_DIR}/VegaFEM/VegaFEMConfigVersion.cmake"
  DESTINATION
    ${ConfigPackageLocation}
  COMPONENT
    Devel
)

add_subdirectory(include)
