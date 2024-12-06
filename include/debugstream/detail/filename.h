#pragma once

#include "../stdc++.h"

namespace gxt {
namespace filename {

#define G_GET_FILE_PATH() __FILE__

#define G_GET_FILE_FILENAME()                                   \
  []() -> std::string {                                         \
    std::string full_path = __FILE__;                           \
    return full_path.substr(full_path.find_last_of("/\\") + 1); \
  }()

#define G_GET_FILE_BASENAME()                                      \
  []() -> std::string {                                            \
    std::string file_name = G_GET_FILE_FILENAME();                 \
    std::string::size_type position = file_name.find_last_of('.'); \
    if (position != std::string::npos) {                           \
      file_name = file_name.substr(0, position);                   \
    }                                                              \
    return file_name;                                              \
  }()

// 获取当前时间，格式化为 YYYY-MM-DDTHH:MM:SS
inline std::string GetCurrentTime() {
  std::time_t now = std::time(nullptr);
  std::tm *localTime = std::localtime(&now);

  std::ostringstream oss;
  oss << localTime->tm_year + 1900 << "-" << std::setfill('0') << std::setw(2)
      << (localTime->tm_mon + 1) << "-" << std::setfill('0') << std::setw(2)
      << localTime->tm_mday << "_" << std::setfill('0') << std::setw(2)
      << localTime->tm_hour << ":" << std::setfill('0') << std::setw(2)
      << localTime->tm_min << ":" << std::setfill('0') << std::setw(2)
      << localTime->tm_sec;

  return oss.str();
}

// 创建备份文件路径
// 例如输入 /abc/123.txt 输出 /abc/123_back_2024-12-06_T_17:50:46.txt
inline std::string GetFilePathWithData(const std::string &originalPath) {
  size_t lastSlash = originalPath.find_last_of("/\\");
  std::string directory = (lastSlash != std::string::npos)
                              ? originalPath.substr(0, lastSlash + 1)
                              : "";
  std::string filename = (lastSlash != std::string::npos)
                             ? originalPath.substr(lastSlash + 1)
                             : originalPath;

  size_t dotPos = filename.find_last_of('.');
  std::string baseName =
      (dotPos != std::string::npos) ? filename.substr(0, dotPos) : filename;
  std::string extension =
      (dotPos != std::string::npos) ? filename.substr(dotPos) : "";

  std::string timeStamp = GetCurrentTime();
  return directory + baseName + "_" + timeStamp + extension;
}

}  // namespace filename
}  // namespace gxt
