# ---- Developer mode ----

# Developer mode enables targets and code paths in the CMake scripts that are
# only relevant for the developer(s) of locate-example
# Targets necessary to build the project must be provided unconditionally, so
# consumers can trivially build and package the project
if(PROJECT_IS_TOP_LEVEL)
  option(locate-example_DEVELOPER_MODE "Enable developer mode" OFF)
  option(BUILD_SHARED_LIBS "Build shared libs." OFF)

  set(CMAKE_INSTALL_INCLUDEDIR include/locate-example CACHE PATH "")
endif()

# We set up the build directory like an install directory, so always include
# these variables, even if CMAKE_SKIP_INSTALL_RULES is ON
include(GNUInstallDirs)

# Allow package maintainers to freely override this project's installed data files
set(locate-example_INSTALL_DATADIR "${CMAKE_INSTALL_DATADIR}/locate-example"
  CACHE PATH "Project data file root relative to the install prefix"
)
mark_as_advanced(locate-example_INSTALL_DATADIR)
set(
  locate-example_INSTALL_CMAKEDIR "${CMAKE_INSTALL_DATADIR}/locate-example"
  CACHE PATH "CMake package config location relative to the install prefix"
)
mark_as_advanced(locate-example_INSTALL_CMAKEDIR)

# ---- Suppress C4251 on Windows ----

# Please see include/locate-example/locate-example.hpp for more details
set(pragma_suppress_c4251 "
/* This needs to suppress only for MSVC */
#if defined(_MSC_VER) && !defined(__ICL)
#  define LOCATE_EXAMPLE_SUPPRESS_C4251 _Pragma(\"warning(suppress:4251)\")
#else
#  define LOCATE_EXAMPLE_SUPPRESS_C4251
#endif
")

# ---- Warning guard ----

# target_include_directories with the SYSTEM modifier will request the compiler
# to omit warnings from the provided paths, if the compiler supports that
# This is to provide a user experience similar to find_package when
# add_subdirectory or FetchContent is used to consume this project
set(warning_guard "")
if(NOT PROJECT_IS_TOP_LEVEL)
  option(
      locate-example_INCLUDES_WITH_SYSTEM
      "Use SYSTEM modifier for locate-example's includes, disabling warnings"
      ON
  )
  mark_as_advanced(locate-example_INCLUDES_WITH_SYSTEM)
  if(locate-example_INCLUDES_WITH_SYSTEM)
    set(warning_guard SYSTEM)
  endif()
endif()
