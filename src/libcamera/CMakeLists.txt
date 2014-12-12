include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_SOURCE_DIR}/include
)

add_library(camera SHARED camera.cpp)
target_link_libraries(camera ${OPENGL_LIBRARIES})
