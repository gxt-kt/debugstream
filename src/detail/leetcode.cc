#include "detail/leetcode.h"

namespace gxt {
namespace leetcode {
namespace detail {
std::string PadStringToDesignChars(const std::string& input, size_t n) {
  // 检查原始字符串的长度
  if (input.length() >= n) {
    return input.substr(0, n);  // 只保留前n个字符
  }
  // 计算需要补全的字符数
  int padding_cnt = n - input.length();
  // 计算前半部分和后半部分的补全字符数
  int left_pad_cnt = padding_cnt / 2;
  int right_pad_cnt = padding_cnt - left_pad_cnt;
  // 构建补全后的字符串
  std::string padded_str;
  padded_str.reserve(n);  // 预分配空间以提高性能
  // 添加前半部分的补全字符
  padded_str.append(left_pad_cnt, ' ');
  // 添加原始字符串
  padded_str += input;
  // 添加后半部分的补全字符
  padded_str.append(right_pad_cnt, ' ');
  return padded_str;
}

std::vector<std::string> SplitStringToVector(const std::string& s, char delim) {
  std::vector<std::string> result;
  std::stringstream ss(s);

  std::string item;
  while (getline(ss, item, delim)) {
    result.push_back(item);
  }
  return result;
}

}  // namespace detail
}  // namespace leetcode
}  // namespace gxt
