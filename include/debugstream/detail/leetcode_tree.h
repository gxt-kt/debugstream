#pragma once

#include "leetcode.h"

namespace gxt {
namespace leetcode {

struct TreeNode {
  int val;
  TreeNode* left;
  TreeNode* right;
  TreeNode() : val(0), left(nullptr), right(nullptr) {}
  TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
  TreeNode(int x, TreeNode* left, TreeNode* right)
      : val(x), left(left), right(right) {}
};

namespace detail {

template <typename T, typename F_LEFT, typename F_RIGHT, typename F_VAL>
inline std::vector<std::vector<std::string>> BinaryTreeLevelOrderTraversal(
    T* node, F_LEFT f_left, F_RIGHT f_right, F_VAL f_val) {
  std::vector<std::vector<std::string>> res;
  if (node == nullptr) {
    return res;
  }

  std::vector<T*> currentLevel;
  std::vector<T*> nextLevel;
  currentLevel.push_back(node);

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
        levelValues.push_back("null");
        nextLevel.push_back(nullptr);
        nextLevel.push_back(nullptr);
        continue;
      }
      levelValues.push_back(std::to_string(f_val(node)));
      nextLevel.push_back(f_left(node));
      nextLevel.push_back((node->right));
    }
    res.push_back(levelValues);
    currentLevel = nextLevel;
    nextLevel.clear();
  }

  return res;
}

inline std::string LevelOrderTravelsalToString(
    const std::vector<std::vector<std::string>>& tree, size_t pad = 6,
    size_t space = 2) {
  if (tree.empty() || tree.at(0).empty()) {
    return "null tree node";
  }
  std::stringstream res;
  for (size_t i = 0; i < tree.size(); i++) {
    res << std::string(pad * (tree.size() - i - 1), ' ');
    for (const auto& val : tree.at(i)) {
      res << detail::PadStringToDesignChars(val, pad)
          << std::string(space, ' ');
    }
    res << "\n";
  }
  return res.str();
}

}  // namespace detail

// 根据给定字符串生成TreeNode（因为使用了new，会发生内存泄漏，仅供测试）
TreeNode* BuildTreeNode(const std::string& input_str,
                        const std::string& null_str = "null");

/*Example:
  std::string res = PrintTree(
      node, [](Node* node){ return node->left; },
      [](Node* node){ return node->right; },
      [](Node* node){ return node->value; });
  std::cout << res;
*/
template <typename T, typename F_LEFT, typename F_RIGHT, typename F_VAL>
inline std::string PrintTree1(T* node, F_LEFT f_left, F_RIGHT f_right,
                              F_VAL f_val, int level = 0, char branch = ' ') {
  if (level == 0 && !node) return "null tree node";
  if (!node) return "";
  std::string res;
  res += PrintTree1(f_right(node), f_left, f_right, f_val, level + 1, '/');
  for (int i = 0; i < level; i++) {
    res += "    ";
  }
  res += (branch + std::string("--") + std::to_string(f_val(node)) + "\n");
  res += PrintTree1(f_left(node), f_left, f_right, f_val, level + 1, '\\');
  return res;
}

template <typename T>
inline std::string PrintTree1(T* node) {
  return PrintTree1(
      node, [](T* node) { return node->left; },
      [](T* node) { return node->right; }, [](T* node) { return node->val; });
}

