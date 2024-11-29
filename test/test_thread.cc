#include <gtest/gtest.h>

#include "debugstream/debugstream.h"

void TestFun(int& a) {
  CALL_ONCE([&a]() { a++; });
}

TEST(test_thread, CallOnce) {
  int a{0};
  TestFun(a);
  ASSERT_EQ(a, 1);
  TestFun(a);
  TestFun(a);
  TestFun(a);
  ASSERT_EQ(a, 1);
  CALL_ONCE([&a]() { a++; });
  ASSERT_EQ(a, 2);
  CALL_ONCE([&a]() { a++; });
  ASSERT_EQ(a, 3);
  CALL_ONCE([&a]() { a++; });
  ASSERT_EQ(a, 4);
}

TEST(test_thread, GetHardwareConcurrency) {
  ASSERT_EQ(gxt::thread::GetHardwareConcurrency(),
            std::thread::hardware_concurrency());
}

TEST(test_thread, GetCurrentID) {
  ASSERT_EQ(gxt::thread::GetCurrentID(), std::this_thread::get_id());
}
