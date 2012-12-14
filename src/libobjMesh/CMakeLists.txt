include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_CURRENT_SOURCE_DIR}/../libmatrix
   ${CMAKE_CURRENT_SOURCE_DIR}/../libminivector
   ${CMAKE_SOURCE_DIR}/include
)

set(OBJ_SRCS
   objMeshBinaryLoader.cpp
   objMesh.cpp
   objMeshRender.cpp
   objMeshEncode.cpp
)

add_library(objMesh SHARED ${OBJ_SRCS})
target_link_libraries(objMesh minivector matrix)
