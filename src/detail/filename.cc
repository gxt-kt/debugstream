#include "detail/filename.h"

namespace gxt {
namespace filename {

#ifdef _WIN32

#include <string.h>  // For strlen
#include <windows.h>
std::string GetExecutableName() {
  char path[MAX_PATH];
  GetModuleFileNameA(nullptr, path, MAX_PATH);
  return std::string(path);  // 可能需要进一步处理
}

#elif __linux__

#include <limits.h>
#include <unistd.h>
#undef basename
#include <libgen.h>  // For basename
std::string GetExecutableName() {
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

#include <libgen.h>  // For basename
#include <limits.h>
#include <mach-o/dyld.h>  // For _NSGetExecutablePath
#include <unistd.h>
std::string GetExecutableName() {
  char path[PATH_MAX];
  uint32_t size = sizeof(path);
  if (_NSGetExecutablePath(path, &size) == 0) {
    return std::string(basename(path));  // 提取文件名
  }
  return "";
}

#else
#error No defined System
#endif

}  // namespace filename
}  // namespace gxt
