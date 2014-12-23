vega_add_library(lighting
  SOURCES lighting.cpp)
target_include_directories(lighting
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(lighting
  PUBLIC
    configFile
    ${OPENGL_LIBRARIES}
)
