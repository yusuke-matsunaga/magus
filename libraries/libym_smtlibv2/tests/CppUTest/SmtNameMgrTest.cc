
/// @file SmtNameMgrTest.cc
/// @brief SmtNameMgr のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "CppUTest/TestHarness.h"

#include "ym_smtlibv2/SmtId.h"
#include "ym_smtlibv2/SmtSort.h"
#include "ym_smtlibv2/SmtTerm.h"
#include "ym_smtlibv2/SmtVarFun.h"
#include "SmtIdMgr.h"
#include "SmtNameMgr.h"
#include "SmtSortMgr.h"
#include "SmtTermMgr.h"
#include "ym_utils/SimpleAlloc.h"


using namespace nsYm;
using namespace nsYm::nsSmtLibV2;

TEST_GROUP(SmtNameMgrTestGroup)
{
  SimpleAlloc* alloc;
  SmtIdMgr* IdMgr;
  SmtSortMgr* SortMgr;
  SmtNameMgr* NameMgr;
  SmtTermMgr* TermMgr;

  TEST_SETUP() {
    alloc = new SimpleAlloc(4096);
    IdMgr = new SmtIdMgr(*alloc);
    SortMgr = new SmtSortMgr(*alloc, 0, NULL);
    NameMgr = new SmtNameMgr(*alloc, 0, NULL);
    TermMgr = new SmtTermMgr(*alloc);
  }

  TEST_TEARDOWN() {
    delete IdMgr;
    delete SortMgr;
    delete NameMgr;
    delete TermMgr;
    delete alloc;
    ShString::free_all_memory();
  }
};

// setup(), teardown() でメモリリークが発生していないことを確認するためのテスト
TEST(SmtNameMgrTestGroup, empty)
{
}

// 関数を宣言するテスト1
TEST(SmtNameMgrTestGroup, simple_decl1)
{
  // (declare-sort a 0)
  const SmtId* id_a = IdMgr->make_id(ShString("a"));
  bool stat0 = SortMgr->reg_sort(id_a, 0);
  CHECK( stat0 );
  const SmtSort* sort_a = SortMgr->make_sort(id_a);
  CHECK( sort_a != NULL );

  // (declare-fun f () a)
  const SmtId* id_f = IdMgr->make_id(ShString("f"));
  vector<const SmtSort*> input_list0(0);
  const SmtVarFun* fun_f = NameMgr->reg_fun(id_f, input_list0, sort_a);
  CHECK( fun_f != NULL );
  LONGS_EQUAL( id_f->id(), fun_f->name()->id() );
  LONGS_EQUAL( 0, fun_f->input_num() );
  CHECK( sort_a == fun_f->output_sort() );
  CHECK( NULL == fun_f->fun_body() );
  CHECK( SmtVarFun::kNone == fun_f->fun_attr() );
  LONGS_EQUAL( 0, fun_f->param_num() );

  // fun_str() のチェック
  string s = varfun_str(fun_f);
  STRCMP_EQUAL( "f ( ) a", s.c_str() );

  // find_obj() のチェック
  const SmtVarFun* f2 = NameMgr->find_obj(id_f);
  CHECK( f2 == fun_f );
  CHECK( f2->is_fun() );

  // (declare-fun f () a)
  vector<const SmtSort*> input_list1(0);
  const SmtVarFun* fun_f2 = NameMgr->reg_fun(id_f, input_list0, sort_a);
  // 二度目はエラーとなる．
  CHECK( fun_f2 == NULL );
}

// 関数を宣言するテスト2
TEST(SmtNameMgrTestGroup, simple_decl2)
{
  // (declare-sort a 0)
  const SmtId* id_a = IdMgr->make_id(ShString("a"));
  bool stat0 = SortMgr->reg_sort(id_a, 0);
  CHECK( stat0 );
  const SmtSort* sort_a = SortMgr->make_sort(id_a);
  CHECK( sort_a != NULL );

  // (declare-sort b 0)
  const SmtId* id_b = IdMgr->make_id(ShString("b"));
  bool stat1 = SortMgr->reg_sort(id_b, 0);
  CHECK( stat1 );
  const SmtSort* sort_b = SortMgr->make_sort(id_b);
  CHECK( sort_b != NULL );

  // (declare-fun f (b b) a)
  const SmtId* id_f = IdMgr->make_id(ShString("f"));
  vector<const SmtSort*> input_list0(2);
  input_list0[0] = sort_b;
  input_list0[1] = sort_b;

  const SmtVarFun* fun_f = NameMgr->reg_fun(id_f, input_list0, sort_a);
  CHECK( fun_f != NULL );
  LONGS_EQUAL( id_f->id(), fun_f->name()->id() );
  LONGS_EQUAL( 2, fun_f->input_num() );
  CHECK( sort_b == fun_f->input_sort(0) );
  CHECK( NULL == fun_f->input_var(0) );
  CHECK( sort_b == fun_f->input_sort(1) );
  CHECK( NULL == fun_f->input_var(1) );
  CHECK( sort_a == fun_f->output_sort() );
  CHECK( NULL == fun_f->fun_body() );
  CHECK( SmtVarFun::kNone == fun_f->fun_attr() );
  LONGS_EQUAL( 0, fun_f->param_num() );

  string s = varfun_str(fun_f);
  STRCMP_EQUAL( "f ( b b ) a", s.c_str() );
}

