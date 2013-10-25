
/// @file SortMgrTest.cc
/// @brief SortMgr のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "CppUTest/TestHarness.h"

#include "ym_logic/SmtSolver.h"
#include "ym_logic/SmtSort.h"

#include "ym_smtlibv2/SmtId.h"
#include "IdMgr.h"
#include "SortMgr.h"
#include "SortElem.h"
#include "ym_utils/SimpleAlloc.h"


using namespace nsYm;
using namespace nsYm::nsSmtLibV2;

TEST_GROUP(SortMgrTestGroup)
{
  SimpleAlloc* alloc;
  SmtSolver* mSolver;
  IdMgr* mIdMgr;
  SortMgr* mSortMgr;

  TEST_SETUP() {
    alloc = new SimpleAlloc(4096);
    mSolver = SmtSolver::new_solver();
    mIdMgr = new IdMgr(*alloc);
    mSortMgr = new SortMgr(*alloc, *mSolver, 0, NULL);
  }

  TEST_TEARDOWN() {
    delete mIdMgr;
    delete mSolver;
    delete mSortMgr;
    delete alloc;
    ShString::free_all_memory();
  }
};

// setup(), teardown() でメモリリークが発生していないことを確認するためのテスト
TEST(SortMgrTestGroup, empty)
{
}

// 型を登録するテスト
TEST(SortMgrTestGroup, simple_reg)
{
  const SmtId* id = mIdMgr->make_id(ShString("a"));
  bool stat = mSortMgr->declare_sort(id, 2);
  // これは成功するはず
  CHECK( stat );

  // 同じ名前でもう一度登録
  bool stat2 = mSortMgr->declare_sort(id, 2);
  // これは失敗するはず
  CHECK( !stat2 );
}

// 単純な型を作るテスト
TEST(SortMgrTestGroup, simple_sort)
{
  // a という単純な型を作る．
  const SmtId* id = mIdMgr->make_id(ShString("a"));
  CHECK( id != NULL );

  bool stat1 = mSortMgr->declare_sort(id, 0);
  CHECK( stat1 );

  const SmtSort* sort = mSortMgr->make_sort(id);

  // sort は NULL ではないはず
  CHECK( sort != NULL );

  // sort の要素数は0
  LONGS_EQUAL( 0, sort->elem_num() );

  const SmtId* id2 = mIdMgr->make_id(ShString("b"));
  CHECK( id2 != NULL );
  bool stat2 = mSortMgr->declare_sort(id2, 0);
  CHECK( stat2 );

  const SmtSort* sort1 = mSortMgr->make_sort(id2);
  CHECK( sort1 != NULL );

  vector<const SmtSort*> elem_list(1);
  elem_list[0] = sort1;
  // (a (b)) という型を作る．
  const SmtSort* sort2 = mSortMgr->make_sort(id, elem_list);
  // もちろん失敗するはず．
  CHECK( sort2 == NULL );
}

// 複雑な型を作るテスト
TEST(SortMgrTestGroup, complex_sort)
{
  // b という単純な型を作る．
  const SmtId* id_b = mIdMgr->make_id(ShString("b"));
  CHECK( id_b != NULL );

  bool stat1 = mSortMgr->declare_sort(id_b, 0);
  CHECK( stat1 );

  const SmtSort* sort_b = mSortMgr->make_sort(id_b);
  CHECK( sort_b != NULL );

  const SmtId* id_a = mIdMgr->make_id(ShString("a"));
  CHECK( id_a != NULL );

  bool stat2 = mSortMgr->declare_sort(id_a, 1);
  CHECK( stat2 );

  vector<const SmtSort*> elem_list(1);
  elem_list[0] = sort_b;
  // a (b) という型を作る．
  const SmtSort* sort_a = mSortMgr->make_sort(id_a, elem_list);

  // これは成功するはず
  CHECK( sort_a != NULL );

  // 要素数は 1
  LONGS_EQUAL( 1, sort_a->elem_num() );

  // 0 番目の要素型は sort_b のはず．
  CHECK( sort_b = sort_a->elem(0) );
}

// 単純な型の同一性テスト
TEST(SortMgrTestGroup, simple_identity)
{
  // a という単純な型を作る．
  const SmtId* id_a = mIdMgr->make_id(ShString("a"));
  CHECK( id_a != NULL );

  bool stat1 = mSortMgr->declare_sort(id_a, 0);
  CHECK( stat1 );

  const SmtSort* sort_a = mSortMgr->make_sort(id_a);
  CHECK( sort_a != NULL );

  // もう一回 a という単純な型を作る．
  const SmtSort* sort_a1 = mSortMgr->make_sort(id_a);
  CHECK( sort_a1 != NULL );

  // この2つは同一のはず．
  CHECK( sort_a == sort_a1 );
}

// 複雑な型の同一性テスト
TEST(SortMgrTestGroup, complex_identity)
{
  // b という単純な型を作る．
  const SmtId* id_b = mIdMgr->make_id(ShString("b"));
  CHECK( id_b != NULL );

  bool stat1 = mSortMgr->declare_sort(id_b, 0);
  CHECK( stat1 );

  const SmtSort* sort_b = mSortMgr->make_sort(id_b);
  CHECK( sort_b != NULL );

  // a (b) という型を作る．
  const SmtId* id_a = mIdMgr->make_id(ShString("a"));
  CHECK( id_a != NULL );

  bool stat2 = mSortMgr->declare_sort(id_a, 1);
  CHECK( stat2 );

  vector<const SmtSort*> elem_list(1);
  elem_list[0] = sort_b;
  const SmtSort* sort_a = mSortMgr->make_sort(id_a, elem_list);
  CHECK( sort_a != NULL );

  // もう一回 a(b) という型を作る．
  vector<const SmtSort*> elem_list1(1);
  elem_list1[0] = sort_b;
  const SmtSort* sort_a1 = mSortMgr->make_sort(id_a, elem_list1);
  CHECK( sort_a1 != NULL );

  // この2つは同一のはず．
  CHECK( sort_a == sort_a1 );
}

