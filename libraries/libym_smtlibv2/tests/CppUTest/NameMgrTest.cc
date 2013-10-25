
/// @file NameMgrTest.cc
/// @brief NameMgr のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "CppUTest/TestHarness.h"

#include "ym_logic/SmtSolver.h"
#include "ym_logic/SmtSort.h"
#include "ym_logic/SmtTerm.h"
#include "ym_logic/SmtVar.h"
#include "ym_logic/SmtFun.h"

#include "ym_smtlibv2/SmtId.h"
#include "IdMgr.h"
#include "NameMgr.h"
#include "NameObj.h"
#include "SortMgr.h"
#include "SortElem.h"

#include "ym_utils/SimpleAlloc.h"


using namespace nsYm;
using namespace nsYm::nsSmtLibV2;

TEST_GROUP(NameMgrTestGroup)
{
  SimpleAlloc* alloc;
  SmtSolver* mSolver;
  IdMgr* mIdMgr;
  SortMgr* mSortMgr;
  NameMgr* mNameMgr;

  TEST_SETUP() {
    alloc = new SimpleAlloc(4096);
    mSolver = SmtSolver::new_solver();
    mIdMgr = new IdMgr(*alloc);
    mSortMgr = new SortMgr(*alloc, *mSolver, 0, NULL);
    mNameMgr = new NameMgr(*alloc, 0, NULL);
  }

  TEST_TEARDOWN() {
    delete mIdMgr;
    delete mSolver;
    delete mSortMgr;
    delete mNameMgr;
    delete alloc;
    ShString::free_all_memory();
  }
};

// setup(), teardown() でメモリリークが発生していないことを確認するためのテスト
TEST(NameMgrTestGroup, empty)
{
}

// 関数を登録するテスト
TEST(NameMgrTestGroup, reg_fun)
{
  // (declare-sort a 0)
  const SmtId* id_a = mIdMgr->make_id(ShString("a"));
  CHECK( id_a != NULL );

  bool stat0 = mSortMgr->declare_sort(id_a, 0);
  CHECK( stat0 );

  const SmtSort* sort_a = mSortMgr->make_sort(id_a);
  CHECK( sort_a != NULL );

  // (declare-fun f () a)
  const SmtId* id_f = mIdMgr->make_id(ShString("f"));
  CHECK( id_f != NULL );

  vector<const SmtSort*> input_list0(0);
  const SmtFun* fun_f = mSolver->make_fun(input_list0, sort_a);
  CHECK( fun_f != NULL );

  bool stat1 = mNameMgr->reg_fun(id_f, fun_f);
  CHECK( stat1 );

  // find_obj() のチェック
  const NameObj* obj = mNameMgr->find_obj(id_f);
  CHECK( obj != NULL );
  CHECK( obj->is_fun() );
  CHECK( obj->fun() == fun_f );

  // (declare-fun f () a)
  bool stat2 = mNameMgr->reg_fun(id_f, fun_f);
  // 二度目はエラーとなる．
  CHECK( !stat2 );

  const SmtVar* var = mSolver->make_var(sort_a);
  CHECK( var != NULL );

  bool stat3 = mNameMgr->reg_var(id_f, var);
  // 同名の変数もエラー
  CHECK( !stat3 );
}

// 変数を登録するテスト
TEST(NameMgrTestGroup, reg_var)
{
  // (declare-sort a 0)
  const SmtId* id_a = mIdMgr->make_id(ShString("a"));
  CHECK( id_a != NULL );

  bool stat0 = mSortMgr->declare_sort(id_a, 0);
  CHECK( stat0 );

  const SmtSort* sort_a = mSortMgr->make_sort(id_a);
  CHECK( sort_a != NULL );

  const SmtVar* var = mSolver->make_var(sort_a);
  CHECK( var != NULL );

  const SmtId* id_x = mIdMgr->make_id(ShString("x"));
  bool stat1 = mNameMgr->reg_var(id_x, var);
  CHECK( stat1 );

  // find_obj のチェック
  const NameObj* obj = mNameMgr->find_obj(id_x);
  CHECK( obj != NULL );
  CHECK( obj->is_var() );
  CHECK( obj->var() == var );

  bool stat2 = mNameMgr->reg_var(id_x, var);
  // 二度目はエラーとなる．
  CHECK( !stat2 );

  vector<const SmtSort*> input_list0(0);
  const SmtFun* fun_f = mSolver->make_fun(input_list0, sort_a);
  CHECK( fun_f != NULL );

  // 同名の関数の登録もエラー
  bool stat3 = mNameMgr->reg_fun(id_x, fun_f);
  CHECK( !stat3 );
}
