#pragma once

#include "../stdc++.h"

namespace gxt {

// 单例类
template <typename T>
class Singleton {
 public:
  // 禁止拷贝构造和赋值
  Singleton(const Singleton&) = delete;
  Singleton& operator=(const Singleton&) = delete;
  // 获取实例的方法
  static T& GetInstance() {
    static T instance;  // 局部静态变量
    return instance;
  }

 private:
  // 私有构造函数
  Singleton() {}
  ~Singleton() {}
};

};  // namespace gxt
