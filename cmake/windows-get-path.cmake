# This function will add shared libraries paths to the VARIABLE, which can be
# added to tests, so they can be found. Windows does not support RPATH or
# similar. See:
# https://docs.microsoft.com/en-us/windows/win32/dlls/dynamic-link-library-search-order
# Usage: windows_get_path(<var> <target>...)
function(windows_get_path VARIABLE)
  if(NOT CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
    return()
  endif()

  set(path "")
  set(glue "")
  foreach(target IN LISTS ARGN)
    if(TARGET "${target}")
      get_target_property(type "${target}" TYPE)
      if(type STREQUAL "SHARED_LIBRARY")
        set(path "${path}${glue}$<TARGET_FILE_DIR:${target}>")
        set(glue "$<SEMICOLON>") # backslash is important
      endif()
    endif()
  endforeach()
  if(DEFINED "${VARIABLE}")
    set("${VARIABLE}" "${${VARIABLE}}${path}" PARENT_SCOPE)
  else()
    if(NOT path STREQUAL "")
      set("${VARIABLE}" "PATH=${path}" PARENT_SCOPE)
    else()
      set("${VARIABLE}" "")
    endif()
  endif()
endfunction()
