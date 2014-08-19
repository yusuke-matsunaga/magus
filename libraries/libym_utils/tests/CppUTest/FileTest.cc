
/// @file FileTest.cc
/// @brief File.h 関係のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "CppUTest/TestHarness.h"
#include "utils/File.h"


using namespace nsYm;

TEST_GROUP(libym_utils_FileTestGroup)
{
  TEST_SETUP() {
  }

  TEST_TEARDOWN() {
  }

  void common_test(const PathName& path_name,
		   bool exp_is_valid,
		   PathName::tType exp_type,
		   const string& exp_str,
		   const PathName& exp_head,
		   const string& exp_tail,
		   const string& exp_body,
		   const string& exp_ext)
  {
    LONGS_EQUAL( exp_is_valid, path_name.is_valid() );

    LONGS_EQUAL( static_cast<ymuint>(exp_type), static_cast<ymuint>(path_name.type()) );

    string s = path_name.str();
    STRCMP_EQUAL( exp_str.c_str(), s.c_str() );

    PathName head = path_name.head();
    LONGS_EQUAL( exp_head.is_valid(), head.is_valid() );

    LONGS_EQUAL( static_cast<ymuint>(exp_head.type()), static_cast<ymuint>(head.type()) );

    string exp_head_s = exp_head.str();
    string head_s = head.str();
    STRCMP_EQUAL( exp_head_s.c_str(), head_s.c_str() );

    string t = path_name.tail();
    STRCMP_EQUAL( exp_tail.c_str(), t.c_str() );

    string b = path_name.body();
    STRCMP_EQUAL( exp_body.c_str(), b.c_str() );

    string e = path_name.ext();
    STRCMP_EQUAL( exp_ext.c_str(), e.c_str() );
  }

};

// setup(), teardown() のみで問題が起きないことのテスト
TEST(libym_utils_FileTestGroup, empty)
{
}

// 空の PathName のテスト
TEST(libym_utils_FileTestGroup, pathname_empty)
{
  PathName path_name;

  common_test(path_name,
	      false,
	      PathName::kRelative,
	      string(""),
	      PathName(),
	      string(""),
	      string(""),
	      string(""));
}

// 絶対パス指定のテスト1
TEST(libym_utils_FileTestGroup, pathname_abs1)
{
  PathName path_name("/0123/456.");

  common_test(path_name,
	      true,
	      PathName::kAbsolute,
	      string("/0123/456."),
	      PathName("/0123"),
	      string("456."),
	      string("456"),
	      string(""));
}

// 絶対パス指定のテスト2
TEST(libym_utils_FileTestGroup, pathname_abs2)
{
  PathName path_name("/xyz/0123/abcd.yyx");

  common_test(path_name,
	      true,
	      PathName::kAbsolute,
	      string("/xyz/0123/abcd.yyx"),
	      PathName("/xyz/0123"),
	      string("abcd.yyx"),
	      string("abcd"),
	      string("yyx"));
}

// 絶対パス指定のテスト3
TEST(libym_utils_FileTestGroup, pathname_abs3)
{
  PathName path_name("/abc/.xxx");

  common_test(path_name,
	      true,
	      PathName::kAbsolute,
	      string("/abc/.xxx"),
	      PathName("/abc"),
	      string(".xxx"),
	      string(".xxx"),
	      string(""));
}

// 相対パス指定のテスト4
TEST(libym_utils_FileTestGroup, pathname_abs4)
{
  PathName path_name("abcde");

  common_test(path_name,
	      true,
	      PathName::kRelative,
	      string("abcde"),
	      PathName(""),
	      string("abcde"),
	      string("abcde"),
	      string(""));
}
