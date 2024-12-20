#pragma once

#include "../stdc++.h"

namespace gxt {

// enum imp
namespace detail {

// Enum value must be greater or equals than G_CONFIG_ENUM_RANGE_MIN. By default
// G_CONFIG_ENUM_RANGE_MIN = -128. If need another min range for all enum types
// by default, redefine the macro G_CONFIG_ENUM_RANGE_MIN.
#if !defined(G_CONFIG_ENUM_RANGE_MIN)
#define G_CONFIG_ENUM_RANGE_MIN -128
#endif

// Enum value must be less or equals than G_CONFIG_ENUM_RANGE_MAX. By default
// G_CONFIG_ENUM_RANGE_MAX = 128. If need another max range for all enum types
// by default, redefine the macro G_CONFIG_ENUM_RANGE_MAX.
#if !defined(G_CONFIG_ENUM_RANGE_MAX)
#define G_CONFIG_ENUM_RANGE_MAX 128
#endif

static_assert(G_CONFIG_ENUM_RANGE_MAX < std::numeric_limits<int>::max(),
              "G_CONFIG_ENUM_RANGE_MAX must be less than INT_MAX.");

static_assert(G_CONFIG_ENUM_RANGE_MIN > std::numeric_limits<int>::min(),
              "G_CONFIG_ENUM_RANGE_MIN must be greater than INT_MIN.");

template <typename T, T N>
inline std::string GetEnumNameImp() {
#if defined(__GNUC__) || defined(__clang__)
  std::string tmp = __PRETTY_FUNCTION__;
  auto first = tmp.find("T N = ");
  first += 6;
  auto end = tmp.find(";", first);
  return std::string(tmp, first, end - first);
#elif defined(_MSC_VER)
  // TODO: add support for msvc
#else
#endif
}

template <int begin, int end, typename F>
typename std::enable_if<begin == end>::type TemplateForLoop(const F &fun) {
  fun.template call<begin>();
}
template <int begin, int end, typename F>
typename std::enable_if<begin != end>::type TemplateForLoop(const F &fun) {
  fun.template call<begin>();
  TemplateForLoop<begin + 1, end>(fun);
}

template <typename T>
struct GetEnumClass {
  int n_;
  std::string &str_;
  GetEnumClass(int n, std::string &str) : n_(n), str_(str) {}

  template <int N>
  void call() const {
    if (n_ == N) {
      str_ = detail::GetEnumNameImp<T, T(N)>();
    }
  }
};

}  // namespace detail

template <typename T, int min = G_CONFIG_ENUM_RANGE_MIN,
          int max = G_CONFIG_ENUM_RANGE_MAX>
inline std::string GetEnumName(T n) {
  std::string str;
  gxt::detail::TemplateForLoop<min, max>(
      gxt::detail::GetEnumClass<T>(static_cast<int>(n), str));
  if (str.empty()) {
    throw std::runtime_error("\nenum out of range\n");
  }
  return str;
}

template <typename T, int min = G_CONFIG_ENUM_RANGE_MIN,
          int max = G_CONFIG_ENUM_RANGE_MAX>
inline int GetNameEnum(std::string name) {
  std::string str;
  for (int i = G_CONFIG_ENUM_RANGE_MIN; i <= G_CONFIG_ENUM_RANGE_MAX; i++) {
    gxt::detail::TemplateForLoop<G_CONFIG_ENUM_RANGE_MIN,
                                 G_CONFIG_ENUM_RANGE_MAX>(
        gxt::detail::GetEnumClass<T>(static_cast<int>(i), str));
    if (!str.empty()) {  // solve bug that use class enum
      auto find = str.find("::");
      if (find != std::string::npos) {
        find += 2;
        str = std::string(str, find);
      }
    }
    if (!str.empty() && str == name) {
      return i;
    }
  }
  throw std::runtime_error("\nenum out of range\n");
  return 0;
}
}  // namespace gxt
