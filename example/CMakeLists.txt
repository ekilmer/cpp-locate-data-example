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

  # Might need to set data directory path
  set(datadir_path "")
  if(NOT LOCATE_EXAMPLE_SHARED)
    set(datadir_path "${LOCATE_EXAMPLE_DATA_DIR}")
  endif()

  # Set PATH for DLL
  set(win_env "")
  if(LOCATE_EXAMPLE_SHARED AND CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
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
