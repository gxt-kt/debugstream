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

}  // namespace thread
}  // namespace gxt
