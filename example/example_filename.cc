#include "debugstream/debugstream.h"

int main(int argc, char *argv[]) {
  std::cout << G_GET_FILE_PATH() << std::endl;
  std::cout << G_GET_FILE_FILENAME() << std::endl;
  std::cout << G_GET_FILE_BASENAME() << std::endl;
  // clang-format off
  std::cout << gxt::filename::GetFilePathWithData("abc/123.txt") << std::endl;
  std::cout << gxt::filename::GetFilePathWithData("/abc/123.txt") << std::endl;
  std::cout << gxt::filename::GetFilePathWithData("/abc/567/123.txt") << std::endl;
  std::cout << gxt::filename::GetFilePathWithData("123.txt") << std::endl;
  std::cout << gxt::filename::GetFilePathWithData("123") << std::endl;
  std::cout << gxt::filename::GetFilePathWithData("/123") << std::endl;
  std::cout << gxt::filename::GetFilePathWithData("/123/567") << std::endl;
  // clang-format on
  return 0;
}