// 関数を宣言するテスト3
TEST(SmtNameMgrTestGroup, simple_decl3)
{
  // (decl-sort a 0)
  const SmtId* id_a = IdMgr->make_id(ShString("a"));
  bool stat0 = SortMgr->reg_sort(id_a, 0);
  CHECK( stat0 );
  const SmtSort* sort_a = SortMgr->make_sort(id_a);
  CHECK( sort_a != NULL );

  // (declare-fun f1 ( a a ) a :right_assoc)
  const SmtId* id_f1 = IdMgr->make_id(ShString("f1"));
  vector<const SmtSort*> input_list1(2);
  input_list1[0] = sort_a;
  input_list1[1] = sort_a;
  const SmtVarFun* fun_f1 = NameMgr->reg_fun(id_f1, input_list1, sort_a, SmtVarFun::kRightAssoc);
  CHECK( fun_f1 != NULL );
  LONGS_EQUAL( id_f1->id(), fun_f1->name()->id() );
  LONGS_EQUAL( 2, fun_f1->input_num() );
  CHECK( sort_a == fun_f1->input_sort(0) );
  CHECK( NULL == fun_f1->input_var(0) );
  CHECK( sort_a == fun_f1->input_sort(1) );
  CHECK( NULL == fun_f1->input_var(1) );
  CHECK( sort_a == fun_f1->output_sort() );
  CHECK( NULL == fun_f1->fun_body() );
  CHECK( SmtVarFun::kRightAssoc == fun_f1->fun_attr() );
  LONGS_EQUAL( 0, fun_f1->param_num() );

  // (declare-fun f2 ( a a ) a :left_assoc)
  const SmtId* id_f2 = IdMgr->make_id(ShString("f2"));
  vector<const SmtSort*> input_list2(2);
  input_list2[0] = sort_a;
  input_list2[1] = sort_a;
  const SmtVarFun* fun_f2 = NameMgr->reg_fun(id_f2, input_list2, sort_a, SmtVarFun::kLeftAssoc);
  CHECK( fun_f2 != NULL );
  LONGS_EQUAL( id_f2->id(), fun_f2->name()->id() );
  LONGS_EQUAL( 2, fun_f2->input_num() );
  CHECK( sort_a == fun_f2->input_sort(0) );
  CHECK( NULL == fun_f2->input_var(0) );
  CHECK( sort_a == fun_f2->input_sort(1) );
  CHECK( NULL == fun_f2->input_var(1) );
  CHECK( sort_a == fun_f2->output_sort() );
  CHECK( NULL == fun_f2->fun_body() );
  CHECK( SmtVarFun::kLeftAssoc == fun_f2->fun_attr() );
  LONGS_EQUAL( 0, fun_f2->param_num() );

  // (declare-fun f3 ( a a ) a :left_assoc)
  const SmtId* id_f3 = IdMgr->make_id(ShString("f3"));
  vector<const SmtSort*> input_list3(2);
  input_list3[0] = sort_a;
  input_list3[1] = sort_a;
  const SmtVarFun* fun_f3 = NameMgr->reg_fun(id_f3, input_list3, sort_a, SmtVarFun::kChainable);
  CHECK( fun_f3 != NULL );
  LONGS_EQUAL( id_f3->id(), fun_f3->name()->id() );
  LONGS_EQUAL( 2, fun_f3->input_num() );
  CHECK( sort_a == fun_f3->input_sort(0) );
  CHECK( NULL == fun_f3->input_var(0) );
  CHECK( sort_a == fun_f3->input_sort(1) );
  CHECK( NULL == fun_f3->input_var(1) );
  CHECK( sort_a == fun_f3->output_sort() );
  CHECK( NULL == fun_f3->fun_body() );
  CHECK( SmtVarFun::kChainable == fun_f3->fun_attr() );
  LONGS_EQUAL( 0, fun_f3->param_num() );

  // (declare-fun f4 ( a a ) a :left_assoc)
  const SmtId* id_f4 = IdMgr->make_id(ShString("f4"));
  vector<const SmtSort*> input_list4(2);
  input_list4[0] = sort_a;
  input_list4[1] = sort_a;
  const SmtVarFun* fun_f4 = NameMgr->reg_fun(id_f4, input_list4, sort_a, SmtVarFun::kPairwise);
  CHECK( fun_f4 != NULL );
  LONGS_EQUAL( id_f4->id(), fun_f4->name()->id() );
  LONGS_EQUAL( 2, fun_f4->input_num() );
  CHECK( sort_a == fun_f4->input_sort(0) );
  CHECK( NULL == fun_f4->input_var(0) );
  CHECK( sort_a == fun_f4->input_sort(1) );
  CHECK( NULL == fun_f4->input_var(1) );
  CHECK( sort_a == fun_f4->output_sort() );
  CHECK( NULL == fun_f4->fun_body() );
  CHECK( SmtVarFun::kPairwise == fun_f4->fun_attr() );
  LONGS_EQUAL( 0, fun_f4->param_num() );
}

