vega_add_library(renderVolumetricMesh SOURCES renderVolumetricMesh.cpp)
target_include_directories(renderVolumetricMesh
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(renderVolumetricMesh
  PUBLIC
    volumetricMesh
    openglhelper
)