template <typename T, typename F_LEFT, typename F_RIGHT, typename F_VAL>
inline std::string PrintTree2(T* node, F_LEFT f_left, F_RIGHT f_right,
                              F_VAL f_val) {
  std::stringstream res;
  if (!node) {
    return "null tree node";
  }
  size_t max_depth = 0;
  {
    // get maxdepth
    std::queue<TreeNode*> que;
    size_t depth = 0;
    que.push(node);
    while (que.size()) {
      max_depth = std::max(max_depth, ++depth);
      size_t que_node = que.size();

      while (que_node--) {
        auto p = que.front();
        que.pop();
        if (f_left(p)) que.push(f_left(p));
        if (f_right(p)) que.push(f_right(p));
      }
    }
  }
  std::vector<std::string> s(max_depth);
  {
    int width = 4;
    for (size_t i = 0; i < max_depth; i++) {
      s[i].resize(((1 << max_depth) - 1) * width);
      for (char& c : s[i]) c = ' ';
    }
    std::queue<std::pair<std::pair<TreeNode*, int>, int>> Q;
    int row = 0, arm = (1 << max_depth) / 4;
    Q.push(make_pair(std::make_pair(node, ((1 << max_depth) - 1) / 2), 0));
    while (Q.size()) {
      int l = Q.size();
      while (l--) {
        auto p = Q.front();
        Q.pop();
        std::string temp = std::to_string(p.first.first->val);
        int rr = (width - temp.size()) / 2;
        int ll = width - temp.size() - rr;
        for (int i = ll; temp[i - ll]; i++)
          s[row][p.first.second * width + i] = temp[i - ll];
        if (p.second) {
          if (p.second == 1) {
            s[row - 1][p.first.second * width + ll + temp.size() - 2] = '\\';
            for (int i = p.first.second * width + ll + temp.size() - 3;
                 s[row - 1][i] == ' '; i--)
              s[row - 1][i] = '=';
          } else {
            s[row - 1][p.first.second * width + ll] = '/';
            for (int i = p.first.second * width + ll + 1; s[row - 1][i] == ' ';
                 i++)
              s[row - 1][i] = '=';
          }
        }
        if (f_left(p.first.first))
          Q.push(make_pair(
              std::make_pair(f_left(p.first.first), p.first.second - arm), -1));
        if (f_right(p.first.first))
          Q.push(make_pair(
              std::make_pair(f_right(p.first.first), p.first.second + arm), 1));
      }
      row++;
      arm >>= 1;
    }
  }
  std::vector<std::string> tmp(max_depth);
  bool makeup = true;
  for (size_t j = 0; j < s[0].size();) {
    bool needmakeup = false, clear = true;
    for (size_t i = 0; i < max_depth; i++) {
      if (s[i][j] != ' ' && s[i][j] != '=') clear = false;
      if ((s[i][j] == '-' || isdigit(s[i][j])) && tmp[0].size() &&
          tmp[i].back() == '/')
        needmakeup = true;
    }
    if (needmakeup && !makeup) {
      for (size_t i = 0; i < max_depth; i++) {
        if (tmp[i].size() && (tmp[i].back() == '=' || isdigit(tmp[i].back())))
          tmp[i] += '=';
        else
          tmp[i] += ' ';
      }
      makeup = true;
    } else {
      if (!clear)
        for (size_t i = 0; i < max_depth; i++) tmp[i] += s[i][j];
      makeup = false;
      j++;
    }
  }
  //
  std::unordered_map<char, std::string> M{{'=', "═"}, {'/', "╔"}, {'\\', "╗"}};
  // std::unordered_map<char, std::string> M{{'=', "─"}, {'/', "╭"}, {'\\',
  // "╮"}};
  for (size_t i = 0; i < max_depth; i++) {
    res << "\n ";
    for (size_t j = 0; j < tmp[i].size(); j++) {
      if (tmp[i][j] == '=') {
        if (M.count(tmp[i][j + 1]))
          res << M[tmp[i][j]];
        else
          res << ' ';
      } else if (M.count(tmp[i][j]))
        res << M[tmp[i][j]];
      else
        res << tmp[i][j];
    }
  }
  return res.str();
}

template <typename T>
inline std::string PrintTree2(T* node) {
  return PrintTree2(
      node, [](T* node) { return node->left; },
      [](T* node) { return node->right; }, [](T* node) { return node->val; });
}

template <typename T, typename F_LEFT, typename F_RIGHT, typename F_VAL>
inline std::string PrintTree3(T* node, F_LEFT f_left, F_RIGHT f_right,
                              F_VAL f_val, size_t pad = 6, size_t space = 2) {
  auto level_order_travelsal =
      detail::BinaryTreeLevelOrderTraversal(node, f_left, f_right, f_val);
  return detail::LevelOrderTravelsalToString(level_order_travelsal, pad, space);
}

/*Example:
  std::string res = PrintTree(
      node, [](Node* node){ return node->left; },
      [](Node* node){ return node->right; },
      [](Node* node){ return node->value; });
  std::cout << res;
*/
template <typename T>
inline std::string PrintTree3(T* node, size_t pad = 6, size_t space = 2) {
  auto level_order_travelsal = detail::BinaryTreeLevelOrderTraversal(
      node, [](T* node) { return node->left; },
      [](T* node) { return node->right; }, [](T* node) { return node->val; });
  return detail::LevelOrderTravelsalToString(level_order_travelsal, pad, space);
}

}  // namespace leetcode
}  // namespace gxt
