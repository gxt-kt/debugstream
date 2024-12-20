#pragma once

#include "../stdc++.h"

// random number lib
#include <cstdlib>
#include <random>
namespace gxt {

// get random number method : pool scalability
/*
template <typename T=int>
inline typename std::enable_if<std::is_integral<T>::value == true, T>::type
Random(T min = std::numeric_limits<T>::min(),
       T max = std::numeric_limits<T>::max()) {
  if (min > max) {
    throw std::invalid_argument("Invalid range: min > max");
  }
  std::random_device rd;
  std::mt19937 gen(0);
  std::uniform_int_distribution<T> dist(min, max);
  return dist(gen);
}

template <typename T>
inline typename std::enable_if<std::is_floating_point<T>::value == true,
T>::type Random(T min = std::numeric_limits<T>::min(), T max =
std::numeric_limits<T>::max()) { if (min > max) { throw
std::invalid_argument("Invalid range: min > max");
  }
  std::random_device rd;
  std::mt19937 gen(0);
  std::uniform_real_distribution<T> dist(min, max);
  return dist(gen);
}
*/

namespace detail {
template <typename T>
struct RandomTypeTraits {
  static T Min() { return std::numeric_limits<T>::min(); }
  static T Max() { return std::numeric_limits<T>::max(); }

  template <typename U>
  typename std::enable_if<std::is_integral<U>::value == true, U>::type
  GetValImpl_(U min, U max) {
    std::uniform_int_distribution<U> dist(min, max);
    return dist(gen_);
  }
  template <typename U>
  typename std::enable_if<std::is_floating_point<U>::value == true, U>::type
  GetValImpl_(U min, U max) {
    std::uniform_real_distribution<U> dist(min, max);
    return dist(gen_);
  }

  T GetVal(T min, T max) { return GetValImpl_<T>(min, max); }
  RandomTypeTraits(std::mt19937& gen) : gen_(gen) {};
  std::mt19937& gen_;
};

template <>
struct RandomTypeTraits<bool> {
  static double Min() { return 0.0; }
  static double Max() { return 1.0; }
  using DistributionType = std::uniform_real_distribution<double>;
  bool GetVal(bool min, bool max) {
    DistributionType dist(Min(), Max());
    return dist(gen_) > 0.5;
  }
  RandomTypeTraits(std::mt19937& gen) : gen_(gen) {};
  std::mt19937& gen_;
};

inline std::mt19937& GenerateRandomGen(
    unsigned int value = std::numeric_limits<unsigned int>::max()) {
  static std::random_device rd;
  // static std::mt19937 gen(0);
  static std::mt19937 gen(rd());
  return gen;
}
}  // namespace detail

template <typename T = int, int rd_val = -1>
inline T Random(T min = gxt::detail::RandomTypeTraits<T>::Min(),
                T max = gxt::detail::RandomTypeTraits<T>::Max()) {
  if (min > max) {
    throw std::invalid_argument("Invalid range: min > max");
  }
  if (rd_val == -1) {
    return gxt::detail::RandomTypeTraits<T>(gxt::detail::GenerateRandomGen())
        .GetVal(min, max);
  } else {
    static std::mt19937 gen(rd_val);
    return gxt::detail::RandomTypeTraits<T>(gen).GetVal(min, max);
  }
}

}  // namespace gxt
