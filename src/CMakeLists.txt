foreach(mod ${VegaFEM_Modules})
  add_subdirectory(lib${mod})
endforeach()

add_subdirectory(util/volumetricMeshUtilities)
if (VegaFEM_ENABLE_OpenGL_SUPPORT)
  add_subdirectory(util/interactiveDeformableSimulator)
endif()
