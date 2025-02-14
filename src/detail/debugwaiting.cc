#include "detail/debugwaiting.h"

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <unistd.h>
#include <termios.h>
#endif

namespace gxt {
namespace detail {

void SetNonBlocking(bool enable) {
  #ifdef _WIN32
      // Windows下不需要设置非阻塞模式，直接使用_kbhit和_getch即可
  #else
      termios tty;
      tcgetattr(STDIN_FILENO, &tty);
      if (enable) {
          tty.c_lflag &= ~ICANON & ~ECHO;
          tty.c_cc[VMIN] = 0;
          tty.c_cc[VTIME] = 0;
      } else {
          tty.c_lflag |= ICANON | ECHO;
      }
      tcsetattr(STDIN_FILENO, TCSANOW, &tty);
  #endif
}

int kbhit(void) {
  #ifdef _WIN32
      return _kbhit();
  #else
      std::array<char, 1> buf;
      SetNonBlocking(true);
      auto ret = read(STDIN_FILENO, buf.data(), 1);
      SetNonBlocking(false);
      if (ret > 0) {
          // 把读到的字符放回去，以便后面的读取操作能够获取到它
          ungetc(buf[0], stdin);
          return 1;
      }
      return 0;
  #endif
}
  

void DebugWaitPlugin(int time_s) {
  assert(time_s >= 0);
  std::cout << "[GXT DEBUG]: debug wait plugin enable, THREAD_ID: " << getpid()
            << " TIMEOUT: <" << time_s << "s>" << std::endl;
#if defined(NDEBUG)
  std::cout << "[GXT DEBUG]: Current is <Release> or <RelWithDebInfo> Mode"
            << std::endl;
#else
  std::cout << "[GXT DEBUG]: Current is <Debug> Mode" << std::endl;
#endif
  std::cout << "[GXT DEBUG]: Press 'Enter' to skip waiting, 'q' or 'ESC' to "
               "exit, 'c' to "
               "toggle unlimited wait."
            << std::endl;
  setbuf(stdout, NULL);  // set stdout can output when debug

  using namespace std::chrono;

  auto start = steady_clock::now();
  bool unlimited_wait = false;

  SetNonBlocking(true);

  while (true) {
      if (kbhit()) {
#ifdef _WIN32
          char ch = _getch();
#else
          char ch = getchar();
#endif
          if (ch == '\r' || ch == '\n') {  // Enter
              std::cout << "[GXT DEBUG]: Continue Exec!" << std::endl;
              std::cout << "[GXT DEBUG]: ====================" << std::endl;
              break;
          } else if (ch == 'q' || ch == 27) {  // 'q' or ESC
              std::cout << "[GXT DEBUG]: Termiate and Exit!" << std::endl;
              SetNonBlocking(false);
              std::terminate();
          } else if (ch == 'c') {
              if (unlimited_wait) {
                  std::cout << "\n[GXT DEBUG]: Continue Exec!" << std::endl;
                  std::cout << "[GXT DEBUG]: ====================" << std::endl;
                  break;
              }
              std::cout << "\n[GXT DEBUG]: Unlimited Waiting! Press 'c' again to "
                           "continue!"
                        << std::endl;
              unlimited_wait = !unlimited_wait;
          }
      }

      if (!unlimited_wait) {
          auto now = steady_clock::now();
          auto elapsed = duration_cast<seconds>(now - start);
          if (elapsed.count() >= time_s) {
              std::cout << "\n[GXT DEBUG]: Time Out! Continue Exec!" << std::endl;
              std::cout << "[GXT DEBUG]: ====================" << std::endl;
              break;
          }
      }

      if (!unlimited_wait) std::cout << '.' << std::flush;
      std::this_thread::sleep_for(milliseconds(100));  // 减少 CPU 使用
  }

  SetNonBlocking(false);
}

}  // namespace detail
}  // namespace gxt
