cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    locate-example
    VERSION 0.1.0
    DESCRIPTION "CMake C++ example to locate installed data files"
    HOMEPAGE_URL "https://github.com/ekilmer/cpp-locate-data-example"
    LANGUAGES CXX
)

docs_early_return()

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Declare library ----

add_library(
    locate-example_locate-example
    source/locate-example.cpp
)
add_library(locate-example::locate-example ALIAS locate-example_locate-example)

include(GenerateExportHeader)
generate_export_header(
    locate-example_locate-example
    BASE_NAME locate-example
    EXPORT_FILE_NAME export/locate-example/locate-example_export.hpp
    CUSTOM_CONTENT_FROM_VARIABLE pragma_suppress_c4251
)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(locate-example_locate-example PUBLIC LOCATE_EXAMPLE_STATIC_DEFINE)
endif()

set_target_properties(
    locate-example_locate-example PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
    VERSION "${PROJECT_VERSION}"
    SOVERSION "${PROJECT_VERSION_MAJOR}"
    EXPORT_NAME locate-example
    OUTPUT_NAME locate-example
)

target_include_directories(
    locate-example_locate-example ${warning_guard}
    PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_include_directories(
    locate-example_locate-example SYSTEM
    PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/export>"
)

target_compile_features(locate-example_locate-example PUBLIC cxx_std_17)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Examples ----

if(PROJECT_IS_TOP_LEVEL)
  option(BUILD_EXAMPLES "Build examples tree." "${locate-example_DEVELOPER_MODE}")
  if(BUILD_EXAMPLES)
    add_subdirectory(example)
  endif()
endif()

# ---- Developer mode ----

if(NOT locate-example_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of locate-example"
  )
endif()

include(cmake/dev-mode.cmake)
