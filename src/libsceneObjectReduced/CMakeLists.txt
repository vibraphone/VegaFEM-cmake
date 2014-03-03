include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/include
	)

set(sceneobjreduced_srcs
	sceneObjectReduced.cpp
	sceneObjectReduced6DOF.cpp
	sceneObjectReducedCPU.cpp
	sceneObjectReducedCPU6DOF.cpp
	)
if(CG_LIBS)
	set(sceneobjreduced_srcs
		${sceneobjreduced_srcs}
		sceneObjectReducedGPU.cpp
		sceneObjectReducedGPU6DOF.cpp
		)
endif(CG_LIBS)

add_library(sceneObjectReduced SHARED ${sceneobjreduced_srcs})
target_link_libraries(sceneObjectReduced objMesh lighting minivector modalMatrix sceneObject)
if(CG_LIBS)
	target_link_libraries(sceneObjectReduced objMeshGPUDeformer)
endif(CG_LIBS)

