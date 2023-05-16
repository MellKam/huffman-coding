#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
#include <memory>

namespace Huffman
{
  struct TreeNode
  {
  public:
    char symbol;
    unsigned frequency;
    std::shared_ptr<TreeNode> left;
    std::shared_ptr<TreeNode> right;

    TreeNode(char symbol, unsigned frequency);

    static std::shared_ptr<TreeNode> buildTree(const std::string &text);
  };

  class EncodingMap
  {
  public:
    EncodingMap(const std::string &text);

    const std::string &operator[](char symbol);

    friend std::ostream &operator<<(std::ostream &os, const EncodingMap &map);
  };

  std::pair<std::string, EncodingMap> encode(const std::string &text);
}

#endif /* HUFFMAN_H */
