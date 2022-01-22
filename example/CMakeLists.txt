cmake_minimum_required(VERSION 3.14)

project(locate-exampleExamples CXX)

include(../cmake/project-is-top-level.cmake)
include(../cmake/folders.cmake)

if(PROJECT_IS_TOP_LEVEL)
  find_package(locate-example REQUIRED)
endif()

add_custom_target(run-examples)

function(add_example NAME)
  add_executable("${NAME}" "${NAME}.cpp")
  target_link_libraries("${NAME}" PRIVATE locate-example::locate-example)
  target_compile_features("${NAME}" PRIVATE cxx_std_17)
  # NOTE: Need to add these postfixes to executables for every configuration
  # that ends up in the same folder
  set_target_properties("${NAME}" PROPERTIES
    RELEASE_POSTFIX "${CMAKE_RELEASE_POSTFIX}"
    DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}"
    RELWITHDEBINFO_POSTFIX "${CMAKE_RELWITHDEBINFO_POSTFIX}"
    MINSIZEREL_POSTFIX "${CMAKE_MINSIZEREL_POSTFIX}"
  )
  add_custom_target("run_${NAME}" COMMAND "${NAME}" VERBATIM)
  add_dependencies("run_${NAME}" "${NAME}")
  add_dependencies(run-examples "run_${NAME}")
endfunction()

add_example(empty_example)

add_folders(Example)
