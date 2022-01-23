#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>

#include <locate-example/locate-example.hpp>

using path_t = std::filesystem::path;

auto read_file_content(const path_t& path) -> std::optional<std::string>
{
  if (!std::filesystem::is_regular_file(path)) {
    std::cerr << "File at path is not a regular file: " << path << "\n";
    return std::nullopt;
  }

  std::ifstream file(path);
  if (!file.is_open()) {
    std::cerr << "Could not open file at path " << path << "\n";
    return std::nullopt;
  }

  // Read contents
  auto content = std::ostringstream {};
  content << file.rdbuf();

  // Close the file
  file.close();

  return content.str();
}

[[nodiscard]] auto do_example(std::optional<path_t> datadir) -> int
{
  locate_example::locator locator;
  if (datadir) {
    locator.add_explicit_search_dir(*datadir);
  }

  // Static file
  auto static_path = locator.find_rel_path(static_cast<path_t>("static.txt"));
  if (!static_path) {
    std::cerr << "Could not find static.txt\n";
    return 1;
  }
  auto static_content = read_file_content(*static_path);
  if (!static_content) {
    std::cerr << "Could not read static.txt\n";
    return 1;
  }
  std::cout << "Static file contains:\n" << *static_content << "\n";

  std::cout << "\n";

  // Dynamic file
  auto dynamic_path = locator.find_rel_path(static_cast<path_t>("dynamic.txt"));
  if (!dynamic_path) {
    std::cerr << "Could not find dynamic.txt\n";
    return 1;
  }
  auto dynamic_content = read_file_content(*dynamic_path);
  if (!dynamic_content) {
    std::cerr << "Could not read dynamic.txt\n";
    return 1;
  }
  std::cout << "Dynamic file contains:\n" << *dynamic_content << "\n";

  return 0;
}

auto main(int argc, char* argv[]) -> int
{
  std::optional<path_t> datadir;
  if (argc > 1) {
    datadir = path_t(
        argv[1]);  // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
  }

  // TODO(ekilmer): Add a simple CLI parser for taking the data directory of
  //  the installed location of data directory for use when installed as a
  //  static library
  try {
    return do_example(datadir);
  } catch (const std::exception& exception) {
    std::cerr << exception.what() << "\n";
    return 1;
  }

  return 0;
}
