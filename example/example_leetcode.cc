#include "debugstream/debugstream.h"
#include "debugstream/detail/leetcode.h"

using treenode = gxt::leetcode::TreeNode;
int main(int argc, char *argv[]) {
  {
    {
      auto tree_node = gxt::leetcode::BuildTreeNode(
          "1,2,3,null,5,null,null,null,9,10,11", "null");
      std::cout << G_SPLIT_LINE() << std::endl;
      std::cout << gxt::leetcode::PrintTree1(tree_node) << std::endl;
      std::cout << G_SPLIT_LINE() << std::endl;
      std::cout << gxt::leetcode::PrintTree2(tree_node) << std::endl;
      std::cout << G_SPLIT_LINE() << std::endl;
      std::cout << gxt::leetcode::PrintTree3(tree_node) << std::endl;
      std::cout << G_SPLIT_LINE() << std::endl;
    }
    {
      auto tree_node_null = gxt::leetcode::BuildTreeNode("");
      std::cout << G_SPLIT_LINE() << std::endl;
      std::cout << gxt::leetcode::PrintTree1(tree_node_null) << std::endl;
      std::cout << G_SPLIT_LINE() << std::endl;
      std::cout << gxt::leetcode::PrintTree2(tree_node_null) << std::endl;
      std::cout << G_SPLIT_LINE() << std::endl;
      std::cout << gxt::leetcode::PrintTree3(tree_node_null) << std::endl;
      std::cout << G_SPLIT_LINE() << std::endl;
    }
  }
  {
    {
      auto list = gxt::leetcode::BuildList("1,2,3,4,5,6,7,8,9,10");
      std::cout << G_SPLIT_LINE() << std::endl;
      std::cout << gxt::leetcode::PrintList(list) << std::endl;
      std::cout << G_SPLIT_LINE() << std::endl;
    }
    {
      auto list_null = gxt::leetcode::BuildList("");
      std::cout << G_SPLIT_LINE() << std::endl;
      std::cout << gxt::leetcode::PrintList(list_null) << std::endl;
      std::cout << G_SPLIT_LINE() << std::endl;
    }
  }
  return 0;
}
