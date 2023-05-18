#include "huffman.hpp"

#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
#include <memory>
#include <sstream>
#include <utility>

namespace Huffman
{
  bool TreeNode::Compare::operator()(
      std::shared_ptr<TreeNode> left,
      std::shared_ptr<TreeNode> right)
  {
    return left->frequency > right->frequency;
  };

  TreeNode::TreeNode(char symbol, unsigned frequency)
      : symbol(symbol), frequency(frequency) {}

  std::shared_ptr<TreeNode> TreeNode::from(
      const std::string &text)
  {
    std::unordered_map<char, unsigned> frequencyTable;
    for (char c : text)
    {
      ++frequencyTable[c];
    }

    std::priority_queue<std::shared_ptr<TreeNode>, std::vector<std::shared_ptr<TreeNode>>, TreeNode::Compare> queue;

    for (const auto &[symbol, frequency] : frequencyTable)
    {
      queue.push(std::make_shared<TreeNode>(symbol, frequency));
    }

    while (queue.size() > 1)
    {
      auto left = queue.top();
      queue.pop();

      auto right = queue.top();
      queue.pop();

      auto newNode = std::make_shared<TreeNode>('$', left->frequency + right->frequency);
      newNode->left = left;
      newNode->right = right;

      queue.push(newNode);
    }

    return queue.top();
  }

  EncodingMap::EncodingMap(std::shared_ptr<TreeNode> root)
  {
    build(root);
  }

  // recursive method
  void EncodingMap::build(std::shared_ptr<TreeNode> node, const std::string &code)
  {
    if (node.get() == nullptr)
      return;

    if (node->left == nullptr && node->right == nullptr)
    {
      map[node->symbol] = code;
      return;
    }

    build(node->left, code + "0");
    build(node->right, code + "1");
  }

  EncodingMap::EncodingMap(const std::string &str_map)
  {
    std::istringstream iss(str_map);
    std::string line;

    while (std::getline(iss, line))
    {
      if (!line.empty())
      {
        char symbol = line[0];
        std::string code = line.substr(2);
        map[symbol] = code;
      }
    }
  };

  const std::string &EncodingMap::operator[](char symbol)
  {
    return map[symbol];
  }

  const std::unordered_map<char, std::string> &EncodingMap::getMap() const
  {
    return map;
  }

  std::ostream &operator<<(std::ostream &os, const EncodingMap &map)
  {
    for (auto &&[key, value] : map.map)
    {
      std::cout << key << "=" << value << std::endl;
    }
    return os;
  }

  std::pair<std::string, EncodingMap> encode(const std::string &text)
  {
    std::shared_ptr<TreeNode> root = TreeNode::from(text);
    EncodingMap map(root);

    std::string encodedText;
    for (char c : text)
    {
      encodedText += map[c];
    }

    return std::make_pair(encodedText, map);
  }
}
