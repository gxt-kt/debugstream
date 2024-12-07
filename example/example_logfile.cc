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
}
