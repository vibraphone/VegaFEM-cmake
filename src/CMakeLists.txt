foreach(mod ${VegaFEM_Modules})
  add_subdirectory(lib${mod})
endforeach()

if(BUILD_UTILITIES)
  add_subdirectory(util)
endif(BUILD_UTILITIES)
