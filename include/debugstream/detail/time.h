#pragma once

#include "../stdc++.h"

// time lib
namespace gxt {
// #if !defined(G_CONFIG_TIME_COLOR_NAME)
// #define G_CONFIG_TIME_COLOR_NAME gDebugCol3
// #endif
// #if !defined(G_CONFIG_TIME_COLOR_TIME)
// #define G_CONFIG_TIME_COLOR_TIME gDebugCol4
// #endif
namespace detail {

// 默认模板参数为 std::chrono::milliseconds
template <typename T = std::chrono::milliseconds>
struct TimeUnitString {
  static std::string GetUnitString() { return "ms"; }
};
// 针对 std::chrono::microseconds 的模板特化
template <>
struct TimeUnitString<std::chrono::microseconds> {
  static std::string GetUnitString() { return "us"; }
};
template <>
struct TimeUnitString<std::chrono::nanoseconds> {
  static std::string GetUnitString() { return "ns"; }
};
template <>
struct TimeUnitString<std::chrono::seconds> {
  static std::string GetUnitString() { return "s"; }
};

template <typename T = std::chrono::milliseconds>
class TimeCount {
  using time_type = std::chrono::high_resolution_clock::time_point;

 public:
  TimeCount(std::string str, time_type begin) {
    name_ = str;
    start_time_ = begin;
  }
  long Print() {
    has_print_ = true;
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration_time =
        std::chrono::duration_cast<T>(end_time - start_time_).count();
    std::string name;
    std::string time;
    if (std::string(name_).empty())
      name += std::string("Default");
    else
      time += name_;
    time += " Time: " + std::to_string(duration_time) + " ";
    time += TimeUnitString<T>::GetUnitString();
    if (print_) {
      std::cout << name << " " << time << std::endl;
    }
    // G_CONFIG_TIME_COLOR_NAME().NoSpace()
    //     << name << G_CONFIG_TIME_COLOR_TIME() << time;
    return duration_time;
  }
  ~TimeCount() {
    if (!has_print_) {
      Print();
    }
  }
  void SetNotPrint() { print_ = false; };

