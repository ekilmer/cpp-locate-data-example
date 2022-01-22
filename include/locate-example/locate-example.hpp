#pragma once

#include <filesystem>
#include <list>
#include <optional>
#include <string>

#include "locate-example/locate-example_export.hpp"

/**
 * A note about the MSVC warning C4251:
 * This warning should be suppressed for private data members of the project's
 * exported classes, because there are too many ways to work around it and all
 * involve some kind of trade-off (increased code complexity requiring more
 * developer time, writing boilerplate code, longer compile times), but those
 * solutions are very situational and solve things in slightly different ways,
 * depending on the requirements of the project.
 * That is to say, there is no general solution.
 *
 * What can be done instead is understand where issues could arise where this
 * warning is spotting a legitimate bug. I will give the general description of
 * this warning's cause and break it down to make it trivial to understand.
 *
 * C4251 is emitted when an exported class has a non-static data member of a
 * non-exported class type.
 *
 * The exported class in our case is the class below (locator), which
 * has a non-static data member (m_name) of a non-exported class type
 * (std::string).
 *
 * The rationale here is that the user of the exported class could attempt to
 * access (directly, or via an inline member function) a static data member or
 * a non-inline member function of the data member, resulting in a linker
 * error.
 * Inline member function above means member functions that are defined (not
 * declared) in the class definition.
 *
 * Since this exported class never makes these non-exported types available to
 * the user, we can safely ignore this warning. It's fine if there are
 * non-exported class types as private member variables, because they are only
 * accessed by the members of the exported class itself.
 *
 * The name() method below returns a pointer to the stored null-terminated
 * string as a fundamental type (const char), so this is safe to use anywhere.
 * The only downside is that you can have dangling pointers if the pointer
 * outlives the class instance which stored the string.
 *
 * Shared libraries are not easy, they need some discipline to get right, but
 * they also solve some other problems that make them worth the time invested.
 */

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
 * @brief Reports the name of the library
 *
 * Please see the note above for considerations when creating shared libraries.
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
   * @brief Whether to use environment variable when finding files
   * @param use_env whether to use environment variable
   */
  auto use_env(bool use_env) -> void;

  /**
   * @brief Whether to use environment variable when finding files
   */
  [[nodiscard]] auto use_env() const -> bool;

  auto add_explicit_search_dir(const std::string& dir) -> void;

  auto add_explicit_search_file(const std::string& file) -> void;

  /**
   * @brief Find the specified data file by name in the data directory
   * @param rel_path The name of the file to find
   * @param explicit_paths_only Whether to only look in paths that have been
   * manually set
   * @return The path to the file if found
   */
  [[nodiscard]] auto find_rel_path(const std::string& rel_path,
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

  constexpr static auto m_env_install_root = "LOCATE_EXAMPLE_INSTALL_PREFIX";

  bool m_use_env = false;
};

}  // namespace locate_example
