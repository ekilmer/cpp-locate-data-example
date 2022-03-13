#pragma once

#include <filesystem>
#include <list>
#include <optional>
#include <string>

#include "locate-example/locate-example_export.hpp"

namespace locate_example
{
#ifndef LOCATE_EXAMPLE_STATIC_DEFINE
/**
 * @brief Dummy function to expose an address when compiled as a shared library
 *
 * Used to locate files relative to this compiled shared library's location.
 */
LOCATE_EXAMPLE_EXPORT auto find_me() -> void;
#endif

/**
 * @brief Find data files using relative paths to shared library or executable
 * in order to support relocatable installation directories.
 *
 * This class searches a number of locations in order of expected priority to
 * find data files needed during runtime.
 *
 * The user may provide additional search paths in the event that the user wants
 * to use custom data files or if the library is statically linked.
 *
 * With static linking it is impossible to automatically determine where the
 * data files are located because there is no statically determined location
 * that should always exist. For shared libraries and executables that are a
 * part of this project, we can use pre-calculated paths, determined at
 * configure time, to calculate the lookup path based on the location of the
 * shared library or executable.
 */
class LOCATE_EXAMPLE_EXPORT locator
{
public:
  using path_t = std::filesystem::path;

  /**
   * @brief Initializes default hard-coded paths to search for data files
   */
  locator();

  /**
   * @brief Whether to check environment variable when finding files
   * @param use_env whether to check environment variable
   */
  auto use_env(bool use_env) -> void;

  /**
   * @brief Whether to check environment variable when finding files
   */
  [[nodiscard]] auto use_env() const -> bool;

  /**
   * @brief Add a non-default search directory to the lookup locations.
   * @param dir The directory to use as a base for searching
   */
  auto add_explicit_search_dir(const path_t& dir) -> void;

  /**
   * @brief Add a non-default file path to the lookup locations.
   * @param file The file path to use for searching
   */
  auto add_explicit_search_file(const path_t& file) -> void;

  /**
   * @brief Find the specified data file by name in the data directory
   * @param rel_path The name of the file to find
   * @param explicit_paths_only Whether to only look in paths that have been
   * manually set
   * @return The path to the file if found
   */
  [[nodiscard]] auto find_rel_path(const path_t& rel_path,
                                   bool explicit_paths_only = false) const
      -> std::optional<std::filesystem::path>;

private:
  // This path is the install location as specified during the build
  LOCATE_EXAMPLE_SUPPRESS_C4251
  path_t m_standard_location;

  // This path is calculated from where the compiled object (shared lib or
  // executable) is located at runtime. This is to support relocatable
  // installations
  LOCATE_EXAMPLE_SUPPRESS_C4251
  path_t m_calculated_from_obj_path;

  LOCATE_EXAMPLE_SUPPRESS_C4251
  std::list<path_t> m_explicit_directories;

  LOCATE_EXAMPLE_SUPPRESS_C4251
  std::list<path_t> m_explicit_files;

  // Environment variable lookup for installation root
  constexpr static auto m_env_install_root = "LOCATE_EXAMPLE_INSTALL_ROOT";

  // Flag on whether to use the environment variable for extra path lookups
  bool m_use_env = false;
};

}  // namespace locate_example
