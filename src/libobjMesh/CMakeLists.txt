include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../libmatrixIO
	${CMAKE_CURRENT_SOURCE_DIR}/../libimageIO
	${CMAKE_CURRENT_SOURCE_DIR}/../libsparseMatrix
   ${CMAKE_CURRENT_SOURCE_DIR}/../libminivector
   ${CMAKE_CURRENT_SOURCE_DIR}/../libgraph
   ${CMAKE_SOURCE_DIR}/include
)

set(OBJ_SRCS
	boundingBox.cpp
   objMesh.cpp
	objMesh-disjointSet.cpp
   objMeshBinaryLoader.cpp
   objMeshEncode.cpp
	objMeshGraph.cpp
	objMeshOctree.cpp
	objMeshOffsetVoxels.cpp
	objMeshOrientable.cpp
   objMeshRender.cpp
	octree.cpp
	simpleSphere.cpp
	#triangle-closestPoint.cpp
	triangle.cpp
	tribox3.cpp
)

add_library(objMesh SHARED ${OBJ_SRCS})
target_link_libraries(objMesh minivector matrixIO imageIO sparseMatrix graph ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY})
