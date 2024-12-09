#include "debugstream.h"

namespace test01 {
void Test(int cnt = 1000) {
  gDebug() << "gDebugLog";
  while (cnt--) {
    gDebugLog() << VAR(cnt);
  }
}
}  // namespace test01

namespace test02 {
void Test(int cnt = 1000) {
  gDebug() << "gDebugLog";
  while (cnt--) {
    gDebugCout() << VAR(cnt);
  }
}
}  // namespace test02

namespace test03 {
void Test(int cnt = 1000) {
  gDebug() << "gDebugLog";
  while (cnt--) {
    gDebugLogCout() << VAR(cnt);
  }
}
}  // namespace test03

namespace test04 {
void Test(int cnt = 1000) {
  gxt::log::Logger mylog(true, true, "mylog", false);
  auto fun = [&mylog](const std::string& str) { mylog.log(str); };
#undef mylog
#define mylog(...)                                                            \
  ((gxt::DebugStream(fun).NewLine() << gxt::GetCurrentTime() << G_FILE_LINE)( \
      #__VA_ARGS__ __VA_OPT__(, ) __VA_ARGS__))
  gDebug() << "mylog1";
  while (cnt--) {
    mylog() << VAR(cnt);
  }
}
}  // namespace test04

namespace test05 {
void Test(int cnt = 1000) {
  gxt::log::Logger mylog1(true, true, "mylog", true);
  auto fun = [&mylog1](const std::string& str) { mylog1.log(str); };
#undef mylog
#define mylog(...)                                                            \
  ((gxt::DebugStream(fun).NewLine() << gxt::GetCurrentTime() << G_FILE_LINE)( \
      #__VA_ARGS__ __VA_OPT__(, ) __VA_ARGS__))
  gDebug() << "mylog2";
  while (cnt--) {
    mylog() << VAR(cnt);
  }
}
}  // namespace test05

namespace test06 {
void Test(int cnt = 1000) {
  auto LogTestFun = [](const std::string& str) {
    static gxt::log::Logger testlog(false, true, "LogTestFun", true);
    testlog.log(str);
  };
#undef mylog
#define mylog(...)                         \
  ((gxt::DebugStream(LogTestFun).NewLine() \
    << gxt::GetCurrentTime()               \
    << G_FILE_LINE)(#__VA_ARGS__ __VA_OPT__(, ) __VA_ARGS__))
  gDebug() << "LogTestFun";
  while (cnt--) {
    mylog() << VAR(cnt);
  }
}
}  // namespace test06

int main(int argc, char* argv[]) {
  test01::Test();
  test02::Test();
  test03::Test();
  {
    // wait write to file completely
    gxt::Sleep(3);
  }
}
