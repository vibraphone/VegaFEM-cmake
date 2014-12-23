vega_add_library(camera
  SOURCES camera.cpp
  PUBLIC_HEADERS
    camera.h
)
target_link_libraries(camera
  PUBLIC
    ${OPENGL_LIBRARIES}
)
