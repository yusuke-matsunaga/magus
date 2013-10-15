
/// @file SmtSortMgrTest.cc
/// @brief SmtSortMgr のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "CppUTest/TestHarness.h"

#include "ym_smtlibv2/SmtId.h"
#include "ym_smtlibv2/SmtSort.h"
#include "SmtIdMgr.h"
#include "SmtSortMgr.h"
#include "ym_utils/SimpleAlloc.h"


using namespace nsYm;
using namespace nsYm::nsSmtLibV2;

TEST_GROUP(SmtSortMgrTestGroup)
{
  SmtIdMgr* IdMgr;
  SimpleAlloc* alloc;
  SmtSortMgr* SortMgr;

  TEST_SETUP() {
    IdMgr = new SmtIdMgr;
    alloc = new SimpleAlloc(4096);
    SortMgr = new SmtSortMgr(*alloc, 0, NULL);
  }

  TEST_TEARDOWN() {
    delete IdMgr;
    delete SortMgr;
    delete alloc;
    ShString::free_all_memory();
  }
};

// setup(), teardown() でメモリリークが発生していないことを確認するためのテスト
TEST(SmtSortMgrTestGroup, empty)
{
}

// 型を登録するテスト
TEST(SmtSortMgrTestGroup, simple_reg)
{
  const SmtId* id = IdMgr->make_id(ShString("a"));
  bool stat = SortMgr->reg_sort(id, 2);
  // これは成功するはず
  CHECK( stat );

  // 同じ名前でもう一度登録
  bool stat2 = SortMgr->reg_sort(id, 2);
  // これは失敗するはず
  CHECK( !stat2 );
}

// 単純な型を作るテスト
TEST(SmtSortMgrTestGroup, simple_sort)
{
  // a という単純な型を作る．
  const SmtId* id = IdMgr->make_id(ShString("a"));
  (void) SortMgr->reg_sort(id, 0);
  const SmtSort* sort = SortMgr->make_sort(id);

  // sort は NULL ではないはず
  CHECK( sort != NULL );

  // sort の名前は "a" を表す識別子のはず．
  LONGS_EQUAL( id, sort->name() );

  // パラメータではない．
  CHECK( !sort->is_param() );

  // sort の要素数は0
  LONGS_EQUAL( 0, sort->elem_num() );

  // sort_str() のチェック
  string s = sort_str(sort);
  STRCMP_EQUAL( "a", s.c_str() );

  const SmtId* id2 = IdMgr->make_id(ShString("b"));
  (void) SortMgr->reg_sort(id2, 0);
  const SmtSort* sort1 = SortMgr->make_sort(id2);
  vector<const SmtSort*> elem_list(1);
  elem_list[0] = sort1;
  // (a (b)) という型を作る．
  const SmtSort* sort2 = SortMgr->make_sort(id, elem_list);
  // もちろん失敗するはず．
  CHECK( sort2 == NULL );
}

// 複雑な型を作るテスト
TEST(SmtSortMgrTestGroup, complex_sort)
{
  // b という単純な型を作る．
  const SmtId* id_b = IdMgr->make_id(ShString("b"));
  (void) SortMgr->reg_sort(id_b, 0);
  const SmtSort* sort_b = SortMgr->make_sort(id_b);

  const SmtId* id_a = IdMgr->make_id(ShString("a"));
  (void) SortMgr->reg_sort(id_a, 1);
  vector<const SmtSort*> elem_list(1);
  elem_list[0] = sort_b;
  // a (b) という型を作る．
  const SmtSort* sort_a = SortMgr->make_sort(id_a, elem_list);

  // これは成功するはず
  CHECK( sort_a != NULL );

  // sort の名前は a のはず．
  LONGS_EQUAL( id_a, sort_a->name() );

  // パラメータではない．
  CHECK( !sort_a->is_param() );

  // 要素数は 1
  LONGS_EQUAL( 1, sort_a->elem_num() );

  // 0 番目の要素型は sort_b のはず．
  CHECK( sort_b = sort_a->elem(0) );

  // sort_str() のチェック
  string s = sort_str(sort_a);
  STRCMP_EQUAL( "a ( b )", s.c_str() );
}

// 単純な型の同一性テスト
TEST(SmtSortMgrTestGroup, simple_identity)
{
  // a という単純な型を作る．
  const SmtId* id_a = IdMgr->make_id(ShString("a"));
  (void) SortMgr->reg_sort(id_a, 0);
  const SmtSort* sort_a = SortMgr->make_sort(id_a);

  // もう一回 a という単純な型を作る．
  const SmtSort* sort_a1 = SortMgr->make_sort(id_a);

  // この2つは同一のはず．
  CHECK( sort_a == sort_a1 );
}

// 複雑な型の同一性テスト
TEST(SmtSortMgrTestGroup, complex_identity)
{
  // b という単純な型を作る．
  const SmtId* id_b = IdMgr->make_id(ShString("b"));
  (void) SortMgr->reg_sort(id_b, 0);
  const SmtSort* sort_b = SortMgr->make_sort(id_b);

  // a (b) という型を作る．
  const SmtId* id_a = IdMgr->make_id(ShString("a"));
  (void) SortMgr->reg_sort(id_a, 1);
  vector<const SmtSort*> elem_list(1);
  elem_list[0] = sort_b;
  const SmtSort* sort_a = SortMgr->make_sort(id_a, elem_list);

  // もう一回 a(b) という型を作る．
  vector<const SmtSort*> elem_list1(1);
  elem_list1[0] = sort_b;
  const SmtSort* sort_a1 = SortMgr->make_sort(id_a, elem_list1);

  // この2つは同一のはず．
  CHECK( sort_a == sort_a1 );
}

