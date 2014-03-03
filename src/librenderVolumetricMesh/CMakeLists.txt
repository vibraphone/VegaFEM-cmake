include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/include
	)

add_library(renderVolumetricMesh SHARED renderVolumetricMesh.cpp)
target_link_libraries(renderVolumetricMesh volumetricMesh openGLHelper)
