#include <filesystem>

#include "locate-example/locate-example.hpp"

#include <cpplocate/cpplocate.h>

#include "locate-example/defaultPaths.hpp"

namespace locate_example
{
namespace
{
// Search for file 'path' in directory 'dir'
auto find_path_in_dir(const std::filesystem::path& path,
                      const std::filesystem::path& dir)
    -> std::optional<std::filesystem::path>
{
  auto find = dir / path;
  if (std::filesystem::exists(find)) {
    return find;
  }
  return std::nullopt;
}
}  // namespace

locator::locator()
{
  m_standard_location = path_t(install_prefix_dir) / install_datadir_relative;

// TODO(ekilmer): Should we still check if this static library was compiled
// into a shared library?
#ifdef LOCATE_EXAMPLE_STATIC_DEFINE
  m_calculated_from_obj_path = path_t(cpplocate::getExecutablePath())
                                   .append(datadir_relative_lib)
#else
  m_calculated_from_obj_path =
      path_t(cpplocate::getLibraryPath(
                 reinterpret_cast<void*>(&locate_example::find_me)))
          .parent_path()
          .append(datadir_relative_lib)
#endif
                                   .lexically_normal();
}

#ifndef LOCATE_EXAMPLE_STATIC_DEFINE
auto find_me() -> void {};
#endif

auto locator::find_rel_path(const path_t& rel_path,
                            bool explicit_paths_only) const
    -> std::optional<std::filesystem::path>
{
  // Step 1. Search explicit files
  for (const auto& file : m_explicit_files) {
    if (file.filename() == rel_path.lexically_normal().string()
        && std::filesystem::exists(file))
    {
      return file;
    }
  }

  // Step 2. Search explicit directories
  for (const auto& dir : m_explicit_directories) {
    if (auto found = find_path_in_dir(rel_path, dir)) {
      return found;
    }
  }

  if (explicit_paths_only) {
    return std::nullopt;
  }

  // Step 3. Search environment variable
  if (m_use_env) {
    auto* install_dir = std::getenv(m_env_install_root);
    if (install_dir != nullptr) {
      if (auto found = find_path_in_dir(rel_path, install_dir)) {
        return found;
      }
    }
  }

  // Step 4. Relative paths to this compiled object shared library or executable
  if (auto found = find_path_in_dir(rel_path, m_calculated_from_obj_path)) {
    return found;
  }

  // Step 5. Compiled, known install location
  if (auto found = find_path_in_dir(rel_path, m_standard_location)) {
    return found;
  }

  // Else not found
  return std::nullopt;
}

auto locator::use_env(bool use_env) -> void
{
  m_use_env = use_env;
}

auto locator::use_env() const -> bool
{
  return m_use_env;
}

auto locator::add_explicit_search_file(const path_t& file) -> void
{
  m_explicit_files.emplace_back(file);
}

auto locator::add_explicit_search_dir(const path_t& dir) -> void
{
  m_explicit_directories.emplace_back(dir);
}

}  // namespace locate_example
