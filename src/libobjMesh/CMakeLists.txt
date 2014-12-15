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

vega_add_library(objMesh
  SOURCES ${OBJ_SRCS})
target_include_directories(objMesh
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
	  ${CMAKE_CURRENT_SOURCE_DIR}/../libmatrixIO
	  ${CMAKE_CURRENT_SOURCE_DIR}/../libimageIO
	  ${CMAKE_CURRENT_SOURCE_DIR}/../libsparseMatrix
    ${CMAKE_CURRENT_SOURCE_DIR}/../libminivector
    ${CMAKE_CURRENT_SOURCE_DIR}/../libgraph
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(objMesh minivector matrixIO imageIO sparseMatrix graph ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY})
