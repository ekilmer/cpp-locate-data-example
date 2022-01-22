cmake_minimum_required(VERSION 3.14)

project(locate-exampleTests LANGUAGES CXX)

include(../cmake/project-is-top-level.cmake)
include(../cmake/folders.cmake)
include(../cmake/windows-set-path.cmake)

set(locate_example_datadir "${locate-example_BINARY_DIR}/${CMAKE_INSTALL_DATADIR}")
if(PROJECT_IS_TOP_LEVEL)
  find_package(locate-example REQUIRED)
  set(locate_example_datadir "${LOCATE_EXAMPLE_DATA_DIR}")
  enable_testing()
endif()

find_package(Catch2 REQUIRED)

add_executable(locate-example_test source/locate-example_test.cpp)
target_link_libraries(locate-example_test PRIVATE
  locate-example::locate-example
  Catch2::Catch2
)
target_compile_features(locate-example_test PRIVATE cxx_std_17)
target_compile_definitions(locate-example_test PRIVATE
  "LOCATE_EXAMPLE_TEST_DATADIR=\"${locate_example_datadir}\""
  "LOCATE_EXAMPLE_TEST_STATICFILE=\"${CMAKE_CURRENT_LIST_DIR}/data/static.txt\""
)
# NOTE: Need to add these postfixes to executables for every configuration that
# ends up in the same folder
set_target_properties(locate-example_test PROPERTIES
  RELEASE_POSTFIX "${CMAKE_RELEASE_POSTFIX}"
  DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}"
  RELWITHDEBINFO_POSTFIX "${CMAKE_RELWITHDEBINFO_POSTFIX}"
  MINSIZEREL_POSTFIX "${CMAKE_MINSIZEREL_POSTFIX}"
)

windows_set_path(locate-example_test locate-example::locate-example)

include(Catch)
catch_discover_tests(locate-example_test)

add_folders(Test)
