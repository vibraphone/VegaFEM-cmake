set(OBJ_SRCS
  objMesh.cpp
	objMesh-disjointSet.cpp
  objMeshBinaryLoader.cpp
  objMeshEncode.cpp
	objMeshGraph.cpp
	objMeshOrientable.cpp
	simpleSphere.cpp
	#triangle-closestPoint.cpp
	tribox3.cpp
)
if (VegaFEM_ENABLE_OpenGL_SUPPORT)
  set(OBJ_SRCS
    ${OBJ_SRCS}
	  boundingBox.cpp
	  octree.cpp
	  triangle.cpp
	  objMeshOctree.cpp
	  objMeshOffsetVoxels.cpp
    objMeshRender.cpp
  )
endif()

vega_add_library(objMesh
  SOURCES ${OBJ_SRCS}
)
target_include_directories(objMesh
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(objMesh
  PUBLIC
    minivector
    matrixIO
    imageIO
    sparseMatrix
    graph
)
if (VegaFEM_ENABLE_OpenGL_SUPPORT)
  target_link_libraries(objMesh
    PUBLIC
      ${OPENGL_gl_LIBRARY}
      ${OPENGL_glu_LIBRARY}
  )
endif()
