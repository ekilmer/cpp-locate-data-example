#include <string>

#include "locate-example/locate-example.hpp"

auto main() -> int
{
  exported_class e;

  return std::string("locate-example") == e.name() ? 0 : 1;
}
