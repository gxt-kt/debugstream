#include "detail/leetcode_list.h"

namespace gxt {
namespace leetcode {

ListNode *BuildList(const std::string &input_str) {
  std::string str = input_str;
  // 先去除所有空格
  str.erase(remove(str.begin(), str.end(), ' '), str.end());
  if (str.empty()) return nullptr;
  std::vector<std::string> nodes = detail::SplitStringToVector(str, ',');
  ListNode *dummy = new ListNode(0);
  ListNode *curr = dummy;
  for (const auto &node_str : nodes) {
    curr->next = new ListNode(stoi(node_str));
    curr = curr->next;
  }
  return dummy->next;
}

}  // namespace leetcode
}  // namespace gxt
