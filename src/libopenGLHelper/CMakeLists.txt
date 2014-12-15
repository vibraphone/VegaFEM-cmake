set(openglhelper_srcs
  openGLHelper.cpp
  printBitmap.cpp
  saveScreenShot.cpp
)

vega_add_library(openglhelper SOURCES ${openglhelper_srcs})
target_include_directories(openglhelper
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    ${CMAKE_CURRENT_SOURCE_DIR}/../libmatrixIO
)
target_link_libraries(openglhelper matrixIO imageIO)
