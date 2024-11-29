#pragma once

// rename fmt::format to gxt::format
#define FMT_HEADER_ONLY
#include "detail/fmt/format.h"
namespace gxt {
template <typename... T>
FMT_NODISCARD FMT_INLINE auto format(fmt::format_string<T...> fmt, T&&... args)
    -> std::string {
  return vformat(fmt, fmt::make_format_args(args...));
}

template <typename Locale, typename... T,
          FMT_ENABLE_IF(fmt::detail::is_locale<Locale>::value)>
inline auto format(const Locale& loc, fmt::format_string<T...> fmt, T&&... args)
    -> std::string {
  return fmt::vformat(loc, string_view(fmt), fmt::make_format_args(args...));
}
}  // namespace gxt
