# gDebugV2.0

The current  github address is at https://github.com/gxt-kt/gDebugV2.0

The gDebugV1.0 code is at https://github.com/gxt-kt/gDebug/ and is deprecated.

***

## how to use

If you have use qDebug() of "Qt" before, you must use this module easily. And the qDebug is change name to gDebug here.

You can use the default gDebug/gDebug() function to print the debug stream such as `gDebug("hello world");`
Complexly you can write like `gDebug("hello") << "world";` and so on.  The detail code you can see example.

And the default gDebug() has enable the `space` between two `operator<<` and `newline` when deconstruct the object.
If you use the class DebugStream create a new instantiation. The `space` function is exist but the `newline` is invalid.

## example1

**use the default qDebug**

```c++
#include "debugstream.hpp"

int main() {
  gDebug("hello world!");
  gDebug << "hello world!";
  gDebug << "11" << "22" << "33";
  gDebug.NoSpace() << "11" << "22" << "33";
  gDebug.NoSpace().NoNewLine() << "11" << "22" << "33";
  gDebug << "44";

  float f = 3.14159f;
  gDebug("f=%.3f", f);
  gDebug.NoSpace() << "f=" << gDebug("%.3f", f);
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
f=3.142

```

## example2

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

#include "debugstream.hpp"

// Operator debug for your own type
struct Test {
  int val{100};
};
gxt::DebugStream& operator<<(gxt::DebugStream& stream, Test obj) {
  gDebug().NoNewLine() << obj.val;
  return stream;
}

// Print template type
template <typename T>
void Fun(T) {
  gDebug(TYPET(T));
}

int main() {
  gDebug("===================");
  gDebug() << "demo test for debugstream";
  gDebug() << FILE << LINE;
  gDebug() << FILE_LINE;

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
  gDebug(TYPE(aa));
  gDebug(TYPE(array1));

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
  gDebug() << gxt::cyan_bg << gxt::black_fg << "===================";
  gDebugWarn("warn text test");
  gDebugError("error text test");  // will stop
  gDebug("will not exec to here");
}

```

This code will generate color letter and it's better try by your own.

If you have Eigen enable, you can set EIGEN_ENABLE to true.

## example3


*More advanced usage. Generally not used.*


**create your own instantiation**

Attention that if you create a new instantiation, The space funciton is exist but the auto newline is invalid.

(It's because that add newline is accur in the destory construct, but the instantiation will not destory automatically)

```c++
#include "debugstream.hpp"

int main() {
  DebugStream mystream;

  mystream("hello world!\n");
  mystream << "11" << "22" << "33" << "44";
  mystream << "11" << "22" << "33" << "44" << gxt::endl;
  mystream.NoSpace() << "11" <<  "22" << "33" << "44"<< gxt::endl;
  mystream.Space();
  mystream << "11" << mystream("22 33 44");
}
```

the stream out text:

```
hello world!
1122 33 44 11 22 33 44
11223344
11 22 33 44
```



## example4

*More advanced usage. Generally not used.*

**create a new instantiation with your stream out function**

step:
1. Define a function with type `void YourFunName(const char *str, int num) {
   // ... such as  printf("%.*s",num,str); }`
2. Instantiate class `DebugStream demo(YourFunName,256);` the first var is your function name, the second var is the buffer size (default 256).

```c++
#include "debugstream.hpp"

void YourFunName(const char *str, int num) {
  printf("%.*s",num,str);
}

int main() {
  DebugStream demo(YourFunName,256);
  demo << "hello world!" << gxt::endl;
}
```

the stream out text:

```
hello world!


```

## something else

You can define the `NO_DEBUG_OUTPUT` , and there will not stream out. Notice that this will disable all the instantiation's streamout.

You can use `mystream.out_en(false);` to disable the current instantiation's stream out.

