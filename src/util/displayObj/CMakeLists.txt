
include_directories(
#     ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../libopenGLHelper
    ${CMAKE_CURRENT_SOURCE_DIR}/../../libperformanceCounter
    ${CMAKE_CURRENT_SOURCE_DIR}/../../libglslPhong
    ${CMAKE_CURRENT_SOURCE_DIR}/../../libobjMesh
    ${CMAKE_CURRENT_SOURCE_DIR}/../../libvega-getopts
    ${CMAKE_CURRENT_SOURCE_DIR}/../../libmatrixIO
    ${CMAKE_CURRENT_SOURCE_DIR}/../../libloadList
)

add_executable(displayObj displayObj.cpp)
target_include_directories(displayObj
  PUBLIC
    ${GLUI_INCLUDE_DIRS}
)
target_link_libraries(displayObj 
  vega-getopts
  objMesh 
  imageIO 
  glslPhong 
  camera 
  matrixIO 
  lighting 
  configFile 
  loadList
  ${OPENGL_gl_LIBRARY} 
  ${GLUT_glut_LIBRARY} 
  ${OPENGL_glu_LIBRARY} 
  ${GLUI_LIBRARY} 
  ${GLEW_LIBRARIES})

add_executable(objMergeFiles objMergeFiles.cpp)
target_include_directories(objMergeFiles
  PUBLIC
    ${GLUI_INCLUDE_DIRS}
)
target_link_libraries(objMergeFiles 
  vega-getopts
  objMesh 
  imageIO 
  glslPhong 
  camera 
  matrixIO 
  lighting 
  configFile 
  loadList
  ${OPENGL_gl_LIBRARY} 
  ${GLUT_glut_LIBRARY} 
  ${OPENGL_glu_LIBRARY} 
  ${GLUI_LIBRARY} 
  ${GLEW_LIBRARIES})