// パラメータ付きの関数の宣言のテスト1
TEST(SmtNameMgrTestGroup, param_decl1)
{
  // パラメータ0
  const SmtSort* param0 = SortMgr->make_param_sort(0);

  // (declare-sort a 0)
  const SmtId* id_a = IdMgr->make_id(ShString("a"));
  bool stat = SortMgr->reg_sort(id_a, 0);
  CHECK( stat );
  const SmtSort* sort_a = SortMgr->make_sort(id_a);
  CHECK( sort_a != NULL );

  // par (p0) (= p0 p0 a)
  const SmtId* id_eq = IdMgr->make_id(ShString("="));
  vector<const SmtSort*> eq_list(2);
  eq_list[0] = param0;
  eq_list[1] = param0;
  const SmtVarFun* fun_eq = NameMgr->reg_fun(id_eq, eq_list, sort_a);
  CHECK( fun_eq != NULL );
  LONGS_EQUAL( id_eq->id(), fun_eq->name()->id() );
  LONGS_EQUAL( 2, fun_eq->input_num() );
  CHECK( param0 == fun_eq->input_sort(0) );
  CHECK( NULL == fun_eq->input_var(0) );
  CHECK( param0 == fun_eq->input_sort(1) );
  CHECK( NULL == fun_eq->input_var(1) );
  CHECK( sort_a == fun_eq->output_sort() );
  CHECK( NULL == fun_eq->fun_body() );
  LONGS_EQUAL( 1, fun_eq->param_num() );

  string s = varfun_str(fun_eq);
  STRCMP_EQUAL( "= ( #0 #0 ) a", s.c_str() );
}

// パラメータ付きの関数の宣言のテスト2
TEST(SmtNameMgrTestGroup, param_decl2)
{
  // パラメータ1
  const SmtSort* param1 = SortMgr->make_param_sort(1);

  // (declare-sort a 0)
  const SmtId* id_a = IdMgr->make_id(ShString("a"));
  bool stat = SortMgr->reg_sort(id_a, 0);
  CHECK( stat );
  const SmtSort* sort_a = SortMgr->make_sort(id_a);
  CHECK( sort_a != NULL );

  // par (p1) (= p1 p1 a)
  const SmtId* id_eq = IdMgr->make_id(ShString("="));
  vector<const SmtSort*> eq_list(2);
  eq_list[0] = param1;
  eq_list[1] = param1;
  const SmtVarFun* fun_eq = NameMgr->reg_fun(id_eq, eq_list, sort_a);
  // パラメータ0番が抜けているのでエラー
  CHECK( fun_eq == NULL );
}

// 関数定義のテスト
TEST(SmtNameMgrTestGroup, fun_def)
{
  // (declare-sort a 0)
  const SmtId* id_a = IdMgr->make_id(ShString("a"));
  bool stat0 = SortMgr->reg_sort(id_a, 0);
  CHECK( stat0 );
  const SmtSort* sort_a = SortMgr->make_sort(id_a);
  CHECK( sort_a != NULL );

  const SmtTerm* body = TermMgr->make_numeric(0);
  CHECK( body != NULL );

  // (define-fun (f () a 0 ) )
  const SmtId* id_f = IdMgr->make_id(ShString("f"));
  CHECK( id_f != NULL );

  const SmtVarFun* fun = NameMgr->reg_fun(id_f, vector<const SmtVarFun*>(0), sort_a, body);
  CHECK( fun != NULL );

  LONGS_EQUAL( id_f->id(), fun->name()->id() );
  LONGS_EQUAL( 0, fun->input_num() );
  CHECK( sort_a == fun->output_sort() );

  const SmtTerm* fbody = fun->fun_body();
  CHECK( fbody != NULL );
  string sbody = term_str(fbody);
  STRCMP_EQUAL( "0", sbody.c_str() );
}
