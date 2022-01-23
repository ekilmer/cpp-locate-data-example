if(PROJECT_IS_TOP_LEVEL)
  set(CMAKE_INSTALL_INCLUDEDIR include/locate-example CACHE PATH "")
endif()

include(CMakePackageConfigHelpers)

# find_package(<package>) call for consumers to find this project
set(package locate-example)

install(
    DIRECTORY
    include/
    "${PROJECT_BINARY_DIR}/include/"
    "${PROJECT_BINARY_DIR}/export/"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    COMPONENT locate-example_Development
)

install(
    TARGETS locate-example_locate-example
    EXPORT locate-exampleTargets
    RUNTIME #
    COMPONENT locate-example_Runtime
    LIBRARY #
    COMPONENT locate-example_Runtime
    NAMELINK_COMPONENT locate-example_Development
    ARCHIVE #
    COMPONENT locate-example_Development
    INCLUDES #
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

# Allow package maintainers to freely override the path for the configs
set(
    locate-example_INSTALL_CMAKEDIR "${CMAKE_INSTALL_DATAROOTDIR}/${package}"
    CACHE PATH "CMake package config location relative to the install prefix"
)
mark_as_advanced(locate-example_INSTALL_CMAKEDIR)

write_basic_package_version_file(
  "${PROJECT_BINARY_DIR}/${locate-example_INSTALL_CMAKEDIR}/${package}ConfigVersion.cmake"
  COMPATIBILITY SameMajorVersion
)

configure_package_config_file(cmake/install-config.cmake.in
  "${PROJECT_BINARY_DIR}/${locate-example_INSTALL_CMAKEDIR}/${package}Config.cmake"
  INSTALL_DESTINATION "${locate-example_INSTALL_CMAKEDIR}"
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
  PATH_VARS CMAKE_INSTALL_DATADIR
)

install(
  FILES
  "${PROJECT_BINARY_DIR}/${locate-example_INSTALL_CMAKEDIR}/${package}Config.cmake"
  "${PROJECT_BINARY_DIR}/${locate-example_INSTALL_CMAKEDIR}/${package}ConfigVersion.cmake"
  DESTINATION "${locate-example_INSTALL_CMAKEDIR}"
  COMPONENT locate-example_Development
)

install(
  EXPORT locate-exampleTargets
  NAMESPACE locate-example::
  DESTINATION "${locate-example_INSTALL_CMAKEDIR}"
  COMPONENT locate-example_Development
)

# Install our data files. NOTE: Only looks at the build directory because we've
# already had to copy all data files here
install(
  FILES
  "${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_DATADIR}/static.txt"
  "${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_DATADIR}/dynamic.txt"
  DESTINATION "${CMAKE_INSTALL_DATADIR}"
  COMPONENT locate-example_Runtime
)

# Make the build directory importable
export(
  EXPORT locate-exampleTargets
  NAMESPACE locate-example::
  FILE "${PROJECT_BINARY_DIR}/${locate-example_INSTALL_CMAKEDIR}/${package}Targets.cmake"
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
