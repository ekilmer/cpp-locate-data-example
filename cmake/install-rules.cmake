if(PROJECT_IS_TOP_LEVEL)
  set(CMAKE_INSTALL_INCLUDEDIR include/locate-example CACHE PATH "")
endif()

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# find_package(<package>) call for consumers to find this project
set(package locate-example)

install(
    DIRECTORY
    include/
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

write_basic_package_version_file(
    "${package}ConfigVersion.cmake"
    COMPATIBILITY SameMajorVersion
)

# Allow package maintainers to freely override the path for the configs
set(
    locate-example_INSTALL_CMAKEDIR "${CMAKE_INSTALL_DATADIR}/${package}"
    CACHE PATH "CMake package config location relative to the install prefix"
)
mark_as_advanced(locate-example_INSTALL_CMAKEDIR)

install(
    FILES cmake/install-config.cmake
    DESTINATION "${locate-example_INSTALL_CMAKEDIR}"
    RENAME "${package}Config.cmake"
    COMPONENT locate-example_Development
)

install(
    FILES "${PROJECT_BINARY_DIR}/${package}ConfigVersion.cmake"
    DESTINATION "${locate-example_INSTALL_CMAKEDIR}"
    COMPONENT locate-example_Development
)

install(
    EXPORT locate-exampleTargets
    NAMESPACE locate-example::
    DESTINATION "${locate-example_INSTALL_CMAKEDIR}"
    COMPONENT locate-example_Development
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
