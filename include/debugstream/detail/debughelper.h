#pragma once

#include "../stdc++.h"

namespace gxt {
namespace detail {

// Print split line such as "=============================="
class SplitLine {
 public:
  std::string operator()(std::string str, size_t size, char c) const {
    size_t len = str.size();
    if (len > size) return "";
    size_t left_len = (size - len) / 2;
    std::string ret;
    ret = std::string(left_len, c) + str +
          std::string((size - left_len - len), c);
    return ret;
  }
  std::string operator()(size_t cnt = 30, std::string str = "=") const {
    std::string res;
    res.reserve(cnt * str.size());
    while (cnt--) res += str;
    return res;
  }
  friend std::ostream& operator<<(std::ostream& os, const SplitLine& obj) {
    std::string res;
    res = obj.operator()(30, "=");
    os << res;
    return os;
  }
};
#define G_SPLIT_LINE gxt::detail::SplitLine()


// print class attributes
template <class T,
          class D = typename std::enable_if<std::is_class<T>::value, T>::type>
[[nodiscard]] inline std::string ClassDetail(std::string class_name) {
  // static_assert(std::is_same<T,D>::value,"T must be class type");

  std::stringstream str;
  str << std::boolalpha;
  str << "\n";

#define CLASS_DETAIL_SPLIT(str_) str << G_SPLIT_LINE(44, str_) << "\n";

#define CLASS_DETAIL_VAR(tt)                                       \
  str << std::setw(35) << std::left << #tt << "| " << std::setw(5) \
      << std::tt<T>() << " |\n"

  CLASS_DETAIL_SPLIT(">")
  str << "class name: " << class_name << "\n";
  CLASS_DETAIL_SPLIT("=")

  CLASS_DETAIL_VAR(is_constructible);
  CLASS_DETAIL_VAR(is_trivially_constructible);
  CLASS_DETAIL_VAR(is_nothrow_constructible);

  CLASS_DETAIL_SPLIT("-")

  CLASS_DETAIL_VAR(is_default_constructible);
  CLASS_DETAIL_VAR(is_trivially_default_constructible);
  CLASS_DETAIL_VAR(is_nothrow_default_constructible);

  CLASS_DETAIL_SPLIT("-")

  CLASS_DETAIL_VAR(is_copy_constructible);
  CLASS_DETAIL_VAR(is_trivially_copy_constructible);
  CLASS_DETAIL_VAR(is_nothrow_copy_constructible);

  CLASS_DETAIL_SPLIT("-")

  CLASS_DETAIL_VAR(is_move_constructible);
  CLASS_DETAIL_VAR(is_trivially_move_constructible);
  CLASS_DETAIL_VAR(is_nothrow_move_constructible);

  CLASS_DETAIL_SPLIT("-")

  // CLASS_DETAIL_VAR(is_assignable);
  // CLASS_DETAIL_VAR(is_trivially_assignable);
  // CLASS_DETAIL_VAR(is_nothrow_assignable);
  //
  // CLASS_DETAIL_SPLIT("-")

  CLASS_DETAIL_VAR(is_copy_assignable);
  CLASS_DETAIL_VAR(is_trivially_copy_assignable);
  CLASS_DETAIL_VAR(is_nothrow_copy_assignable);

  CLASS_DETAIL_SPLIT("-")

  CLASS_DETAIL_VAR(is_move_assignable);
  CLASS_DETAIL_VAR(is_trivially_move_assignable);
  CLASS_DETAIL_VAR(is_nothrow_move_assignable);

  CLASS_DETAIL_SPLIT("-")

  CLASS_DETAIL_VAR(is_destructible);
  CLASS_DETAIL_VAR(is_trivially_destructible);
  CLASS_DETAIL_VAR(is_nothrow_destructible);

  CLASS_DETAIL_SPLIT("-")

  CLASS_DETAIL_VAR(has_virtual_destructor);

#if SUPPORTS_CPP17
  // CLASS_DETAIL_SPLIT("-")
  // CLASS_DETAIL_VAR(is_swappable_with);
  CLASS_DETAIL_VAR(is_swappable);
  // CLASS_DETAIL_VAR(is_nothrow_swappable_with);
  CLASS_DETAIL_VAR(is_nothrow_swappable);
#endif

  CLASS_DETAIL_SPLIT("=")
  CLASS_DETAIL_SPLIT("<")

#undef CLASS_DETAIL_VAR
#undef CLASS_DETAIL_SPLIT

  return str.str();
}

}  // namespace detail
}  // namespace gxt

#define G_CLASS(type) gxt::detail::ClassDetail<type>(#type)
