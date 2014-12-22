vega_add_library(glslPhong
  SOURCES glslPhong.cpp)
target_include_directories(glslPhong
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(glslPhong
  PUBLIC
    ${OPENGL_LIBRARIES}
)
