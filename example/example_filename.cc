#include "debugstream/debugstream.h"


int main (int argc, char *argv[]) {
  std::cout << G_GET_FILE_PATH() << std::endl;
  std::cout << G_GET_FILE_FILENAME() << std::endl;
  std::cout << G_GET_FILE_BASENAME() << std::endl;
  return 0;
}
