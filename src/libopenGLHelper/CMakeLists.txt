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
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(openglhelper
  PUBLIC
    matrixIO
    imageIO
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARY}
)
