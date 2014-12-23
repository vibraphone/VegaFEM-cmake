set(clothbw_srcs
	clothBW.cpp
	clothBWFromObjMesh.cpp
	clothBWMT.cpp
)

vega_add_library(clothbw
  SOURCES ${clothbw_srcs})
target_include_directories(clothbw
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(clothbw
  PUBLIC
    minivector
    objMesh
    sparseMatrix
)
