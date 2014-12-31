set(clothbw_srcs
  clothBW.cpp
  clothBWFromObjMesh.cpp
)

set(clothbw_hdrs
  clothBW.h
  clothBWFromObjMesh.h
)

if(VegaFEM_ENABLE_PTHREADS_SUPPORT)
  list(APPEND clothbw_hdrs clothBWMT.h)
  list(APPEND clothbw_srcs clothBWMT.cpp)
endif()

vega_add_library(clothBW
  SOURCES
    ${clothbw_srcs}
  PUBLIC_HEADERS
    ${clothbw_hdrs}
)

target_link_libraries(clothBW
  PUBLIC
    objMesh
    minivector
    sparseMatrix
  INTERFACE
    ${CMAKE_THREAD_LIBS_INIT}
)
