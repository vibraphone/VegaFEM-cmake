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
  set(multiValueArgs SOURCES)
  if (NOT WIN32)
    set(libtype SHARED)
  else()
    set(libtype STATIC)
  endif()
  cmake_parse_arguments(target "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )
  add_library(${target} ${libtype}
    ${target_SOURCES}
  )
  vega_install_library(${target})
endfunction()
