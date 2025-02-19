# debugstream

## how to use

If you have use qDebug() of "Qt" before, you must use this module easily. And the qDebug is change name to gDebug here.

You can use the default gDebug/gDebug() function to print the debug stream such as `gDebug("hello world");`
Complexly you can write like `gDebug("hello") << "world";` and so on.  The detail code you can see example.

And the default gDebug() has enable the `space` between two `operator<<` and `newline` when deconstruct the object.
If you use the class DebugStream create a new instantiation. The `space` function is exist but the `newline` is invalid.

## Install debugstream lib

**linux or macos**

```bash
cmake -S . -B build -DBUILD_TESTS=OFF -DBUILD_EXAMPLES=OFF
cmake --build build
sudo cmake --install build
```

**windows**

```bash
cmake -S . -B build -DBUILD_TESTS=OFF -DBUILD_EXAMPLES=OFF -DCMAKE_INSTALL_PREFIX="${YOUR_INSTALL_PATH}" # remember to change install path
cmake --build build --config Release
cmake --install build --config Release
```

--- 


## Use debugstream lib with cmake

**linux or macos**

```cmake
find_package(debugstream REQUIRED)
if(debugstream_FOUND)
    message(STATUS "Found debugstream:")
    message(STATUS "  - Version: ${debugstream_VERSION}")
else()
    message(FATAL_ERROR "debugstream not found!")
endif()
target_link_libraries(demo debugstream)
```

**windows**

```cmake
list(APPEND CMAKE_PREFIX_PATH  "${YOUR_INSTALL_PATH}")  # remember to change install path
find_package(debugstream REQUIRED)
if(debugstream_FOUND)
    message(STATUS "Found debugstream:")
    message(STATUS "  - Version: ${debugstream_VERSION}")
else()
    message(FATAL_ERROR "debugstream not found!")
endif()
target_link_libraries(demo debugstream)
```

## Examples

### example1

**use the default gDebug like qDebug**

```c++
#include <debugstream/debugstream.h>

int main() {
  gDebug("hello world!");
  gDebug() << "hello world!";
  gDebug() << "11" << "22" << "33";
  gDebug().NoSpace() << "11" << "22" << "33";
  gDebug().NoSpace().NoNewLine() << "11" << "22" << "33";
  gDebug() << "44";

  float f = 3.14159f;
  gDebug("f=%.3f", f);
}
```

the stream out text:

```
hello world!
hello world!
11 22 33
112233
11223344
f=3.142
```

### example2

Some high level use skills

```cpp
#include <array>
#include <map>
#include <unordered_map>
#include <vector>

#define EIGEN_ENABLE false

#if EIGEN_ENABLE
#include <Eigen/Core>
#include <Eigen/Dense>
#endif

#include <debugstream/debugstream.h>

// Operator debug for your own type
struct Test {
  int val{100};
};
gxt::DebugStream &operator<<(gxt::DebugStream &stream, Test obj) {
  gDebug().NoNewLine() << obj.val;
  return stream;
}

// Print template type
template <typename T> void Fun(T) { gDebug(G_TYPET(T)); }

int main() {
  gDebug("===================");
  gDebug() << "demo test for debugstream";
  gDebug() << G_FILE << G_LINE;
  gDebug() << G_FILE_LINE;

  gDebug("===================");
  int a{1};
  gDebug(a);
  gDebug() << "a =" << a;
  gDebug() << VAR(a);

  gDebug("===================");
  std::vector<int> b{1, 2, 3};
  gDebug(b);
  gDebug() << "b =" << b;

  gDebug("===================");
  std::unordered_map<std::string, std::vector<int>> map1{
      {"111", {1, 11, 111}}, {"222", {2, 22, 222}}, {"333", {3, 33, 333}}};
  gDebug() << map1;

  gDebug("===================");
  int aa;
  std::array<std::string, 10> array1;
  gDebug(G_TYPE(aa));
  gDebug(G_TYPE(array1));

  gDebug("===================");
  Fun(10);

#if EIGEN_ENABLE
  gDebug("===================");
  Eigen::Matrix3d matrix = decltype(matrix)::Identity();
  gDebug() << matrix;
#endif

  gDebug("===================");
  auto ret = Test();
  ret.val++;
  gDebug() << ret;

  gDebug("===================");
  gDebug() << gxt::detail::cyan_bg << gxt::detail::black_fg
           << "===================";
  gDebugWarn("warn text test");
  gDebugError("error text test"); // will stop
  gDebug("will not exec to here");
}
```

This code will generate color letter and it's better try by your own.

If you have Eigen enable, you can set EIGEN_ENABLE to true.
