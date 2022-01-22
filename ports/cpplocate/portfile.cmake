vcpkg_from_github(
  OUT_SOURCE_PATH SOURCE_PATH
  REPO cginternals/cpplocate
  REF v2.2.0
  SHA512 b5ea01ef03cd736ed867893d7ae499caaacf62881a206682b3444193e8e164bd69a361aac7c0ac323a656b8d608952cd6e0694f10d064befefe996069811cd9d
  HEAD_REF master
  PATCHES
    remove-global-install.patch
    fix-install-paths.patch
)

vcpkg_cmake_configure(
  SOURCE_PATH ${SOURCE_PATH}
  OPTIONS
    -DOPTION_BUILD_TESTS=OFF
)

vcpkg_cmake_install()

vcpkg_cmake_config_fixup(
  PACKAGE_NAME cpplocate
  CONFIG_PATH cmake/cpplocate
  DO_NOT_DELETE_PARENT_CONFIG_PATH
)
vcpkg_cmake_config_fixup(
  PACKAGE_NAME liblocate
  CONFIG_PATH cmake/liblocate
)

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/share")

file(INSTALL "${SOURCE_PATH}/LICENSE" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}" RENAME copyright)
file(INSTALL "${SOURCE_PATH}/LICENSE" DESTINATION "${CURRENT_PACKAGES_DIR}/share/liblocate" RENAME copyright)

# Write new config files
file(WRITE "${CURRENT_PACKAGES_DIR}/share/${PORT}/${PORT}Config.cmake" "include(\${CMAKE_CURRENT_LIST_DIR}/${PORT}-export.cmake)")
file(WRITE "${CURRENT_PACKAGES_DIR}/share/liblocate/liblocateConfig.cmake" "include(\${CMAKE_CURRENT_LIST_DIR}/liblocate-export.cmake)")
