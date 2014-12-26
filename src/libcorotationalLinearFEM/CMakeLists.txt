set(corotationallinearfem_srcs 
    corotationalLinearFEM.cpp)
    
set(corotationallinearfem_hdrs 
    corotationalLinearFEM.h)
    
if(VegaFEM_ENABLE_PTHREADS_SUPPORT)
  list(APPEND corotationallinearfem_hdrs corotationalLinearFEMMT.h)
  list(APPEND corotationallinearfem_srcs corotationalLinearFEMMT.cpp)
endif()

vega_add_library(corotationalLinearFEM
  SOURCES
    ${corotationallinearfem_srcs}
  PUBLIC_HEADERS
    ${corotationallinearfem_hdrs}
)

target_link_libraries(corotationalLinearFEM
  PUBLIC
    polarDecomposition
    volumetricMesh
    sparseMatrix
  INTERFACE
    minivector
    ${CMAKE_THREAD_LIBS_INIT}
)
