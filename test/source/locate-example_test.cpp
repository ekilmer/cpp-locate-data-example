// Let Catch provide main():
#define CATCH_CONFIG_MAIN

#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>

#include "locate-example/locate-example.hpp"

#include <catch2/catch.hpp>

using path_t = std::filesystem::path;

auto read_file_content(const path_t& path) -> std::string
{
  REQUIRE(std::filesystem::is_regular_file(path));

  std::ifstream file(path);
  if (!file.is_open()) {
    FAIL("Could not open file at path " << path);
  }

  // Read contents
  auto content = std::ostringstream {};
  content << file.rdbuf();

  // Close the file
  file.close();

  return content.str();
}

// Test doesn't always work when compiled statically
#ifndef LOCATE_EXAMPLE_STATIC_DEFINE

TEST_CASE("Test default path lookup", "[classic]")
{
  locate_example::locator locator;

  SECTION("Find static.txt")
  {
    auto path = locator.find_rel_path(static_cast<path_t>("static.txt"));
    REQUIRE(path.has_value());

    auto content = read_file_content(*path);
    REQUIRE(content == "HelloWorld!");
  }

  SECTION("Find dynamic.txt")
  {
    auto path = locator.find_rel_path(static_cast<path_t>("dynamic.txt"));
    REQUIRE(path.has_value());

    auto content = read_file_content(*path);
    REQUIRE(content.rfind("Project version: ", 0) == 0);
  }
}

#endif

TEST_CASE("Test explicit path lookup", "[classic]")
{
  locate_example::locator locator;
  locator.add_explicit_search_dir(LOCATE_EXAMPLE_TEST_DATADIR);

  SECTION("Find static.txt uses dir")
  {
    auto path = locator.find_rel_path(static_cast<path_t>("static.txt"),
                                      /*explicit_paths_only=*/true);
    REQUIRE(path.has_value());

    auto content = read_file_content(*path);
    REQUIRE(content == "HelloWorld!");
  }

  SECTION("Find dynamic.txt uses dir")
  {
    auto path = locator.find_rel_path(static_cast<path_t>("dynamic.txt"),
                                      /*explicit_paths_only=*/true);
    REQUIRE(path.has_value());

    auto content = read_file_content(*path);
    REQUIRE(content.rfind("Project version: ", 0) == 0);
  }

  locator.add_explicit_search_file(LOCATE_EXAMPLE_TEST_STATICFILE);
  SECTION("Find static.txt uses file")
  {
    auto path = locator.find_rel_path(static_cast<path_t>("static.txt"),
                                      /*explicit_paths_only=*/true);
    REQUIRE(path.has_value());

    auto content = read_file_content(*path);
    REQUIRE(content == "GoodbyeWorld!");
  }

  SECTION("Find dynamic.txt uses dir")
  {
    auto path = locator.find_rel_path(static_cast<path_t>("dynamic.txt"),
                                      /*explicit_paths_only=*/true);
    REQUIRE(path.has_value());

    auto content = read_file_content(*path);
    REQUIRE(content.rfind("Project version: ", 0) == 0);
  }
}

TEST_CASE("Test subdir in path", "[classic]")
{
  locate_example::locator locator;
  locator.add_explicit_search_file(LOCATE_EXAMPLE_TEST_STATICFILE);

  SECTION("Do not find subpath")
  {
    auto path = locator.find_rel_path(static_cast<path_t>("data/static.txt"),
                                      /*explicit_paths_only=*/true);
    // Don't find this because we only support relative paths from the
    // directories given
    REQUIRE(path == std::nullopt);
  }

  SECTION("Find file in current directory")
  {
    auto path = locator.find_rel_path(static_cast<path_t>("./static.txt"),
                                      /*explicit_paths_only=*/true);
    // Don't find this because we only support relative paths from the
    // directories given
    REQUIRE(path.has_value());
  }

  locator.add_explicit_search_dir(
      path_t(LOCATE_EXAMPLE_TEST_STATICFILE).parent_path().parent_path());
  SECTION("Find file in data/ directory")
  {
    auto path = locator.find_rel_path(static_cast<path_t>("data/static.txt"),
                                      /*explicit_paths_only=*/true);
    // Don't find this because we only support relative paths from the
    // directories given
    REQUIRE(path.has_value());
  }
}
