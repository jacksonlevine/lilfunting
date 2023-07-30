#include <iostream>
#include <sstream>
#include <fstream>
int main() {
  std::ifstream file("./rando.txt");
  std::stringstream buf;
  buf << file.rdbuf();
  std::cout << buf.str();



  std::string _;
  std::cin >> _;
  return 1;
}