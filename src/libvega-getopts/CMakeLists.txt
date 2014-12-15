vega_add_library(vega-getopts
  SOURCES getopts.cpp)
target_include_directories(vega-getopts
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
