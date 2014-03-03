include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_SOURCE_DIR}/include
)

set(clothbw_srcs
	clothBW.cpp
	clothBWFromObjMesh.cpp
	clothBWMT.cpp
)

add_library(clothbw SHARED ${clothbw_srcs})
target_link_libraries(clothbw minivector objMesh sparseMatrix)
