include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_CURRENT_SOURCE_DIR}/../libconfigFile
   ${CMAKE_SOURCE_DIR}/include
)

add_library(lighting SHARED lighting.cpp)
target_link_libraries(lighting configFile ${OPENGL_LIBRARIES})
