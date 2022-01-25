cmake_minimum_required(VERSION 3.14)

project(locate-exampleTests LANGUAGES CXX)

include(../cmake/project-is-top-level.cmake)
include(../cmake/folders.cmake)
include(../cmake/windows-get-path.cmake)

if(PROJECT_IS_TOP_LEVEL)
  find_package(locate-example REQUIRED)
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
  "LOCATE_EXAMPLE_TEST_DATADIR=\"${LOCATE_EXAMPLE_DATA_DIR}\""
  "LOCATE_EXAMPLE_TEST_STATICFILE=\"${CMAKE_CURRENT_LIST_DIR}/data/static.txt\""
)

windows_get_path(win_path locate-example::locate-example)

include(Catch)
catch_discover_tests(
  locate-example_test
  PROPERTIES
    ENVIRONMENT "${win_path}"
)

add_folders(Test)
