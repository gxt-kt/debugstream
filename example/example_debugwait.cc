#include <debugstream/debugstream.h>

G_DEBUG_WAIT_PLUGIN(5)

int main(int argc, char *argv[]) {
  gDebug() << "hello world";
  return 0;
}
