#pragma once

#include "stdc++.h"

// clang-format off
namespace {
#define TO_STRING_WRAP(x) #x
#define TO_STRING(x) TO_STRING_WRAP(x)

#define COUNT_ARGS_IMPL(_null, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) N
#define COUNT_ARGS(...) COUNT_ARGS_IMPL(0 __VA_OPT__(, ) __VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

#define VARIMP(x) #x << "=" << x

#define G_VAR0() ""
#define G_VAR1(_1) VARIMP(_1)
#define G_VAR2(_1, _2) VARIMP(_1) << "," << G_VAR1(_2)
#define G_VAR3(_1, _2, _3) VARIMP(_1) << "," << G_VAR2(_2, _3)
#define G_VAR4(_1, _2, _3, _4) VARIMP(_1) << "," << G_VAR3(_2, _3, _4)
#define G_VAR5(_1, _2, _3, _4, _5) VARIMP(_1) << "," << G_VAR4(_2, _3, _4, _5)
#define G_VAR6(_1, _2, _3, _4, _5, _6) VARIMP(_1) << "," << G_VAR5(_2, _3, _4, _5, _6)
#define G_VAR7(_1, _2, _3, _4, _5, _6, _7) \
  VARIMP(_1) << "," << G_VAR6(_2, _3, _4, _5, _6, _7)
#define G_VAR8(_1, _2, _3, _4, _5, _6, _7, _8) \
  VARIMP(_1) << "," << G_VAR7(_2, _3, _4, _5, _6, _7, _8)
#define G_VAR9(_1, _2, _3, _4, _5, _6, _7, _8, _9) \
  VARIMP(_1) << "," << G_VAR8(_2, _3, _4, _5, _6, _7, _8, _9)
#define G_VAR10(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10) \
  VARIMP(_1) << "," << G_VAR9(_2, _3, _4, _5, _6, _7, _8, _9, _10)

#define G_VARHELPIMP(N, ...) G_VAR##N(__VA_ARGS__)
#define G_VARHELP(N, ...) G_VARHELPIMP(N __VA_OPT__(, ) __VA_ARGS__)

// Usage: std::cout << VAR(a,b) // stdout: a = ${a} , b = ${b}
#define VAR(...) G_VARHELP(COUNT_ARGS(__VA_ARGS__) __VA_OPT__(, ) __VA_ARGS__)
}
// clang-format on

// Define a macro to get the parameter at the specified position in the
// parameter list
#define GET_ARG(N, ...) GET_ARG_##N(__VA_ARGS__)
#define GET_ARG_1(arg, ...) arg
#define GET_ARG_2(arg, ...) GET_ARG_1(__VA_ARGS__)
#define GET_ARG_3(arg, ...) GET_ARG_2(__VA_ARGS__)
#define GET_ARG_4(arg, ...) GET_ARG_3(__VA_ARGS__)
#define GET_ARG_5(arg, ...) GET_ARG_4(__VA_ARGS__)

#define G_FILE __FILE__
#define G_LINE __LINE__
#define G_FILE_LINE (__FILE__ ":" TO_STRING(__LINE__))
