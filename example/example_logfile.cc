#include "debugstream.h"

int main(int argc, char* argv[]) {
  const int n = 1000;

  {
    gDebug() << "gDebugLog";
    int cnt = n;
    while (cnt--) {
      gDebugLog() << VAR(cnt);
    }
  }

  {
    gDebug() << "gDebugCout";
    int cnt = n;
    while (cnt--) {
      gDebugCout() << VAR(cnt);
    }
  }

  {
    gDebug() << "gDebugLogCout";
    int cnt = n;
    while (cnt--) {
      gDebugLogCout() << VAR(cnt);
    }
  }

  {
    // define my logfile
    gxt::Logger mylog1(true, true, "mylog1", false);
    auto fun = [&mylog1](const std::string& str) { mylog1.log(str); };
#undef mylog
#define mylog(...)                                                            \
  ((gxt::DebugStream(fun).NewLine() << gxt::GetCurrentTime() << G_FILE_LINE)( \
      #__VA_ARGS__ __VA_OPT__(, ) __VA_ARGS__))
    gDebug() << "mylog1";
    int cnt = n;
    while (cnt--) {
      mylog() << VAR(cnt);
    }
  }

  {
    // define my logfile
    gxt::Logger mylog2(true, true, "mylog2", true);
    auto fun = [&mylog2](const std::string& str) { mylog2.log(str); };
#undef mylog
#define mylog(...)                                                            \
  ((gxt::DebugStream(fun).NewLine() << gxt::GetCurrentTime() << G_FILE_LINE)( \
      #__VA_ARGS__ __VA_OPT__(, ) __VA_ARGS__))
    gDebug() << "mylog2";
    int cnt = n;
    while (cnt--) {
      mylog() << VAR(cnt);
    }
  }

  {
    auto LogTestFun = [](const std::string& str) {
      static gxt::Logger testlog(false, true, "LogTestFun", true);
      testlog.log(str);
    };
#undef mylog
#define mylog(...)                         \
  ((gxt::DebugStream(LogTestFun).NewLine() \
    << gxt::GetCurrentTime()               \
    << G_FILE_LINE)(#__VA_ARGS__ __VA_OPT__(, ) __VA_ARGS__))

    // define my logfile
    gDebug() << "LogTestFun";
    int cnt = n;
    while (cnt--) {
      mylog() << VAR(cnt);
    }
  }

  {
    // wait write to file completely
    gxt::Sleep(3);
  }
}
