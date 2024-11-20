#pragma once

#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <functional>
#include <memory>
#include <sstream>
#include <string>

// Reference : https://github.com/p-ranav/pprint

// There will no debug stream output if define the G_CONFIG_NO_DEBUG_OUTPUT
// #define G_CONFIG_NO_DEBUG_OUTPUT

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <complex>
#include <cstddef>
#include <deque>
#include <fstream>
#include <initializer_list>
#include <iomanip>
#include <iosfwd>
#include <iostream>
#include <limits>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <string_view>
#include <thread>
#include <tuple>
#include <type_traits>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>
#ifdef __GNUG__
#include <cxxabi.h>
#endif

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdlib>

// support c++17 variant and optional
#define SUPPORTS_CPP17 (__cplusplus >= 201703L)
