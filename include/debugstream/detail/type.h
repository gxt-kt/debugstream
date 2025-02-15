#pragma once

#include "../stdc++.h"

namespace gxt {
namespace detail {

#if 0
template <typename T> inline std::string TypeImpl() {
  const char *name = typeid(T).name();
#ifdef __GNUC__
  int status = 0;
  char *demangled = abi::__cxa_demangle(name, nullptr, nullptr, &status);
  if (status == 0) {
    std::string result(demangled);
    free(demangled);
    return result;
  }
#endif
  return name;
}

#else
template <typename T> inline std::string TypeImpl() {
  std::string str;
#ifdef _MSC_VER // MSVC compiler
  str = __FUNCSIG__;
  // MSVC 的函数签名格式类似于：std::string __cdecl
  // TypeImpl<std::vector<std::vector<int>>>(void)
  auto posi_start = str.find("TypeImpl<");
  if (posi_start == std::string::npos) {
    return "Unknown";
  }
  posi_start += 9; // 跳过 "TypeImpl<"
  // 处理嵌套模板参数
  int nested_level = 0;
  auto posi_end = posi_start;
  for (; posi_end < str.size(); ++posi_end) {
    if (str[posi_end] == '<') {
      nested_level++;
    } else if (str[posi_end] == '>') {
      if (nested_level == 0) {
        break;
      }
      nested_level--;
    }
  }
  if (posi_end == str.size()) {
    return "Unknown";
  }
#else // GCC 和 Clang 编译器
  str = __PRETTY_FUNCTION__;
  std::cout << "str" << str << std::endl;
  // GCC/Clang 的函数签名格式类似于：std::string TypeImpl() [with T = int]
  auto posi_start = str.find("T = ");
  if (posi_start == std::string::npos) {
    return "Unknown";
  }
  posi_start += 4; // 跳过 "T = "
  auto posi_end = str.find_first_of(";]", posi_start);
  if (posi_end == std::string::npos) {
    return "Unknown";
  }
#endif
  return str.substr(posi_start, posi_end - posi_start);
}
#endif

} // namespace detail
} // namespace gxt

#define G_TYPET(type) (gxt::detail::TypeImpl<type>())
#define G_TYPE(type) (gxt::detail::TypeImpl<decltype(type)>())
