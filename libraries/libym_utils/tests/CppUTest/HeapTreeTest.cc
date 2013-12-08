
/// @file HeapTreeTest.cc
/// @brief HeapTree のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "CppUTest/TestHarness.h"
#include "ym_utils/HeapTree.h"


using namespace nsYm;


// 整数用の比較オブジェクト
struct CompFunc1
{
  int
  operator()(int a,
	     int b)
  {
    return a - b;
  }
};

// 文字列用の比較オブジェクト
struct CompFunc2
{
  int
  operator()(const char* a,
	     const char* b)
  {
    return strcmp(a, b);
  }
};


TEST_GROUP(libym_utils_HeapTreeTestGroup)
{
  TEST_SETUP() {
  }

  TEST_TEARDOWN() {
  }
};

// 空のテスト
TEST(libym_utils_HeapTreeTestGroup, empty)
{
}

// 整数データを用いたテスト
TEST(libym_utils_HeapTreeTestGroup, int1)
{
  HeapTree<int, CompFunc1> ht;

  ht.put(1);
  ht.put(3);
  ht.put(5);
  ht.put(2);
  ht.put(3);
  ht.put(9);
  ht.put(6);
  ht.put(7);
  ht.put(4);

  int prev = 0;
  while ( !ht.empty() ) {
    int a = ht.getmin();
    CHECK( prev <= a );
    ht.popmin();
    prev = a;
  }
}

// 文字列データを用いたテスト
TEST(libym_utils_HeapTreeTestGroup, str1)
{
  HeapTree<const char*, CompFunc2> ht;

  ht.put("abc");
  ht.put("ab");
  ht.put("a");
  ht.put("xyz");
  ht.put("pqr");
  ht.put("this");
  ht.put("quick");
  ht.put("brown");
  ht.put("fox");
  ht.put("jumped");
  ht.put("over");
  ht.put("a");
  ht.put("lazy");
  ht.put("dog");

  const char* prev = "";
  while ( !ht.empty() ) {
    const char* a = ht.getmin();
    CHECK( strcmp(prev, a) <= 0 );
    ht.popmin();
    prev = a;
  }
}

// 複合データを用いたテスト
TEST(libym_utils_HeapTreeTestGroup, cmp1)
{
  SimpleHeapTree<const char*> ht;
}
