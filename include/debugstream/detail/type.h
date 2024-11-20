#pragma once

#include "../stdc++.h"

namespace gxt {
namespace detail {

// Type Name Implement
template <typename T>
inline std::string TypeImpl() {
#ifdef _MSC_VER  // msvc support
  std::string str = __FUNCSIG__;
  auto posi_start = str.find(",");
  posi_start += 1;
  auto posi_end = str.find_first_of(">", posi_start);
#else  // gcc and clangd
  std::string str = __PRETTY_FUNCTION__;
  auto posi_start = str.find("T = ");
  posi_start += 4;
  auto posi_end = str.find_first_of(";", posi_start);
#endif
  return str.substr(posi_start, posi_end - posi_start);
}

}  // namespace detail
}  // namespace gxt

#define G_TYPET(type) (gxt::detail::TypeImpl<type>())
#define G_TYPE(type) (gxt::detail::TypeImpl<decltype(type)>())
