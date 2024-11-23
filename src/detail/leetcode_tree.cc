#include "detail/leetcode_tree.h"

namespace gxt {
namespace leetcode {

TreeNode* BuildTreeNode(const std::string& input_str,
                        const std::string& null_str) {
  std::string str = input_str;
  // 先去除所有空格
  str.erase(remove(str.begin(), str.end(), ' '), str.end());
  if (str.empty()) return nullptr;
  std::vector<std::string> nodes = detail::SplitStringToVector(str, ',');
  std::queue<TreeNode*> q;
  TreeNode* root = new TreeNode(stoi(nodes[0]));
  q.push(root);

  size_t i = 1;
  while (!q.empty() && i < nodes.size()) {
    TreeNode* node = q.front();
    q.pop();
    if (nodes[i] != null_str) {
      node->left = new TreeNode(stoi(nodes[i]));
      q.push(node->left);
    }
    i++;
    if (i < nodes.size() && nodes[i] != null_str) {
      node->right = new TreeNode(stoi(nodes[i]));
      q.push(node->right);
    }
    i++;
  }

  return root;
}

}  // namespace leetcode
}  // namespace gxt
