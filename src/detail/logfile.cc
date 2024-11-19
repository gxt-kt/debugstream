#include "detail/logfile.h"

WriteToFile::WriteToFile(const std::string& filename) {
  if (filename.empty()) {
    // TODO: tell user to set filename;
    std::terminate();
  } else {
    file_stream_.open(filename, std::ios::out | std::ios::app);
    if (!file_stream_.is_open()) {
      throw std::runtime_error("Could not open log file.");
    }
  }
  logging_thread_ = std::thread(&WriteToFile::ProcessQueue, this);
}

WriteToFile::~WriteToFile() {
  stop_writing_ = true;
  queue_.skip_pop_once();
  queue_.notify_one();
  logging_thread_.join();
  if (file_stream_.is_open()) {
    file_stream_.close();
  }
}

void WriteToFile::Write(const std::string& str) { queue_.push(str); }

void WriteToFile::ProcessQueue() {
  while (!stop_writing_) {
    auto str_batch = queue_.pop_batch();
    for (const auto& str : str_batch) {
      file_stream_ << str;
    }
    file_stream_.flush();
  }
}

Logger::Logger(bool if_write_to_cout, bool if_write_to_file,
               const std::string& file_name) {
  if (if_write_to_cout) {
    auto write_to_cout = std::make_shared<WriteToCout>();
    funs_.insert(write_to_cout);
  }
  if (if_write_to_file) {
    auto write_to_file = std::make_shared<WriteToFile>(file_name);
    funs_.insert(write_to_file);
  }
}

void Logger::log(const std::string& str) const {
  for (const auto& pair : funs_) {
    pair->Write(str);
  }
}

void Logger::log(char* ptr, size_t size) const { log(std::string(ptr, size)); }

Logger __G_LOG_COUT__(true, true, "log.log");
Logger __G_LOG_FILE__(false, true, "log.log");
Logger __G_LOG_COUT_FILE__(true, true, "log.log");
