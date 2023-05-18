#include "./huffman.hpp"

int main()
{
  auto [encoded, map] = Huffman::encode("hello world");
  std::cout << encoded << std::endl;
  std::cout << map << std::endl;

  Huffman::writeBitsToFile("huffman.bin", encoded);
  auto data = Huffman::readBitsFromFile("huffman.bin");
  std::cout << data << std::endl;

  return 0;
}