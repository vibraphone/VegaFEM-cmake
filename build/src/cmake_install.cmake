# Install script for directory: /home/cyapp/vegafem/src

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  INCLUDE("/home/cyapp/vegafem/build/src/libconfigFile/cmake_install.cmake")
  INCLUDE("/home/cyapp/vegafem/build/src/libgetopts/cmake_install.cmake")
  INCLUDE("/home/cyapp/vegafem/build/src/libloadList/cmake_install.cmake")
  INCLUDE("/home/cyapp/vegafem/build/src/libcamera/cmake_install.cmake")
  INCLUDE("/home/cyapp/vegafem/build/src/libperformanceCounter/cmake_install.cmake")
  INCLUDE("/home/cyapp/vegafem/build/src/libminivector/cmake_install.cmake")
  INCLUDE("/home/cyapp/vegafem/build/src/libinsertRows/cmake_install.cmake")
  INCLUDE("/home/cyapp/vegafem/build/src/libmatrix/cmake_install.cmake")
  INCLUDE("/home/cyapp/vegafem/build/src/libsparseMatrix/cmake_install.cmake")
  INCLUDE("/home/cyapp/vegafem/build/src/libsparseSolver/cmake_install.cmake")
  INCLUDE("/home/cyapp/vegafem/build/src/liblighting/cmake_install.cmake")
  INCLUDE("/home/cyapp/vegafem/build/src/libgraph/cmake_install.cmake")
  INCLUDE("/home/cyapp/vegafem/build/src/libforceModel/cmake_install.cmake")
  INCLUDE("/home/cyapp/vegafem/build/src/libpolarDecomposition/cmake_install.cmake")
  INCLUDE("/home/cyapp/vegafem/build/src/libobjMesh/cmake_install.cmake")
  INCLUDE("/home/cyapp/vegafem/build/src/libsceneObject/cmake_install.cmake")
  INCLUDE("/home/cyapp/vegafem/build/src/libvolumetricMesh/cmake_install.cmake")
  INCLUDE("/home/cyapp/vegafem/build/src/libstvk/cmake_install.cmake")
  INCLUDE("/home/cyapp/vegafem/build/src/libmassSpringSystem/cmake_install.cmake")
  INCLUDE("/home/cyapp/vegafem/build/src/libisotropicHyperelasticFEM/cmake_install.cmake")
  INCLUDE("/home/cyapp/vegafem/build/src/libcorotationalLinearFEM/cmake_install.cmake")
  INCLUDE("/home/cyapp/vegafem/build/src/libelasticForceModel/cmake_install.cmake")
  INCLUDE("/home/cyapp/vegafem/build/src/libintegrator/cmake_install.cmake")
  INCLUDE("/home/cyapp/vegafem/build/src/util/interactiveDeformableSimulator/cmake_install.cmake")
  INCLUDE("/home/cyapp/vegafem/build/src/util/volumetricMeshUtilities/cmake_install.cmake")

ENDIF(NOT CMAKE_INSTALL_LOCAL_ONLY)

