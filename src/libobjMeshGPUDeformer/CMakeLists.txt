vega_add_library(objMeshGPUDeformer
  SOURCES
    objMeshGPUDeformer.cpp
    objMeshGPUDeformer_coarseToFine.cpp
    objMeshGPUDeformer_coarseToFine_fbo.cpp
    objMeshGPUDeformer_uUq.cpp
    objMeshGPUDeformer_uUq_fbo.cpp
    objMeshGPUDeformer_uUq_pbuffer.cpp
    objMeshGPUDeformer_uUq_setLighting.cpp
    vbo.cpp
  PUBLIC_HEADERS
    glh_extensions.h
    glh_genext.h
    objMeshGPUDeformer.h
    objMeshGPUDeformer_coarseToFine.h
    objMeshGPUDeformer_coarseToFine_fbo.h
    objMeshGPUDeformer_uUq.h
    objMeshGPUDeformer_uUq_fbo.h
    objMeshGPUDeformer_uUq_pbuffer.h
    objMeshGPUDeformer_uUq_setLighting.h
    vbo.h
)
target_include_directories(objMeshGPUDeformer
  PUBLIC
    ${OPENGL_INCLUDE_DIR}
    ${CG_INCLUDE_PATH}
)
if (APPLE)
  # The nVidia installer puts the framework in /Library/Frameworks/Cg.framework
  # but modern Xcode compilers expect frameworks to live in the SDK directory
  # if one is specified... so add framework search paths to fix this:
  target_compile_options(objMeshGPUDeformer
    PUBLIC
      -iframework "${Cg_FRAMEWORKS}/.."
  )
  set_target_properties(objMeshGPUDeformer
    PROPERTIES
    LINK_FLAGS "-F ${Cg_FRAMEWORKS}/.."
  )
endif()
target_link_libraries(objMeshGPUDeformer
  PUBLIC
    objMesh
    lighting
    ${OPENGL_LIBRARIES}
    ${CG_LIBRARY}
)
