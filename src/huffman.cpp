#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
#include <memory>

namespace Huffman
{
  struct TreeNode
  {
  private:
    struct Compare
    {
      bool operator()(std::shared_ptr<TreeNode> left, std::shared_ptr<TreeNode> right)
      {
        return left->frequency > right->frequency;
      }
    };

  public:
    char symbol;
    unsigned frequency;
    std::shared_ptr<TreeNode> left;
    std::shared_ptr<TreeNode> right;

    TreeNode(char symbol, unsigned frequency)
        : symbol(symbol), frequency(frequency) {}

    static std::shared_ptr<TreeNode> buildTree(const std::string &text)
    {
      std::unordered_map<char, unsigned> frequencyTable;
      for (char c : text)
      {
        ++frequencyTable[c];
      }

      std::priority_queue<std::shared_ptr<TreeNode>, std::vector<std::shared_ptr<TreeNode>>, Compare> queue;

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
  };

  class EncodingMap
  {
  private:
    std::unordered_map<char, std::string> map;

    void build(std::shared_ptr<Huffman::TreeNode> node, std::string code)
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

  public:
    EncodingMap(const std::string &text)
    {
      auto root = TreeNode::buildTree(text);
      build(root, "");
    }

    const std::string &operator[](char symbol)
    {
      return map[symbol];
    }

    friend std::ostream &operator<<(std::ostream &os, const EncodingMap &map)
    {
      for (auto &&[key, value] : map.map)
      {
        std::cout << key << "=" << value << std::endl;
      }
      return os;
    }
  };

  std::pair<std::string, EncodingMap> encode(const std::string &text)
  {
    EncodingMap map = EncodingMap(text);

    std::string encodedText;
    for (char c : text)
    {
      encodedText += map[c];
    }

    return std::make_pair(encodedText, map);
  }
}