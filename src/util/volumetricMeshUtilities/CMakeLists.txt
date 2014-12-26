include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_CURRENT_SOURCE_DIR}/../../libminivector
   ${CMAKE_CURRENT_SOURCE_DIR}/../../libobjMesh
   ${CMAKE_CURRENT_SOURCE_DIR}/../../libmatrixIO
   ${CMAKE_CURRENT_SOURCE_DIR}/../../libvega-getopts
   ${CMAKE_CURRENT_SOURCE_DIR}/../../libloadList
   ${CMAKE_CURRENT_SOURCE_DIR}/../../libsparseMatrix
   ${CMAKE_CURRENT_SOURCE_DIR}/../../libvolumetricMesh
   ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set(VOLUTILS_LIBS volumetricMesh objMesh matrixIO vega-getopts loadList sparseMatrix)
set(VOLUTILS_LIBS ${VOLUTILS_LIBS} ${SPOOLES_LIBRARY} ${PARDISO_LIBRARY} ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY})

add_executable(generateMassMatrix generateMassMatrix.cpp)
target_link_libraries(generateMassMatrix ${VOLUTILS_LIBS})
add_executable(generateInterpolant generateInterpolant.cpp)
target_link_libraries(generateInterpolant ${VOLUTILS_LIBS})
add_executable(generateInterpolationMatrix generateInterpolationMatrix.cpp)
target_link_libraries(generateInterpolationMatrix ${VOLUTILS_LIBS})
add_executable(generateSurfaceMesh generateSurfaceMesh.cpp)
target_link_libraries(generateSurfaceMesh ${VOLUTILS_LIBS})
