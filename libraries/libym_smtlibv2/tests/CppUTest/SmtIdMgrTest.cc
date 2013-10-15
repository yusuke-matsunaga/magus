
/// @file SmtIdMgrTest.cc
/// @brief SmtIdMgr のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "CppUTest/TestHarness.h"
#include "CppUTest/CommandLineTestRunner.h"

#include "ym_smtlibv2/SmtId.h"
#include "SmtIdMgr.h"


using namespace nsYm;
using namespace nsYm::nsSmtLibV2;

TEST_GROUP(SmtIdMgrTestGroup)
{
  SmtIdMgr* IdMgr;

  void setup()
  {
    IdMgr = new SmtIdMgr;
  }

  void teardown()
  {
    delete IdMgr;
  }
};

TEST(SmtIdMgrTestGroup, empty)
{
}

TEST(SmtIdMgrTestGroup, make_id_simple)
{
  // 単純な形式の識別子を作る．
  const SmtId* id1 = IdMgr->make_id(ShString("a"));

  // 名前が等しいかチェック
  STRCMP_EQUAL( "a", static_cast<const char*>(id1->name()) );

  // ShString として等しいかチェック
  LONGS_EQUAL( ShString("a").id(), id1->name().id() );

  // インデックスのサイズをチェック
  LONGS_EQUAL( 0, id1->index_size() );
}

#if 0
TEST(SmtIdMgrTestGroup, make_id_identity)
{
  // 単純な形式の識別子を作る．
  const SmtId* id1 = IdMgr->make_id(ShString("a"));

  // 同じ名前の識別子を作る．
  const SmtId* id2 = IdMgr->make_id(ShString("a"));

  // id1 と id2 は同じもののはず．
  LONGS_EQUAL( id1->id(), id2->id() );
}

TEST(SmtIdMgrTestGroup, make_id_complex1)
{
  // インデックス付きの識別子を作る．
  const SmtId* id1 = IdMgr->make_id(ShString("x"), vector<ymuint32>(1, 0));

  // 名前が等しいかチェック
  STRCMP_EQUAL( "x", static_cast<const char*>(id1->name()) );

  // ShString として等しいかチェック
  LONGS_EQUAL( ShString("x").id(), id1->name().id() );

  // インデックスのサイズをチェック
  LONGS_EQUAL( 1, id1->index_size() );

  // インデックスをチェック
  LONGS_EQUAL( 0, id1->index(0) );
}
#endif

int
main(int argc,
     char** argv)
{
  //MemoryLeakWarningPlugin::turnOffNewDeleteOverloads();
  RUN_ALL_TESTS(argc, argv);
}