 private:
  std::string name_;
  bool has_print_{false};
  bool print_{true};
  time_type start_time_;
};

}  // namespace detail

// 定义宏 TIME_FUNCTION 来计算一个函数耗时
#define TIME_FUNCTION(func)                                                \
  [&]() -> decltype(func) {                                                \
    auto start = std::chrono::high_resolution_clock::now();                \
    decltype(func) result = func;                                          \
    auto end = std::chrono::high_resolution_clock::now();                  \
    auto duration =                                                        \
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start) \
            .count();                                                      \
    std::string name;                                                      \
    std::string time;                                                      \
    name += std::string(#func);                                            \
    time += std::string(" Time: ") + std::to_string(duration) + " ms";     \
    G_CONFIG_TIME_COLOR_NAME().NoSpace()                                   \
        << name << G_CONFIG_TIME_COLOR_TIME() << time;                     \
    return result;                                                         \
  }()

// 定义宏 TIME_BEGIN 来开始计时(如果不执行TIME_END，就会在析构时自动输出时间)
#define TIME_BEGIN_MS(...)                                                    \
  std::unique_ptr<gxt::detail::TimeCount<std::chrono::milliseconds>>          \
      __time_count_##__VA_ARGS__ =                                            \
          std::unique_ptr<gxt::detail::TimeCount<std::chrono::milliseconds>>( \
              new gxt::detail::TimeCount<std::chrono::milliseconds>(          \
                  #__VA_ARGS__, std::chrono::high_resolution_clock::now()))
#define TIME_BEGIN_US(...)                                                    \
  std::unique_ptr<gxt::detail::TimeCount<std::chrono::microseconds>>          \
      __time_count_##__VA_ARGS__ =                                            \
          std::unique_ptr<gxt::detail::TimeCount<std::chrono::microseconds>>( \
              new gxt::detail::TimeCount<std::chrono::microseconds>(          \
                  #__VA_ARGS__, std::chrono::high_resolution_clock::now()))
#define TIME_BEGIN_NS(...)                                                   \
  std::unique_ptr<gxt::detail::TimeCount<std::chrono::nanoseconds>>          \
      __time_count_##__VA_ARGS__ =                                           \
          std::unique_ptr<gxt::detail::TimeCount<std::chrono::nanoseconds>>( \
              new gxt::detail::TimeCount<std::chrono::nanoseconds>(          \
                  #__VA_ARGS__, std::chrono::high_resolution_clock::now()))
#define TIME_BEGIN_S(...)                                                \
  std::unique_ptr<gxt::detail::TimeCount<std::chrono::seconds>>          \
      __time_count_##__VA_ARGS__ =                                       \
          std::unique_ptr<gxt::detail::TimeCount<std::chrono::seconds>>( \
              new gxt::detail::TimeCount<std::chrono::seconds>(          \
                  #__VA_ARGS__, std::chrono::high_resolution_clock::now()))
#define TIME_BEGIN(...) TIME_BEGIN_MS(__VA_ARGS__)

// 定义宏 TIME_END 来打印输出执行时间
#define TIME_END(...) __time_count_##__VA_ARGS__->Print()
// 定义宏不打印输出时间
#define TIME_END_SET_NO_PRINT(...) __time_count_##__VA_ARGS__->SetNotPrint()

// 定义宏 TIME_CODE 来开始计算代码执行时间
#define TIME_CODE(...) \
  [&]() {              \
    TIME_BEGIN();      \
    __VA_ARGS__;       \
    TIME_END();        \
  }();

// 定义宏 TIME_LOOP 来计算一个循环耗时
#define TIME_LOOP(...)                                                       \
  []() -> std::string {                                                      \
    static auto __time_loop_begin_##__VA_ARGS__ =                            \
        std::chrono::high_resolution_clock::now();                           \
    static size_t __time_loop_i__##__VA_ARGS__ = 0;                          \
    auto __time_loop_end_##__VA_ARGS__ =                                     \
        std::chrono::high_resolution_clock::now();                           \
    auto __loop_duration_time__##__VA_ARGS__ =                               \
        std::chrono::duration_cast<std::chrono::milliseconds>(               \
            __time_loop_end_##__VA_ARGS__ - __time_loop_begin_##__VA_ARGS__) \
            .count();                                                        \
    __time_loop_begin_##__VA_ARGS__ = __time_loop_end_##__VA_ARGS__;         \
    std::string name;                                                        \
    std::string time;                                                        \
    if (__time_loop_i__##__VA_ARGS__ == 0) {                                 \
      name = std::string("TIME_LOOP(") + #__VA_ARGS__ +                      \
             "): " + std::to_string(__time_loop_i__##__VA_ARGS__);           \
      time = std::string(" Time: ") + "initialize";                          \
    } else {                                                                 \
      name = std::string("TIME_LOOP(") + #__VA_ARGS__ +                      \
             "): " + std::to_string(__time_loop_i__##__VA_ARGS__);           \
      time = std::string(" Time: ") +                                        \
             std::to_string(__loop_duration_time__##__VA_ARGS__) + " ms";    \
    }                                                                        \
    ++__time_loop_i__##__VA_ARGS__;                                          \
    G_CONFIG_TIME_COLOR_NAME().NoSpace()                                     \
        << name << G_CONFIG_TIME_COLOR_TIME() << time;                       \
    return name + time;                                                      \
  }()

inline void Sleep(std::int64_t time) {
  std::this_thread::sleep_for(std::chrono::seconds(time));
}
inline void SleepMs(int64_t time) {
  std::this_thread::sleep_for(std::chrono::milliseconds(time));
}
inline void SleepUs(int64_t time) {
  std::this_thread::sleep_for(std::chrono::microseconds(time));
}
inline void SleepNs(int64_t time) {
  std::this_thread::sleep_for(std::chrono::nanoseconds(time));
}

inline long GetTime() {
  auto time = std::chrono::duration_cast<std::chrono::seconds>(
                  std::chrono::system_clock::now().time_since_epoch())
                  .count();
  return time;
}
inline long GetTimeMs() {
  auto time = std::chrono::duration_cast<std::chrono::milliseconds>(
                  std::chrono::system_clock::now().time_since_epoch())
                  .count();
  return time;
}
inline long GetTimeUs() {
  auto time = std::chrono::duration_cast<std::chrono::microseconds>(
                  std::chrono::system_clock::now().time_since_epoch())
                  .count();
  return time;
}
inline long GetTimeNs() {
  auto time = std::chrono::duration_cast<std::chrono::nanoseconds>(
                  std::chrono::system_clock::now().time_since_epoch())
                  .count();
  return time;
}

// log time %Y-%m-%d %H:%M:%S.ms
inline std::string GetCurrentTime() {
  // 获取当前时间点
  auto now = std::chrono::system_clock::now();
  // 转换为时间_t
  std::time_t now_c = std::chrono::system_clock::to_time_t(now);
  // 转换为当前时间的结构体
  std::tm* now_tm = std::localtime(&now_c);
  // 获取毫秒部分
  auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(
                          now.time_since_epoch()) %
                      1000;
  // 使用字符串流格式化输出
  std::ostringstream oss;
  oss << std::put_time(now_tm, "%Y-%m-%d %H:%M:%S") << '.' << std::setw(3)
      << std::setfill('0') << milliseconds.count();

  return oss.str();
}

}  // namespace gxt
