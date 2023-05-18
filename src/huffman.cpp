#include "huffman.hpp"

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

      auto newNode = std::make_shared<TreeNode>();
      newNode->frequency = left->frequency + right->frequency;
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

  std::string decode(const std::string &data, const std::string &str_map)
  {
    std::unordered_map<std::string, char> map;

    std::istringstream iss(str_map);
    std::string line;

    while (std::getline(iss, line))
    {
      if (!line.empty())
      {
        char symbol = line[0];
        std::string code = line.substr(2);
        map[code] = symbol;
      }
    }

    std::string decodedResult;
    std::string currCode;

    for (char c : data)
    {
      currCode += c;
      if (map[currCode])
      {
        decodedResult += map[currCode];
        currCode.clear();
      }
    }

    return decodedResult;
  }

  void writeBitsToFile(const std::string &filename, const std::string &bits)
  {
    std::ofstream outputFile(filename, std::ios::binary);

    if (!outputFile.is_open())
    {
      throw std::runtime_error("Failed to open the file " + filename);
    }

    std::size_t numBytes = bits.length() / 8;
    if (bits.length() % 8 != 0)
    {
      numBytes++;
    }

    outputFile.write(reinterpret_cast<const char *>(&numBytes), sizeof(numBytes));

    for (std::size_t i = 0; i < bits.length(); i += 8)
    {
      std::bitset<8> byte(bits.substr(i, 8));
      outputFile.write(reinterpret_cast<const char *>(&byte), 1);
    }

    outputFile.close();
  };

  std::string readBitsFromFile(const std::string &filename)
  {
    std::ifstream inputFile(filename, std::ios::binary);

    if (!inputFile.is_open())
    {
      throw std::runtime_error("Failed to open the file " + filename);
    }

    std::size_t numBytes;
    inputFile.read(reinterpret_cast<char *>(&numBytes), sizeof(numBytes));

    std::string bits;
    for (std::size_t i = 0; i < numBytes; ++i)
    {
      std::bitset<8> byte;
      inputFile.read(reinterpret_cast<char *>(&byte), 1);
      bits += byte.to_string();
    }

    inputFile.close();
    return bits;
  }
}
