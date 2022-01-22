cmake_minimum_required(VERSION 3.14)

project(locate-exampleTests LANGUAGES CXX)

include(../cmake/project-is-top-level.cmake)
include(../cmake/folders.cmake)
include(../cmake/windows-set-path.cmake)

if(PROJECT_IS_TOP_LEVEL)
  find_package(locate-example REQUIRED)
  enable_testing()
endif()

add_executable(locate-example_test source/locate-example_test.cpp)
target_link_libraries(locate-example_test PRIVATE locate-example::locate-example)
target_compile_features(locate-example_test PRIVATE cxx_std_17)

add_test(NAME locate-example_test COMMAND locate-example_test)
windows_set_path(locate-example_test locate-example::locate-example)

add_folders(Test)
