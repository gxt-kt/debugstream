#include <debugstream/debugstream.h>

void Print() { std::cout << gxt::Singleton<int>::GetInstance() << std::endl; }

int main(int argc, char *argv[]) {
  gxt::Singleton<int>::GetInstance() = 10;
  std::cout << gxt::Singleton<int>::GetInstance() << std::endl;
  for (int i = 0; i < 10; i++) {
    std::thread tmp(Print);
    tmp.join();
  }
  return 0;
}
