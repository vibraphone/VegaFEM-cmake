include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_CURRENT_SOURCE_DIR}/../libobjMesh
   ${CMAKE_CURRENT_SOURCE_DIR}/../liblighting
   ${CMAKE_CURRENT_SOURCE_DIR}/../libminivector
   ${CMAKE_SOURCE_DIR}/include
)

set(SCOBJ_SRCS
   sceneObjectDeformable.cpp
   sceneObject.cpp
   sceneObjectWithRestPosition.cpp
)

add_library(sceneObject SHARED ${SCOBJ_SRCS})
target_link_libraries(sceneObject objMesh lighting minivector)
