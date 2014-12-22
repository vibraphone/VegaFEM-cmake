# *******************************************************************
# ***                  VegaFEM CMakeLists.txt                     ***
# *******************************************************************

# This file contains the top level CMakeLists.txt logic for the
# Vega FEM software package.

project(VEGAFEM)
set(VegaFEM_VERSION 2.0)

# Minimum required version of CMake
cmake_minimum_required(VERSION 2.8)
if(COMMAND CMAKE_POLICY)
  CMAKE_POLICY(SET CMP0003 NEW)
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
option(VEGA_BUILD_MODEL_REDUCTION "Should model-reduction techniques be included?" ON)
option(VEGA_ENABLE_CUDA_SUPPORT "Should modules that require CUDA be enabled?" OFF)

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
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${VEGAFEM_BINARY_DIR}/${LIB_DIR} CACHE INTERNAL "Single output directory for building all libraries.")
  endif(NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
  if(NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${VEGAFEM_BINARY_DIR}/${LIB_DIR} CACHE INTERNAL "Single output directory for building all archives.")
  endif(NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
  if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${VEGAFEM_BINARY_DIR}/${BIN_DIR} CACHE INTERNAL "Single output directory for building all executables.")
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
  set(CFG_ROOT ${VEGAFEM_BINARY_DIR})
  foreach(CFG_TYPE ${CMAKE_CONFIGURATION_TYPES})
    if(NOT "${CMAKE_CFG_INTDIR}" STREQUAL ".")
      set(CFG_ROOT ${VEGAFEM_BINARY_DIR}/${CFG_TYPE})
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
    if(NOT "VEGAFEM_BINARY_DIR_${CFG_TYPE_UPPER}")
      set("VEGAFEM_BINARY_DIR_${CFG_TYPE_UPPER}" ${CFG_ROOT} CACHE INTERNAL "Toplevel binary dir for ${CFG_TYPE} building.")
    endif(NOT "VEGAFEM_BINARY_DIR_${CFG_TYPE_UPPER}")
  endforeach()
endif(NOT CMAKE_CONFIGURATION_TYPES)


find_package(OpenGL)
find_package(GLUT)
find_package(GLUI)
if (VEGA_BUILD_MODEL_REDUCTION)
  find_package(BLAS REQUIRED)
  find_package(LAPACK REQUIRED)
  if (VEGA_ENABLE_CUDA_SUPPORT)
    find_package(CUDA REQUIRED)
  endif()
endif()

if(NOT GLUI_INCLUDE_DIR OR NOT GLUI_LIBRARY)
  add_subdirectory(src/other/glui/glui-2.35/src)
  set(GLUI_LIBRARY glui)
  set(GLUI_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/other/glui/glui-2.35/src/include)
endif(NOT GLUI_INCLUDE_DIR OR NOT GLUI_LIBRARY)

set(VegaFEM_Modules
  minivector
  matrixIO
  sparseMatrix
  graph
  imageIO
  objMesh
  configFile
  lighting
  sceneObject
  integrator
  performanceCounter
  insertRows
  sparseSolver
  forceModel
  integratorSparse
  polarDecomposition
  volumetricMesh
  corotationalLinearFEM
  massSpringSystem
  stvk
  isotropicHyperelasticFEM
  elasticForceModel
  loadList
  vega-getopts
  camera
)
if (VEGA_BUILD_MODEL_REDUCTION)
  set(VegaFEM_Modules
    ${VegaFEM_Modules}
	  hashTable
	  quaternion
	  glslPhong
	  matrix
	  openGLHelper
	  modalMatrix
	  reducedForceModel
	  sceneObjectReduced
	  renderVolumetricMesh
	  reducedStvk
	  reducedElasticForceModel
	  integratorDense
	  clothBW
	  rigidBodyDynamics
  )
  if (VEGA_ENABLE_CUDA_SUPPORT)
    set(VegaFEM_Modules
      ${VegaFEM_Modules}
	    objMeshGPUDeformer
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

# Local Variables:
# tab-width: 8
# mode: cmake
# indent-tabs-mode: t
# End:
# ex: shiftwidth=2 tabstop=8
