# Set necessary multi-config target properties, which will avoid overwriting with
# the same name.
# This should be called on the target(s) that are responsible for doing data lookups
function(set_multiconfig_target_properties TARGET)
  if (NOT TARGET "${TARGET}")
    message(FATAL "The passed target is not a target: ${TARGET}")
  endif()

  # NOTE: These are all supported multi-config generator configurations. We
  # list these here because we also need to define suffixes for them to live
  # in the same directories... Not sure if there's a better way
  set(build_configs  "" "RELEASE" "DEBUG" "RELWITHDEBINFO" "MINSIZEREL")
  set(config_postfix "" ""        "_d"    "_rwdi"          "_msr")

  # Do some list math to prepare for zipping...
  list(LENGTH build_configs num_configs)
  math(EXPR range_configs "${num_configs} - 1")

  # Zip the lists
  foreach(idx RANGE ${range_configs})
    list(GET build_configs ${idx} config)
    list(GET config_postfix ${idx} postfix)

    # Need to add underscores in some positions because we need to handle the
    # default empty string config
    set(u_config "_${config}")
    set(config_u "${config}_")
    if(config STREQUAL "")
      set(u_config "")
      set(config_u "")
    endif()

    # Mirror installation layout in build directory so that we can find the data
    # files using relative paths no matter which config we use
    set_target_properties(
      "${TARGET}" PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY${u_config} "${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}"
      ARCHIVE_OUTPUT_DIRECTORY${u_config} "${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}"
      LIBRARY_OUTPUT_DIRECTORY${u_config} "${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}"
      ${config_u}POSTFIX "${postfix}"
    )
  endforeach()
endfunction()