#pragma once

#include "../stdc++.h"

namespace gxt {
namespace filename {

// 获取完整文件名 例如 /home/abc/def/xyz.cc
#define G_GET_FILE_PATH() __FILE__

// 获取去除掉路径的文件名 例如 xyz.cc
#define G_GET_FILE_FILENAME()                                        \
  []() -> std::string {                                              \
    std::string full_path = G_GET_FILE_PATH();                       \
    return gxt::filename::ExtractPathComponents(full_path).filename; \
  }()

// 获取去除掉路径和后缀的文件名 例如 xyz
#define G_GET_FILE_BASENAME()                                        \
  []() -> std::string {                                              \
    std::string full_path = G_GET_FILE_FILENAME();                   \
    return gxt::filename::ExtractPathComponents(full_path).basename; \
  }()

// 获取路径和后缀的文件名 例如 /home/abc/def
#define G_GET_FILE_DIRECTORY()                                        \
  []() -> std::string {                                               \
    std::string full_path = G_GET_FILE_FILENAME();                    \
    return gxt::filename::ExtractPathComponents(full_path).directory; \
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

struct PathComponents {
  std::string filename;
  std::string basename;
  std::string directory;
  std::string extension;
};

/*
 * 把路径提取出对应的名字
 * exmaple：input "/home/abc/def/xyz.cc"
 * filename: "xyz.cc"
 * basename: "xyz"
 * directory: "/home/abc/def"
 * extension: "cc"
 *
 *
 */
inline PathComponents ExtractPathComponents(const std::string &original_path) {
  PathComponents components;
  std::string::size_type pos = original_path.find_last_of("/\\");
  std::string::size_type ext_pos = original_path.rfind('.');

  // Trim leading and trailing whitespaces from the path
  std::string trimmed_path = original_path;
  trimmed_path.erase(0, trimmed_path.find_first_not_of(" \t\n\r"));
  trimmed_path.erase(trimmed_path.find_last_not_of(" \t\n\r") + 1);

  // If the trimmed path is empty, set all components to empty and return
  if (trimmed_path.empty()) {
    return components;
  }

  // Handle the case where there is no directory
  if ((pos = trimmed_path.find_last_of("/\\")) == std::string::npos) {
    pos = 0;  // Set pos to 0 for relative paths without directory
  } else {
    // Include the '/' or '\' in the directory
    pos += 1;
  }

  // Handle the case where there is no extension
  if (ext_pos == std::string::npos || ext_pos < pos) {
    ext_pos = trimmed_path.length();
  }

  // Extract the directory
  if (pos > 0) {  // Only set directory if pos > 0 to avoid leading slash
    components.directory = trimmed_path.substr(0, pos - 1);
  }

  // Extract the filename
  components.filename = trimmed_path.substr(pos);

  // Extract the basename and extension
  std::string::size_type dot_pos =
      ext_pos > pos ? ext_pos : trimmed_path.length();
  components.basename = trimmed_path.substr(pos, dot_pos - pos);
  components.extension =
      dot_pos < trimmed_path.length() ? trimmed_path.substr(dot_pos + 1) : "";

  return components;
}

// GetExecutableName
#ifdef _WIN32
#include <string.h>  // For strlen
#include <windows.h>
inline std::string GetExecutableName() {
  char path[MAX_PATH];
  GetModuleFileNameA(nullptr, path, MAX_PATH);
  return std::string(path);  // 可能需要进一步处理
}
#elif __linux__
#include <limits.h>
#include <unistd.h>
#undef basename
#include <libgen.h>  // For basename
inline std::string GetExecutableName() {
  char path[PATH_MAX];
  ssize_t count = readlink("/proc/self/exe", path, PATH_MAX);
  if (count != -1) {
    path[count] = '\0';                  // 确保字符串以 null 结尾
    return std::string(basename(path));  // 提取文件名
  }
  return "";
#undef basename
}
#elif __APPLE__
#include <limits.h>
#include <unistd.h>
#undef basename
#include <libgen.h>       // For basename
#include <mach-o/dyld.h>  // For _NSGetExecutablePath
inline std::string GetExecutableName() {
  char path[PATH_MAX];
  uint32_t size = sizeof(path);
  if (_NSGetExecutablePath(path, &size) == 0) {
    return std::string(basename(path));  // 提取文件名
  }
  return "";
}
#undef basename
#else
#endif

}  // namespace filename
}  // namespace gxt
