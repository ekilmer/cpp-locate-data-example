#include <string>

#include "locate-example/locate-example.hpp"

exported_class::exported_class()
    : m_name("locate-example")
{
}

auto exported_class::name() const -> const char*
{
  return m_name.c_str();
}
