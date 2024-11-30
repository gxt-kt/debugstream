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

}  // namespace filename
}  // namespace gxt
