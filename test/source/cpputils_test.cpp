#include "cpputils/cpputils.hpp"

auto main() -> int
{
  auto const result = name();

  return result == "cpputils" ? 0 : 1;
}
