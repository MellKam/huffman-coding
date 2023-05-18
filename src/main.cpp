#include "./huffman.hpp"

int main()
{
  std::string str_map = "e=000\nh=010\nd=011\no=111\nl=10\nw=001\nr=1100\n =1101";

  Huffman::EncodingMap map1(str_map);
  std::cout << map1 << std::endl;

  auto [encoded, map] = Huffman::encode("hello world");
  std::cout << encoded << std::endl;
  std::cout << map << std::endl;

  return 0;
}