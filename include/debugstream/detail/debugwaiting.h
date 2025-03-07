#pragma once

#include "../stdc++.h"

namespace gxt {
namespace detail {

// 设置终端为非阻塞模式
void SetNonBlocking(bool enable);

// 检查按键是否被按下
int kbhit(void);

// 主要的函数
void DebugWaitPlugin(int time_s = 10);

}  // namespace detail
}  // namespace gxt

#if defined(_MSC_VER)
  // MSVC平台使用DllMain或者全局对象方式
  #define G_DEBUG_WAIT_PLUGIN(...)                                    \
    namespace {                                                       \
      struct GDebugWaitPluginInitializer {                            \
        GDebugWaitPluginInitializer() {                               \
          gxt::detail::DebugWaitPlugin(__VA_ARGS__);                  \
        }                                                             \
      };                                                              \
      static GDebugWaitPluginInitializer g_debug_wait_initializer;    \
    }
#else
  // GCC/Clang等平台使用constructor属性
  #define G_DEBUG_WAIT_PLUGIN(...)                                     \
    __attribute__((constructor(101))) void __g_debug_wait_plugin__() { \
      gxt::detail::DebugWaitPlugin(__VA_ARGS__);                       \
    }
#endif