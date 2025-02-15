#include <gtest/gtest.h>

#include "debugstream/debugstream.h"

TEST(test_filename, define_filename_api) {
  ASSERT_STREQ(G_GET_FILE_PATH(), __FILE__);
  ASSERT_EQ(G_GET_FILE_FILENAME(), "test_filename.cc");
  ASSERT_EQ(G_GET_FILE_BASENAME(), "test_filename");
}

using namespace gxt::filename;
TEST(test_filename, ExtractPathComponents) {
  {
    std::string path = "/home/abc/def/xyz.cc";
    auto p = ExtractPathComponents(path);
    ASSERT_EQ(p.filename, "xyz.cc");
    ASSERT_EQ(p.basename, "xyz");
    ASSERT_EQ(p.directory, "/home/abc/def");
    ASSERT_EQ(p.extension, "cc");
  }
  {
    std::string path = "/home/abc/def/xyzcc";
    auto p = ExtractPathComponents(path);
    ASSERT_EQ(p.filename, "xyzcc");
    ASSERT_EQ(p.basename, "xyzcc");
    ASSERT_EQ(p.directory, "/home/abc/def");
    ASSERT_EQ(p.extension, "");
  }
  {
    std::string path = "xyzcc";
    auto p = ExtractPathComponents(path);
    ASSERT_EQ(p.filename, "xyzcc");
    ASSERT_EQ(p.basename, "xyzcc");
    ASSERT_EQ(p.directory, "");
    ASSERT_EQ(p.extension, "");
  }
  {
    std::string path = "";
    auto p = ExtractPathComponents(path);
    ASSERT_EQ(p.filename, "");
    ASSERT_EQ(p.basename, "");
    ASSERT_EQ(p.directory, "");
    ASSERT_EQ(p.extension, "");
  }
  {
    std::string path = "  ";
    auto p = ExtractPathComponents(path);
    ASSERT_EQ(p.filename, "");
    ASSERT_EQ(p.basename, "");
    ASSERT_EQ(p.directory, "");
    ASSERT_EQ(p.extension, "");
  }
  {
    std::string path = "/home/abc/def/.xyzcc";
    auto p = ExtractPathComponents(path);
    ASSERT_EQ(p.filename, ".xyzcc");
    ASSERT_EQ(p.basename, ".xyzcc");
    ASSERT_EQ(p.directory, "/home/abc/def");
    ASSERT_EQ(p.extension, "");
  }
  {
    std::string path = "/home/abc/def/.abc.conf";
    auto p = ExtractPathComponents(path);
    ASSERT_EQ(p.filename, ".abc.conf");
    ASSERT_EQ(p.basename, ".abc");
    ASSERT_EQ(p.directory, "/home/abc/def");
    ASSERT_EQ(p.extension, "conf");
  }
  {
    std::string path = ".abc";
    auto p = ExtractPathComponents(path);
    ASSERT_EQ(p.filename, ".abc");
    ASSERT_EQ(p.basename, ".abc");
    ASSERT_EQ(p.directory, "");
    ASSERT_EQ(p.extension, "");
  }
  {
    std::string path = ".abc.conf";
    auto p = ExtractPathComponents(path);
    ASSERT_EQ(p.filename, ".abc.conf");
    ASSERT_EQ(p.basename, ".abc");
    ASSERT_EQ(p.directory, "");
    ASSERT_EQ(p.extension, "conf");
  }
  {
    std::string path = "home/abc/def/xyz.cc";
    auto p = ExtractPathComponents(path);
    ASSERT_EQ(p.filename, "xyz.cc");
    ASSERT_EQ(p.basename, "xyz");
    ASSERT_EQ(p.directory, "home/abc/def");
    ASSERT_EQ(p.extension, "cc");
  }
  {
    std::string path = "h'\"om，。e/1中 文/d,)ef /xyz.cc";
    auto p = ExtractPathComponents(path);
    ASSERT_EQ(p.filename, "xyz.cc");
    ASSERT_EQ(p.basename, "xyz");
    ASSERT_EQ(p.directory, "h'\"om，。e/1中 文/d,)ef ");
    ASSERT_EQ(p.extension, "cc");
  }
}