// 型パラメータの生成テスト
TEST(SortMgrTestGroup, param)
{
  const SortElem* sort = mSortMgr->make_param_sort_templ(0);
  CHECK( sort != NULL );

  // パラメータ型のはず．
  CHECK( sort->is_param() );

  // 要素数は 0
  LONGS_EQUAL( 0, sort->elem_num() );

  // パラメータ番号は 0
  LONGS_EQUAL( 0, sort->param_id() );

  const SortElem* sort1 = mSortMgr->make_param_sort_templ(1);
  CHECK( sort1 != NULL );

  // パラメータ型のはず．
  CHECK( sort1->is_param() );

  // 要素数は 0
  LONGS_EQUAL( 0, sort1->elem_num() );

  // パラメータ番号は 1
  LONGS_EQUAL( 1, sort1->param_id() );
}

// 型エイリアスの登録テスト
TEST(SortMgrTestGroup, alias1)
{
  // パラメータ型の生成
  const SortElem* param0 = mSortMgr->make_param_sort_templ(0);
  CHECK( param0 != NULL );

  // 型テンプレートを作る．
  // a (p0, p0)
  const SmtId* id_a = mIdMgr->make_id(ShString("a"));
  CHECK( id_a != NULL );

  bool stat1 = mSortMgr->declare_sort(id_a, 2);
  CHECK( stat1 );

  vector<const SortElem*> elem_list(2);
  elem_list[0] = param0;
  elem_list[1] = param0;
  const SortElem* sort_tmpl = mSortMgr->make_complex_sort_templ(id_a, elem_list);

  // これは成功するはず
  CHECK( sort_tmpl != NULL );

  // エイリアスを登録する．
  // (define-sort b (p0) (a p0 p0))
  const SmtId* id_b = mIdMgr->make_id(ShString("b"));
  CHECK( id_b != NULL );

  bool stat = mSortMgr->define_sort(id_b, sort_tmpl);

  // これは成功するはず．
  CHECK( stat );

  // sort_tmpl を実体化する．
  // ( b  x ) = (a x x )
  const SmtId* id_x = mIdMgr->make_id(ShString("x"));
  CHECK( id_x != NULL );

  bool stat2 = mSortMgr->declare_sort(id_x, 0);
  CHECK( stat2 );

  const SmtSort* sort_x = mSortMgr->make_sort(id_x);
  CHECK( sort_x != NULL );

  vector<const SmtSort*> elem_list1(1);
  elem_list1[0] = sort_x;
  const SmtSort* sort_bx = mSortMgr->make_sort(id_b, elem_list1);

  // これは成功するはず．
  CHECK( sort_bx != NULL );

  // sort_bx の要素数は2
  LONGS_EQUAL( 2, sort_bx->elem_num() );

  // sort_bx の0番めの要素は sort_x
  CHECK( sort_bx->elem(0) == sort_x );

  // sort_bx の1番目の要素は sort_x
  CHECK( sort_bx->elem(1) == sort_x );

}

// エイリアスの登録テスト2
TEST(SortMgrTestGroup, alias2)
{
  // パラメータ型の生成
  const SortElem* param1 = mSortMgr->make_param_sort_templ(1);
  CHECK( param1 != NULL );

  // 型テンプレートを作る．
  // a (p1)
  const SmtId* id_a = mIdMgr->make_id(ShString("a"));
  CHECK( id_a != NULL );

  bool stat1 = mSortMgr->declare_sort(id_a, 1);
  CHECK( stat1 );

  vector<const SortElem*> elem_list(1);
  elem_list[0] = param1;
  const SortElem* sort_tmpl = mSortMgr->make_complex_sort_templ(id_a, elem_list);

  // これは歯抜けなので失敗するはず．
  CHECK( sort_tmpl != NULL );
}

// エイリアスの登録テスト3
TEST(SortMgrTestGroup, alias3)
{
  // パラメータ型の生成
  const SortElem* param0 = mSortMgr->make_param_sort_templ(0);
  CHECK( param0 != NULL );

  // 型テンプレートを作る．
  // a (p0 p0)
  const SmtId* id_a = mIdMgr->make_id(ShString("a"));
  CHECK( id_a != NULL );

  bool stat1 = mSortMgr->declare_sort(id_a, 2);
  CHECK( stat1 );

  vector<const SortElem*> elem_list(2);
  elem_list[0] = param0;
  elem_list[1] = param0;
  const SortElem* sort_tmpl = mSortMgr->make_complex_sort_templ(id_a, elem_list);

  // これは成功するはず．
  CHECK( sort_tmpl != NULL );

  // エイリアスの登録
  const SmtId* id_b = mIdMgr->make_id(ShString("b"));
  CHECK( id_b != NULL );

  bool stat = mSortMgr->define_sort(id_b, sort_tmpl);

  // これは成功するはず
  CHECK( stat );

  // おなじ名前でもう一回登録する．
  bool stat2 = mSortMgr->declare_sort(id_b, 0);

  // これは失敗するはず
  CHECK( !stat2 );

  // 同じ名前でもう一回登録する．
  bool stat3 = mSortMgr->define_sort(id_b, sort_tmpl);

  // これも失敗するはず．
  CHECK( !stat3 );

}
