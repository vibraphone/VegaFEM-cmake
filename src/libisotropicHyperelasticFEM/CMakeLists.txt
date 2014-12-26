set(isotropichyperelasticfem_srcs     
    isotropicMaterial.cpp
    MooneyRivlinIsotropicMaterial.cpp
    neoHookeanIsotropicMaterial.cpp
    StVKIsotropicMaterial.cpp
    homogeneousMooneyRivlinIsotropicMaterial.cpp
    homogeneousStVKIsotropicMaterial.cpp
    homogeneousNeoHookeanIsotropicMaterial.cpp
    isotropicHyperelasticFEM.cpp
    isotropicMaterialWithCompressionResistance.cpp
    )
    
set(isotropichyperelasticfem_hdrs 
    MooneyRivlinIsotropicMaterial.h
    StVKIsotropicMaterial.h
    homogeneousMooneyRivlinIsotropicMaterial.h
    homogeneousNeoHookeanIsotropicMaterial.h
    homogeneousStVKIsotropicMaterial.h
    isotropicHyperelasticFEM.h
    isotropicMaterial.h
    isotropicMaterialWithCompressionResistance.h
    neoHookeanIsotropicMaterial.h
    )

    
if(VegaFEM_ENABLE_PTHREADS_SUPPORT)
  list(APPEND isotropichyperelasticfem_hdrs isotropicHyperelasticFEMMT.h)
  list(APPEND isotropichyperelasticfem_srcs isotropicHyperelasticFEMMT.cpp)
endif()

vega_add_library(isotropicHyperelasticFEM
  SOURCES
    ${isotropichyperelasticfem_srcs}
  PUBLIC_HEADERS
    ${isotropichyperelasticfem_hdrs}
)
target_link_libraries(isotropicHyperelasticFEM
  PUBLIC
    minivector
    volumetricMesh
    sparseMatrix
  INTERFACE
    ${CMAKE_THREAD_LIBS_INIT}
)
