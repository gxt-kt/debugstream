#pragma once

#include "../stdc++.h"

template <typename T>
class ThreadSafeQueue {
 public:
  void push(const T& value) {
    std::unique_lock<std::mutex> lock(mtx_);
    queue_.push(value);
    lock.unlock();
    cv_.notify_one();
  }
  void push(T&& value) {
    std::unique_lock<std::mutex> lock(mtx_);
    queue_.push(std::move(value));
    lock.unlock();
    cv_.notify_one();
  }

  std::vector<T> pop_batch() {
    std::vector<T> res;
    std::unique_lock<std::mutex> lock(mtx_);
    cv_.wait(lock, [this] { return !queue_.empty() || skip_pop_; });
    while (!queue_.empty()) {
      res.emplace_back(std::move(queue_.front()));
      queue_.pop();
    }
    // if res empty then skip_pop_ already set to true
    if (res.empty()) {
      skip_pop_ = false;
    }
    return res;
  }

  bool empty() const {
    std::unique_lock<std::mutex> lock(mtx_);
    return queue_.empty();
  }
  bool size() const {
    std::unique_lock<std::mutex> lock(mtx_);
    return queue_.size();
  }
  void notify_one() { cv_.notify_one(); }
  void notify_all() { cv_.notify_all(); }

  void skip_pop_once() { skip_pop_ = true; }

 private:
  std::queue<T> queue_;
  mutable std::mutex mtx_;
  std::condition_variable cv_;

  std::atomic<bool> skip_pop_{false};
};

class G_LOG {
 public:
  virtual void Write(const std::string& str) = 0;
  virtual ~G_LOG() = default;
};

class WriteToCout : public G_LOG {
 public:
  void Write(const std::string& str) override { std::cout << str; }
};

class WriteToCerr : public G_LOG {
 public:
  void Write(const std::string& str) override { std::cerr << str; }
};

class WriteToClog : public G_LOG {
 public:
  void Write(const std::string& str) override { std::clog << str; }
};

class WriteToFile : public G_LOG {
 public:
  explicit WriteToFile(const std::string& filename = "");
  ~WriteToFile() override;
  void Write(const std::string& str) override;

 private:
  void ProcessQueue();
  std::ofstream file_stream_;
  ThreadSafeQueue<std::string> queue_;
  std::thread logging_thread_;
  bool stop_writing_ = false;
};

class Logger {
 public:
  Logger(bool if_write_to_cout = true, bool if_write_to_file = false,
         const std::string& file_name = "");
  void log(const std::string& str) const;
  void log(char* ptr, size_t size) const;

 private:
  std::set<std::shared_ptr<G_LOG>> funs_;
};

extern Logger __G_LOG_FILE__;

void inline DebugLog(const std::string& str) { __G_LOG_FILE__.log(str); }

// clang-format off
// #define gDebugColFun(col_fg,col_bg,func,...) ((gxt::DebugStream(func).NewLine().ClearColor()<<col_fg<<col_bg)(#__VA_ARGS__ __VA_OPT__(,) __VA_ARGS__))
// #define gDebugLog(...) gDebugColFun(gxt::detail::normal_fg,gxt::detail::normal_bg,DebugLog,##__VA_ARGS__)
#define gDebugLog(...) ((gxt::DebugStream(DebugLog).NewLine()<<gxt::GetCurrentTime()<<G_FILE_LINE)(#__VA_ARGS__ __VA_OPT__(,) __VA_ARGS__))

// clang-format on
// #define gDebugColLog(col_fg, col_bg, ...)             \
// #define gDebugColLog(col_fg, col_bg, ...)             \
//   ((gxt::DebugStream(DebugLog).NewLine().ClearColor() \
//     << col_fg << col_bg)(#__VA_ARGS__ __VA_OPT__(, ) __VA_ARGS__))
// #define gDebugLog(...) \
//   gDebugColLog(gxt::black_fg, gxt::yellow_bg, ##__VA_ARGS__)
