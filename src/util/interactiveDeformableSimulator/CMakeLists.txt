include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_CURRENT_SOURCE_DIR}/../../libcamera
   ${CMAKE_CURRENT_SOURCE_DIR}/../../libconfigFile
   ${CMAKE_CURRENT_SOURCE_DIR}/../../libcorotationalLinearFEM
   ${CMAKE_CURRENT_SOURCE_DIR}/../../libelasticForceModel
   ${CMAKE_CURRENT_SOURCE_DIR}/../../libforceModel
   ${CMAKE_CURRENT_SOURCE_DIR}/../../libgetopts
   ${CMAKE_CURRENT_SOURCE_DIR}/../../libgraph
   ${CMAKE_CURRENT_SOURCE_DIR}/../../libinsertRows
   ${CMAKE_CURRENT_SOURCE_DIR}/../../libintegrator
   ${CMAKE_CURRENT_SOURCE_DIR}/../../libisotropicHyperelasticFEM
   ${CMAKE_CURRENT_SOURCE_DIR}/../../liblighting
   ${CMAKE_CURRENT_SOURCE_DIR}/../../libloadList
   ${CMAKE_CURRENT_SOURCE_DIR}/../../libmassSpringSystem
   ${CMAKE_CURRENT_SOURCE_DIR}/../../libmatrix
   ${CMAKE_CURRENT_SOURCE_DIR}/../../libminivector
   ${CMAKE_CURRENT_SOURCE_DIR}/../../libobjMesh
   ${CMAKE_CURRENT_SOURCE_DIR}/../../libperformanceCounter
   ${CMAKE_CURRENT_SOURCE_DIR}/../../libpolarDecomposition
   ${CMAKE_CURRENT_SOURCE_DIR}/../../libsceneObject
   ${CMAKE_CURRENT_SOURCE_DIR}/../../libsparseMatrix
   ${CMAKE_CURRENT_SOURCE_DIR}/../../libsparseSolver
   ${CMAKE_CURRENT_SOURCE_DIR}/../../libstvk
   ${CMAKE_CURRENT_SOURCE_DIR}/../../libvolumetricMesh
   ${CMAKE_SOURCE_DIR}/include
)

add_executable(simulator interactiveDeformableSimulator.cpp initGraphics.cpp)
target_link_libraries(simulator sceneObject integrator elasticForceModel forceModel 
matrix sparseMatrix loadList insertRows lighting configFile volumetricMesh getopts 
camera graph isotropicHyperelasticFEM minivector stvk corotationalLinearFEM 
polarDecomposition massSpringSystem objMesh sparseSolver 
${OPENGL_gl_LIBRARY} ${GLUT_glut_LIBRARY} ${OPENGL_glu_LIBRARY} ${GLUI_LIBRARY})

