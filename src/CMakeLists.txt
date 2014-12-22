foreach(mod ${VegaFEM_Modules})
  add_subdirectory(lib${mod})
endforeach()

add_subdirectory(util/interactiveDeformableSimulator)
add_subdirectory(util/volumetricMeshUtilities)
