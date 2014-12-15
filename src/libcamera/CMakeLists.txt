vega_add_library(camera
  SOURCES camera.cpp)
target_include_directories(camera
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(camera
  PUBLIC
    ${OPENGL_LIBRARIES}
)
