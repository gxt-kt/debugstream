#pragma once

#include "../stdc++.h"

namespace gxt {
namespace thread {

// 获取当前线程的 ID
inline auto GetCurrentID() { return std::this_thread::get_id(); }

// 挂起当前线程
inline void ThreadSuspend() { std::this_thread::yield(); }

// 获取硬件线程数
inline unsigned int GetHardwareConcurrency() {
  unsigned int num_threads = std::thread::hardware_concurrency();
  return num_threads;
}

#define __param_name(name, line) name##line
#define _param_name(name, line) __param_name(name, line)
#define param_name_line(name) _param_name(name, __LINE__)

// 定义一个辅助宏以拼接行号
#define CALL_ONCE(...)                               \
  static std::once_flag param_name_line(once_flag_); \
  std::call_once(param_name_line(once_flag_) __VA_OPT__(, ) __VA_ARGS__);

}  // namespace thread
}  // namespace gxt
