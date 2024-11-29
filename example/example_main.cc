#include "debugstream.h"
#include "fmt.h"
#include "detail/debughelper.h"

// G_DEBUG_WAIT_PLUGIN();

#define MyDebugCol(...) (DebugStream()(#__VA_ARGS__ __VA_OPT__(, ) __VA_ARGS__))
#define MyDebug(...) DebugCol(__VA_ARGS__)

int main(int argc, char* argv[]) {
  std::cout << "MY_LIB_VERSION = " << MY_LIB_VERSION << std::endl;
  std::cout << G_FILE_LINE << std::endl;
  std::cout << G_LINE << std::endl;
  std::cout << G_FILE << std::endl;
  std::cout << G_TYPET(int) << std::endl;
  int a = 10;
  // MyDebugCol(a);
  // MyDebugCol(a,b);
  // MyDebugCol(a,b);
  // MyDebugCol(std::vector<int>{1,2,3},b);
  // std::string tmp;
  // MyDebugCol(tmp,b);
  // MyDebugCol("123%d", 2);
  // MyDebugCol("123%f", 2,3);
  gDebug(a);
  gDebug() << "123";
  gDebug() << "456";
  gDebugCol1() << "456";
  gDebugCol2() << "456";
  gDebugCol2("123%d", 2) << "456";
  gDebugWarn("123%d", 2) << "456";
  gDebugWarn("123%f", 2.13f);
  gDebugWarn("123 %f %d", 2.13f, 5);
  // gDebug() << fmt::format("{}",123124.5);
  gDebug() << gxt::format("{:.3f}",123124.5);

  gDebugWarn("123") << gDebug("%d",123);

  TIME_BEGIN_MS();
  int n=1000;
  while(n--) {
    gDebugLog() << VAR(n);
  }
  TIME_END();
}
