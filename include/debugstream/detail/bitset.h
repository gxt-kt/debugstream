#pragma once

#include "../stdc++.h"

// watch bit
#if SUPPORTS_CPP17
#include <bitset>
namespace gxt {
#define G_WATCH_BYTE(var, n) \
  (static_cast<int>(*(reinterpret_cast<const std::byte*>(&var) + n)))

template <typename T, size_t N = sizeof(T)>
std::string WatchBit(const T& val) {
  // get address
  // std::stringstream ss;
  // ss << "0x" << std::hex << reinterpret_cast<long>(&val);
  // gDebug(ss.str());

  std::stringstream ss;
  for (int i = N - 1; i >= 0; i--) {
    ss << std::bitset<8>(G_WATCH_BYTE(val, i)) << " ";
  }
  std::string str = ss.str();
  str.pop_back();
  return str;
}
}  // namespace gxt
#endif
