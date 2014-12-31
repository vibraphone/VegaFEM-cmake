
# FindwxWidgets
find_package(wxWidgets)

if(wxWidgets_FOUND)
  include_directories(${wxWidgets_INCLUDE_DIRS})
  set(LARGE_MODAL_DEF_SRCS cubicPolynomials.cpp fixedVertices.cpp frequencies.cpp largeModalDeformationFactory.cpp linearModes.cpp main.cpp nonlinearModes.cpp view.cpp renderingMesh.cpp simulationMesh.cpp canvas.cpp modalDerivatives.cpp sketch.cpp interpolate.cpp convert.cpp runtime.cpp StVKReducedInternalForcesWX.cpp)
  add_executable(largeModalDeformationFactory ${LARGE_MODAL_DEF_SRCS})
  target_link_libraries(largeModalDeformationFactory reducedStvk stvk reducedElasticForceModel reducedForceModel forceModel renderVolumetricMesh sparseSolver sparseMatrix volumetricMesh objMesh imageIO modalMatrix matrix matrixIO getopts insertRows loadList camera minivector openGLHelper
  ${CMAKE_THREAD_LIBS_INIT}
  ${OPENGL_gl_LIBRARY} ${GLUT_glut_LIBRARY} ${OPENGL_glu_LIBRARY} ${GLUI_LIBRARY})
endif(wxWidgets_FOUND)
