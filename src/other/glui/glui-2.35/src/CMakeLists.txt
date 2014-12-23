if (NOT APPLE)
  find_package(X11)
endif()

find_package(GLUT)

#LIBGLUI = -L./lib -lglui
#LIBGL   = -lGLU -lGL
#LIBS    = -lXmu -lXext -lX11 -lXi -lm

# One of the following options only...

# (1) OpenGLUT
# LIBGLUT   = -L/usr/X11R6/lib -lopenglut
# CPPFLAGS += -I/usr/X11R6/include -DGLUI_OPENGLUT

# (2) FreeGLUT
# LIBGLUT   = -L/usr/X11R6/lib -lfreeglut
# CPPFLAGS += -I/usr/X11R6/include -DGLUI_FREEGLUT

#CPPFLAGS += -I/usr/X11R6/include

#######################################

set(GLUI_SRCS
  algebra3.cpp
  arcball.cpp
  glui.cpp
  glui_add_controls.cpp
  glui_bitmap_img_data.cpp
  glui_bitmaps.cpp
  glui_button.cpp
  glui_checkbox.cpp
  glui_column.cpp
  glui_commandline.cpp
  glui_control.cpp
  glui_edittext.cpp
  glui_filebrowser.cpp
  glui_list.cpp
  glui_listbox.cpp
  glui_mouse_iaction.cpp
  glui_node.cpp
  glui_panel.cpp
  glui_radio.cpp
  glui_rollout.cpp
  glui_rotation.cpp
  glui_scrollbar.cpp
  glui_separator.cpp
  glui_spinner.cpp
  glui_statictext.cpp
  glui_string.cpp
  glui_textbox.cpp
  glui_translation.cpp
  glui_tree.cpp
  glui_treepanel.cpp
  glui_window.cpp
  quaternion.cpp
  viewmodel.cpp
)

vega_add_library(glui
  SOURCES ${GLUI_SRCS}
)
target_include_directories(glui
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(glui
  PUBLIC
    ${GLUT_LIBRARY}
    ${OPENGL_LIBRARIES}
)
