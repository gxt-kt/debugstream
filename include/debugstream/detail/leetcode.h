#pragma once

#include "../stdc++.h"

namespace gxt {
namespace leetcode {
namespace detail {

// 把字符串截断/补全到指定长度，并且让字符串居中
std::string PadStringToDesignChars(const std::string& input, size_t n = 8);
// 把字符串根据给定符号进行截断，返回vector
std::vector<std::string> SplitStringToVector(const std::string& s, char delim);

}  // namespace detail
}  // namespace leetcode
}  // namespace gxt
