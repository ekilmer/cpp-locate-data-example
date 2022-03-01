cmake_minimum_required(VERSION 3.15)

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

  # Might need to set data directory path if we are statically linking
  set(datadir_path "")
  if(NOT locate-example_SHARED)
    set(datadir_path "${locate-example_DATA_DIR}")
  endif()

  # Set PATH for Windows DLL
  set(win_env "")
  if(locate-example_SHARED AND CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
    # NOTE: Change this to something more robust in a real project.
    # See the following
    # * https://cmake.org/cmake/help/latest/module/GetPrerequisites.html
    # * https://cmake.org/cmake/help/latest/command/file.html#get-runtime-dependencies
    set(win_env ${CMAKE_COMMAND} -E env "\"PATH=$<TARGET_FILE_DIR:locate-example::locate-example>$<SEMICOLON>$<TARGET_FILE_DIR:cpplocate::cpplocate>$<SEMICOLON>%PATH%\"")
  endif()
  add_custom_target("run_${NAME}"
    COMMAND ${win_env} $<TARGET_FILE:${NAME}> "${datadir_path}"
  )
  add_dependencies("run_${NAME}" "${NAME}")
  add_dependencies(run-examples "run_${NAME}")
endfunction()

add_example(empty_example)

add_folders(Example)
