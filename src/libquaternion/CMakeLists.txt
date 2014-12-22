vega_add_library(quaternion SOURCES quaternion.cpp)
target_include_directories(quaternion
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/libquaternion>
    $<INSTALL_INTERFACE:include>
)
