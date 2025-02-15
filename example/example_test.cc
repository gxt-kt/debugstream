#include <debugstream/debugstream.h>


int main(int argc, char* argv[]) {
  // std::cout << "MY_LIB_VERSION = " << MY_LIB_VERSION << std::endl;
  gDebug("123");
  gDebugWarn("??");
  gDebug().Terminate() << gxt::detail::blue_fg << "1234";
  gDebugError("??");
}

