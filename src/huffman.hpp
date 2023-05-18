#pragma once

#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
#include <memory>
#include <utility>

namespace Huffman
{
  struct TreeNode
  {
    char symbol;
    unsigned frequency;
    std::shared_ptr<TreeNode> left;
    std::shared_ptr<TreeNode> right;

    TreeNode(char symbol, unsigned frequency);

    struct Compare
    {
      bool operator()(std::shared_ptr<Huffman::TreeNode> left, std::shared_ptr<Huffman::TreeNode> right);
    };

    static std::shared_ptr<TreeNode> from(const std::string &text);
    // static std::shared_ptr<TreeNode> from(const EncodingMap &map);
  };

  class EncodingMap
  {
  private:
    std::unordered_map<char, std::string> map;

    void build(std::shared_ptr<TreeNode> node, const std::string &code = "");

  public:
    EncodingMap(std::shared_ptr<TreeNode> root);
    EncodingMap(const std::string &str_map);

    const std::string &operator[](char symbol);

    const std::unordered_map<char, std::string> &getMap() const;

    friend std::ostream &operator<<(std::ostream &os, const EncodingMap &map);
  };

  std::pair<std::string, EncodingMap> encode(const std::string &text);
}
