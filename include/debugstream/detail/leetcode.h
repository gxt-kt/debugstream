#pragma once

#include "../stdc++.h"

// string function
namespace gxt {
inline std::string PadStringToDesignChars(const std::string& input,
                                          size_t n = 8) {
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
}  // namespace gxt

namespace gxt {
namespace algorithm {

/*Print Tree
  Example:
  std::string res = PrintTree(
      root, [](Node* node){ return node->left; },
      [](Node* node){ return node->right; },
      [](Node* node){ return node->value; });
  gDebugCol1() << res;
*/
template <typename T, typename F_LEFT, typename F_RIGHT, typename F_VAL>
inline std::string PrintTree(T* node, F_LEFT f_left, F_RIGHT f_right,
                             F_VAL f_val, int level = 0, char branch = ' ') {
  if (node == nullptr) return "";
  std::string res;

  res += PrintTree(f_right(node), f_left, f_right, f_val, level + 1, '/');
  for (int i = 0; i < level; i++) {
    res += "    ";
  }
  res += (branch + std::string("--") + std::to_string(f_val(node)) + "\n");
  res += PrintTree(f_left(node), f_left, f_right, f_val, level + 1, '\\');
  return res;
}

template <typename T, typename F_LEFT, typename F_RIGHT, typename F_VAL>
inline std::vector<std::vector<std::string>> CreateBinaryTreeStructure(
    T* root, F_LEFT f_left, F_RIGHT f_right, F_VAL f_val) {
  std::vector<std::vector<std::string>> treeStructure;
  if (root == nullptr) {
    return treeStructure;
  }

  std::vector<T*> currentLevel;
  std::vector<T*> nextLevel;
  currentLevel.push_back(root);

  while (!currentLevel.empty()) {
    std::vector<std::string> levelValues;

    bool null = true;
    for (const auto& node : currentLevel) {
      if (node != nullptr) {
        null = false;
        break;
      }
    }
    if (null == true) break;

    for (const auto& node : currentLevel) {
      if (node == nullptr) {
        levelValues.push_back("NULL");
        nextLevel.push_back(nullptr);
        nextLevel.push_back(nullptr);
        continue;
      }
      levelValues.push_back(std::to_string(f_val(node)));
      nextLevel.push_back(f_left(node));
      nextLevel.push_back((node->right));
    }
    treeStructure.push_back(levelValues);
    currentLevel = nextLevel;
    nextLevel.clear();
  }

  return treeStructure;
}

/* Example:
 DrawTree(gxt::algorithm::CreateBinaryTreeStructure(root, [](Node* node){ return
 node->left; },
      [](Node* node){ return node->right; },
      [](Node* node){ return node->value; }));
*/
inline void DrawTree(const std::vector<std::vector<std::string>>& tree,
                     size_t pad = 6, size_t space = 2) {
  for (size_t i = 0; i < tree.size(); i++) {
    std::cout << std::string(pad * (tree.size() - i - 1), ' ');
    for (const auto& val : tree.at(i)) {
      std::cout << gxt::PadStringToDesignChars(val, pad)
                << std::string(space, ' ');
    }
    std::cout << "\n";
  }
}
}  // namespace algorithm
}  // namespace gxt