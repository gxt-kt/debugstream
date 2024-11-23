#pragma once

#include "leetcode.h"

namespace gxt {
namespace leetcode {

struct ListNode {
  int val;
  ListNode *next;
  ListNode() : val(0), next(nullptr) {}

  ListNode(int x) : val(x), next(nullptr) {}
  ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// 根据给定字符串生成List（因为使用了new，会发生内存泄漏，仅供测试）
ListNode *BuildList(const std::string &input_str);

template <typename T, typename F_NEXT, typename F_VAL>
inline std::string PrintList(T *node, F_NEXT f_next, F_VAL f_val) {
  if (!node) {
    return "null list node";
  }
  std::stringstream res;
  while (node) {
    res << f_val(node);
    if (f_next(node)) {
      res << " -> ";
    }
    node = f_next(node);
  }
  return res.str();
}

template <typename T>
inline std::string PrintList(T *node) {
  return PrintList(
      node, [](T *node) { return node->next; },
      [](T *node) { return node->val; });
}

}  // namespace leetcode
}  // namespace gxt