// 型パラメータの生成テスト
TEST(SmtSortMgrTestGroup, param)
{
  const SmtSort* sort = SortMgr->make_param_sort(0);

  // パラメータ型のはず．
  CHECK( sort->is_param() );

  // 要素数は 0
  LONGS_EQUAL( 0, sort->elem_num() );

  // パラメータ番号は 0
  LONGS_EQUAL( 0, sort->param_id() );

  // sort_str() のチェック
  string s = sort_str(sort);
  STRCMP_EQUAL( "#0", s.c_str() );

  const SmtSort* sort1 = SortMgr->make_param_sort(1);

  // パラメータ番号は 1
  LONGS_EQUAL( 1, sort1->param_id() );

  // sort_str() のチェック
  string s1 = sort_str(sort1);
  STRCMP_EQUAL( "#1", s1.c_str() );
}

// 型エイリアスの登録テスト
TEST(SmtSortMgrTestGroup, alias1)
{
  // パラメータ型の生成
  const SmtSort* param0 = SortMgr->make_param_sort(0);

  // 型テンプレートを作る．
  // a (p0, p0)
  const SmtId* id_a = IdMgr->make_id(ShString("a"));
  (void) SortMgr->reg_sort(id_a, 2);
  vector<const SmtSort*> elem_list(2);
  elem_list[0] = param0;
  elem_list[1] = param0;
  const SmtSort* sort_tmpl = SortMgr->make_sort(id_a, elem_list);

  // これは成功するはず
  CHECK( sort_tmpl != NULL );

  // エイリアスを登録する．
  // (define-sort b (p0) (a p0 p0))
  const SmtId* id_b = IdMgr->make_id(ShString("b"));
  bool stat = SortMgr->reg_alias(id_b, sort_tmpl);

  // これは成功するはず．
  CHECK( stat );

  // sort_tmpl を実体化する．
  // ( b  x ) = (a x x )
  const SmtId* id_x = IdMgr->make_id(ShString("x"));
  (void) SortMgr->reg_sort(id_x, 0);
  const SmtSort* sort_x = SortMgr->make_sort(id_x);

  vector<const SmtSort*> elem_list1(1);
  elem_list1[0] = sort_x;
  const SmtSort* sort_bx = SortMgr->make_sort(id_b, elem_list1);

  // これは成功するはず．
  CHECK( sort_bx != NULL );

  // sort_bx の名前は id_a
  LONGS_EQUAL( id_a->id(), sort_bx->name()->id() );

  // sort_bx の要素数は2
  LONGS_EQUAL( 2, sort_bx->elem_num() );

  // sort_bx の0番めの要素は sort_x
  CHECK( sort_bx->elem(0) == sort_x );

  // sort_bx の1番目の要素は sort_x
  CHECK( sort_bx->elem(1) == sort_x );

  // sort_str() のチェック
  string s = sort_str(sort_bx);
  STRCMP_EQUAL( "a ( x x )", s.c_str() );

}

// エイリアスの登録テスト2
TEST(SmtSortMgrTestGroup, alias2)
{
  // パラメータ型の生成
  const SmtSort* param1 = SortMgr->make_param_sort(1);

  // 型テンプレートを作る．
  // a (p1)
  const SmtId* id_a = IdMgr->make_id(ShString("a"));
  (void) SortMgr->reg_sort(id_a, 1);
  vector<const SmtSort*> elem_list(1);
  elem_list[0] = param1;
  const SmtSort* sort_tmpl = SortMgr->make_sort(id_a, elem_list);

  // これは歯抜けなので失敗するはず．
  CHECK( sort_tmpl != NULL );
}

// エイリアスの登録テスト3
TEST(SmtSortMgrTestGroup, alias3)
{
  // パラメータ型の生成
  const SmtSort* param0 = SortMgr->make_param_sort(0);

  // 型テンプレートを作る．
  // a (p0 p0)
  const SmtId* id_a = IdMgr->make_id(ShString("a"));
  (void) SortMgr->reg_sort(id_a, 2);
  vector<const SmtSort*> elem_list(2);
  elem_list[0] = param0;
  elem_list[1] = param0;
  const SmtSort* sort_tmpl = SortMgr->make_sort(id_a, elem_list);

  // これは成功するはず．
  CHECK( sort_tmpl != NULL );

  // エイリアスの登録
  const SmtId* id_b = IdMgr->make_id(ShString("b"));
  bool stat = SortMgr->reg_alias(id_b, sort_tmpl);

  // これは成功するはず
  CHECK( stat );

  // おなじ名前でもう一回登録する．
  bool stat2 = SortMgr->reg_sort(id_b, 0);

  // これは失敗するはず
  CHECK( !stat2 );

  // 同じ名前でもう一回登録する．
  bool stat3 = SortMgr->reg_alias(id_b, sort_tmpl);

  // これも失敗するはず．
  CHECK( !stat3 );

}
