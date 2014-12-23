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
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
	  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    ${OPENGL_INCLUDE_DIR}
    ${CG_INCLUDE_PATH}
)
if (APPLE)
  # The nVidia installer puts the framework in /Library/Frameworks/Cg.framework
  # but modern Xcode compilers expect frameworks to live in the SDK directory
  # if one is specified... so add framework search paths to fix this:
  target_compile_options(objmeshbgpudeformer
    PUBLIC
      -iframework "${Cg_FRAMEWORKS}/.."
  )
  set_target_properties(objmeshbgpudeformer
    PROPERTIES
    LINK_FLAGS "-F ${Cg_FRAMEWORKS}/.."
  )
endif()
target_link_libraries(objmeshbgpudeformer
  PUBLIC
    objMesh
    lighting
    ${OPENGL_LIBRARIES}
    ${CG_LIBRARY}
)
