#include "./huffman.hpp"

int main()
{
  auto [encoded, map] = Huffman::encode("hello world");
  std::cout << encoded << std::endl;
  std::cout << map << std::endl;

  return 0;
}