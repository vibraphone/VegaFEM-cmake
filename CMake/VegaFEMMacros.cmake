function(vega_install_library target)
  set(options)
  set(oneValueArgs)
  set(multiValueArgs DEPENDS)
  cmake_parse_arguments(target "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )
  install(TARGETS ${target}
    EXPORT VegaFEMTargets
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    INCLUDES DESTINATION include
  )
  export(PACKAGE ${target})
  export(TARGETS ${target} ${target_DEPENDS} APPEND FILE ${target}-exports.cmake)
endfunction()

function(vega_add_library target)
  set(options)
  set(oneValueArgs)
  set(multiValueArgs SOURCES PUBLIC_HEADERS)
  if (NOT WIN32)
    set(libtype SHARED)
  else()
    set(libtype STATIC)
  endif()
  cmake_parse_arguments(target "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )
  add_library(${target} ${libtype}
    ${target_SOURCES}
  )
  target_include_directories(${target}
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
      $<INSTALL_INTERFACE:include/vega>
      $<INSTALL_INTERFACE:include/vega/${target}>
    )
  if (target_PUBLIC_HEADERS)
    vega_install_library(${target})
    install(FILES ${target_PUBLIC_HEADERS}
      DESTINATION include/vega/${target}
      COMPONENT Development
    )
  endif()
endfunction()
