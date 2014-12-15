set(objmeshbgpudeformer_srcs
	objMeshGPUDeformer.cpp
	objMeshGPUDeformer_coarseToFine.cpp
	objMeshGPUDeformer_coarseToFine_fbo.cpp
	objMeshGPUDeformer_uUq.cpp
	objMeshGPUDeformer_uUq_fbo.cpp
	objMeshGPUDeformer_uUq_pbuffer.cpp
	objMeshGPUDeformer_uUq_setLighting.cpp
	vbo.cpp
	)

vega_add_library(objmeshbgpudeformer SOURCES ${objmeshbgpudeformer_srcs})
target_include_directories(objmeshbgpudeformer
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
  PUBLIC
	  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(objmeshbgpudeformer
  PUBLIC
    objMesh
    lighting
)
