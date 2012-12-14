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

add_library(ObjMesh SHARED ${OBJ_SRCS})
target_link_libraries(ObjMesh minivector matrix)
