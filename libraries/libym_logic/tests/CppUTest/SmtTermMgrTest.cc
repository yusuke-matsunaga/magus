
/// @file SmtTermMgrTest.cc
/// @brief SmtTermMgr のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "CppUTest/TestHarness.h"

#include "ym_smtlibv2/SmtAttr.h"
#include "ym_smtlibv2/SmtId.h"
#include "ym_smtlibv2/SmtSort.h"
#include "ym_smtlibv2/SmtTerm.h"
#include "ym_smtlibv2/SmtVarFun.h"
#include "SmtIdMgr.h"
#include "SmtSortMgr.h"
#include "SmtNameMgr.h"
#include "SmtTermMgr.h"
#include "ym_utils/SimpleAlloc.h"


using namespace nsYm;
using namespace nsYm::nsSmtLibV2;

TEST_GROUP(SmtTermMgrTestGroup)
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
TEST(SmtTermMgrTestGroup, empty)
{
}

// make_numeric テスト
TEST(SmtTermMgrTestGroup, make_numeric)
{
  const ymuint32 NUM_VAL = 123;
  const SmtTerm* term = TermMgr->make_numeric(NUM_VAL);
  CHECK( term != NULL );

  LONGS_EQUAL( SmtTerm::kNumConst, term->type() );
  LONGS_EQUAL( NUM_VAL, term->int_value() );
}

// make_decimal テスト
TEST(SmtTermMgrTestGroup, make_decimal)
{
  const char* STR_VAL = "123.456";
  const SmtTerm* term = TermMgr->make_decimal(ShString(STR_VAL));
  CHECK( term != NULL );

  LONGS_EQUAL( SmtTerm::kDecConst, term->type() );
  STRCMP_EQUAL( STR_VAL, term->str_value() );
}

// make_hexadecimal テスト
TEST(SmtTermMgrTestGroup, make_hexadecimal)
{
  const char* STR_VAL = "#x0Ae9";
  const SmtTerm* term = TermMgr->make_hexadecimal(ShString(STR_VAL));
  CHECK( term != NULL );

  LONGS_EQUAL( SmtTerm::kHexConst, term->type() );
  STRCMP_EQUAL( STR_VAL, term->str_value() );
}

// make_binary テスト
TEST(SmtTermMgrTestGroup, make_binary)
{
  const char* STR_VAL = "#b0110";
  const SmtTerm* term = TermMgr->make_binary(ShString(STR_VAL));
  CHECK( term != NULL );

  LONGS_EQUAL( SmtTerm::kBinConst, term->type() );
  STRCMP_EQUAL( STR_VAL, term->str_value() );
}

// make_string テスト
TEST(SmtTermMgrTestGroup, make_string)
{
  const char* STR_VAL = "string ";
  const SmtTerm* term = TermMgr->make_string(ShString(STR_VAL));
  CHECK( term != NULL );

  LONGS_EQUAL( SmtTerm::kStrConst, term->type() );
  STRCMP_EQUAL( STR_VAL, term->str_value() );
}

// make_symbol テスト
TEST(SmtTermMgrTestGroup, make_symbol)
{
  const char* STR_VAL = "abcd";
  const SmtTerm* term = TermMgr->make_symbol(ShString(STR_VAL));
  CHECK( term != NULL );

  LONGS_EQUAL( SmtTerm::kSymbol, term->type() );
  STRCMP_EQUAL( STR_VAL, term->str_value() );
}

// make_keyword テスト
TEST(SmtTermMgrTestGroup, make_keyword)
{
  const char* STR_VAL = ":name";
  const SmtTerm* term = TermMgr->make_keyword(ShString(STR_VAL));
  CHECK( term != NULL );

  LONGS_EQUAL( SmtTerm::kKeyword, term->type() );
  STRCMP_EQUAL( STR_VAL, term->str_value() );
}

#if 0
// make_identifier テスト
TEST(SmtTermMgrTestGroup, make_identifier)
{
  const SmtId* id_a = IdMgr->make_id(ShString("a"));
  CHECK( id_a != NULL );

  const SmtTerm* term = TermMgr->make_identifier(id_a);
  CHECK( term != NULL);

  LONGS_EQUAL( SmtTerm::kIdentifier, term->type() );
  LONGS_EQUAL( id_a->id(), term->identifier()->id() );
}

// make_qual_identifier テスト
TEST(SmtTermMgrTestGroup, make_qual_identifier)
{
  const SmtId* id_a = IdMgr->make_id(ShString("a"));
  CHECK( id_a != NULL );

  const SmtId* id_b = IdMgr->make_id(ShString("B"));
  CHECK( id_b != NULL );
  bool stat = SortMgr->reg_sort(id_b, 0);
  CHECK( stat );
  const SmtSort* sort_b = SortMgr->make_sort(id_b);
  CHECK( sort_b != NULL );

  const SmtTerm* term = TermMgr->make_qual_identifier(id_a, sort_b);
  CHECK( term != NULL );

  LONGS_EQUAL( SmtTerm::kQualIdentifier, term->type() );
  LONGS_EQUAL( id_a->id(), term->identifier()->id() );
  CHECK( sort_b == term->identifier_sort() );
}
#endif

// make_fun テスト1
TEST(SmtTermMgrTestGroup, make_fun1)
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

  // 引数なしの関数呼び出しは失敗する．
  const SmtTerm* term = TermMgr->make_fun(fun_f, vector<const SmtTerm*>(0));
  CHECK( term == NULL );
}

// make_fun テスト2
TEST(SmtTermMgrTestGroup, make_fun2)
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

  const SmtTerm* i0 = TermMgr->make_numeric(0);
  CHECK( i0 != NULL );
  const SmtTerm* i1 = TermMgr->make_numeric(1);
  CHECK( i1 != NULL );
  vector<const SmtTerm*> input_list(2);
  input_list[0] = i0;
  input_list[1] = i1;
  const SmtTerm* term = TermMgr->make_fun(fun_f, input_list);
  CHECK( term != NULL );

  LONGS_EQUAL( SmtTerm::kFunTerm, term->type() );
  CHECK( fun_f == term->function() );
  LONGS_EQUAL( 2, term->input_num() );
  for (ymuint i = 0; i < 2; ++ i) {
    CHECK( input_list[i] == term->input(i) );
  }
}

#if 0
// make_let テスト1
TEST(SmtTermMgrTestGroup, make_let1)
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

  const SmtTerm* i0 = TermMgr->make_numeric(0);
  CHECK( i0 != NULL );
  const SmtTerm* i1 = TermMgr->make_numeric(1);
  CHECK( i1 != NULL );
  vector<const SmtTerm*> input_list(2);
  input_list[0] = i0;
  input_list[1] = i1;
  const SmtTerm* fun_term = TermMgr->make_fun(fun_f, input_list);
  CHECK( fun_term != NULL );

  // var_binding が空ならエラー
  const SmtTerm* term = TermMgr->make_let(vector<SmtVarBinding>(0), fun_term);
  CHECK( term == NULL );
}

// make_let テスト2
TEST(SmtTermMgrTestGroup, make_let2)
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

  const SmtId* id_x = IdMgr->make_id(ShString("x"));
  CHECK( id_x != NULL );
  const SmtTerm* x_term = TermMgr->make_identifier(id_x);
  CHECK( x_term != NULL );
  const SmtId* id_y = IdMgr->make_id(ShString("y"));
  CHECK( id_y != NULL );
  const SmtTerm* y_term = TermMgr->make_identifier(id_y);
  CHECK( y_term != NULL );

  vector<const SmtTerm*> input_list(2);
  input_list[0] = x_term;
  input_list[1] = y_term;
  const SmtTerm* fun_term = TermMgr->make_fun(fun_f, input_list);
  CHECK( fun_term != NULL );

  const SmtTerm* i0 = TermMgr->make_numeric(0);
  CHECK( i0 != NULL );
  const SmtTerm* i1 = TermMgr->make_numeric(1);
  CHECK( i1 != NULL );

  vector<SmtVarBinding> var_binding;
  var_binding.reserve(2);
  var_binding.push_back(SmtVarBinding(id_x, i0));
  var_binding.push_back(SmtVarBinding(id_y, i1));
  const SmtTerm* term = TermMgr->make_let(var_binding, fun_term);
  CHECK( term != NULL );

  LONGS_EQUAL( SmtTerm::kLet, term->type() );
  CHECK( fun_term == term->body() );
  LONGS_EQUAL( var_binding.size(), term->let_binding_num() );
  SmtVarBinding vb0 = term->let_binding(0);
  CHECK( vb0.mVar == id_x );
  CHECK( vb0.mExpr == i0 );
  SmtVarBinding vb1 = term->let_binding(1);
  CHECK( vb1.mVar == id_y );
  CHECK( vb1.mExpr == i1 );
}
#endif

// make_forall テスト1
TEST(SmtTermMgrTestGroup, make_forall1)
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

  const SmtTerm* i0 = TermMgr->make_numeric(0);
  CHECK( i0 != NULL );
  const SmtTerm* i1 = TermMgr->make_numeric(1);
  CHECK( i1 != NULL );
  vector<const SmtTerm*> input_list(2);
  input_list[0] = i0;
  input_list[1] = i1;
  const SmtTerm* fun_term = TermMgr->make_fun(fun_f, input_list);
  CHECK( fun_term != NULL );

  // var_list が空ならエラー
  const SmtTerm* term = TermMgr->make_forall(vector<const SmtVarFun*>(0), fun_term);
  CHECK( term == NULL );
}

// make_forall テスト2
TEST(SmtTermMgrTestGroup, make_forall2)
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

  const SmtId* id_x = IdMgr->make_id(ShString("x"));
  CHECK( id_x != NULL );
  const SmtVarFun* var_x = NameMgr->reg_var(id_x);
  CHECK( var_x != NULL);
  const SmtTerm* x_term = TermMgr->make_var(var_x);
  CHECK( x_term != NULL );
  const SmtId* id_y = IdMgr->make_id(ShString("y"));
  CHECK( id_y != NULL );
  const SmtVarFun* var_y = NameMgr->reg_var(id_y);
  CHECK( var_y != NULL );
  const SmtTerm* y_term = TermMgr->make_var(var_y);
  CHECK( y_term != NULL );

  vector<const SmtTerm*> input_list(2);
  input_list[0] = x_term;
  input_list[1] = y_term;
  const SmtTerm* fun_term = TermMgr->make_fun(fun_f, input_list);
  CHECK( fun_term != NULL );

  const SmtTerm* i0 = TermMgr->make_numeric(0);
  CHECK( i0 != NULL );
  const SmtTerm* i1 = TermMgr->make_numeric(1);
  CHECK( i1 != NULL );

  vector<const SmtVarFun*> var_list;
  var_list.reserve(1);
  const SmtVarFun* var = NameMgr->reg_var(id_x, sort_b);
  var_list.push_back(var);
  const SmtTerm* term = TermMgr->make_forall(var_list, fun_term);
  CHECK( term != NULL );

  LONGS_EQUAL( SmtTerm::kForall, term->type() );
  CHECK( fun_term == term->body() );
  LONGS_EQUAL( var_list.size(), term->var_num() );
  const SmtVarFun* sv = term->bound_var(0);
  CHECK( sv->var_sort() == sort_b );
  LONGS_EQUAL( id_x->id(), sv->name()->id() );
}

// make_exists テスト1
TEST(SmtTermMgrTestGroup, make_exists1)
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

  const SmtTerm* i0 = TermMgr->make_numeric(0);
  CHECK( i0 != NULL );
  const SmtTerm* i1 = TermMgr->make_numeric(1);
  CHECK( i1 != NULL );
  vector<const SmtTerm*> input_list(2);
  input_list[0] = i0;
  input_list[1] = i1;
  const SmtTerm* fun_term = TermMgr->make_fun(fun_f, input_list);
  CHECK( fun_term != NULL );

  // var_list が空ならエラー
  const SmtTerm* term = TermMgr->make_exists(vector<const SmtVarFun*>(0), fun_term);
  CHECK( term == NULL );
}

// make_exists テスト2
TEST(SmtTermMgrTestGroup, make_exists2)
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

  const SmtId* id_x = IdMgr->make_id(ShString("x"));
  CHECK( id_x != NULL );
  const SmtVarFun* var_x = NameMgr->reg_var(id_x);
  CHECK( var_x != NULL );
  const SmtTerm* x_term = TermMgr->make_var(var_x);
  CHECK( x_term != NULL );
  const SmtId* id_y = IdMgr->make_id(ShString("y"));
  CHECK( id_y != NULL );
  const SmtVarFun* var_y = NameMgr->reg_var(id_y);
  CHECK( var_y != NULL );
  const SmtTerm* y_term = TermMgr->make_var(var_y);
  CHECK( y_term != NULL );

  vector<const SmtTerm*> input_list(2);
  input_list[0] = x_term;
  input_list[1] = y_term;
  const SmtTerm* fun_term = TermMgr->make_fun(fun_f, input_list);
  CHECK( fun_term != NULL );

  const SmtTerm* i0 = TermMgr->make_numeric(0);
  CHECK( i0 != NULL );
  const SmtTerm* i1 = TermMgr->make_numeric(1);
  CHECK( i1 != NULL );

  vector<const SmtVarFun*> var_list;
  var_list.reserve(1);
  const SmtVarFun* var = NameMgr->reg_var(id_x, sort_b);
  var_list.push_back(var);
  const SmtTerm* term = TermMgr->make_exists(var_list, fun_term);
  CHECK( term != NULL );

  LONGS_EQUAL( SmtTerm::kExists, term->type() );
  CHECK( fun_term == term->body() );
  LONGS_EQUAL( var_list.size(), term->var_num() );
  const SmtVarFun* sv = term->bound_var(0);
  CHECK( sv != NULL );
  CHECK( sv->var_sort() == sort_b );
  LONGS_EQUAL( id_x->id(), sv->name()->id() );
}

// make_attr テスト1
TEST(SmtTermMgrTestGroup, make_attr1)
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

  const SmtTerm* i0 = TermMgr->make_numeric(0);
  CHECK( i0 != NULL );
  const SmtTerm* i1 = TermMgr->make_numeric(1);
  CHECK( i1 != NULL );
  vector<const SmtTerm*> input_list(2);
  input_list[0] = i0;
  input_list[1] = i1;
  const SmtTerm* fun_term = TermMgr->make_fun(fun_f, input_list);
  CHECK( fun_term != NULL );

  // attr_list が空ならエラー
  const SmtTerm* term = TermMgr->make_attr(fun_term, vector<SmtAttr>(0));
  CHECK( term == NULL );
}

// make_attr テスト2
TEST(SmtTermMgrTestGroup, make_attr2)
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

  const SmtTerm* i0 = TermMgr->make_numeric(0);
  CHECK( i0 != NULL );
  const SmtTerm* i1 = TermMgr->make_numeric(1);
  CHECK( i1 != NULL );
  vector<const SmtTerm*> input_list(2);
  input_list[0] = i0;
  input_list[1] = i1;
  const SmtTerm* fun_term = TermMgr->make_fun(fun_f, input_list);
  CHECK( fun_term != NULL );

  const char* KEYWORD = ":name";
  const char* ATTR_VAL = "ABC";
  vector<SmtAttr> attr_list;
  const SmtTerm* vattr = TermMgr->make_string(ShString(ATTR_VAL));
  attr_list.push_back(SmtAttr(ShString(KEYWORD), vattr));
  const SmtTerm* term = TermMgr->make_attr(fun_term, attr_list);
  CHECK( term != NULL );

  LONGS_EQUAL( SmtTerm::kAttr, term->type() );
  CHECK( fun_term == term->body() );
  LONGS_EQUAL( attr_list.size(), term->attr_num() );
  STRCMP_EQUAL( KEYWORD, term->attr_keyword(0) );
  CHECK( vattr == term->attr_value(0) );
}

// make_list テスト
TEST(SmtTermMgrTestGroup, make_list)
{
  vector<const SmtTerm*> term_list;
  const SmtTerm* t0 = TermMgr->make_numeric(123);
  CHECK( t0 != NULL );
  const SmtTerm* t1 = TermMgr->make_string(ShString("ABC"));
  CHECK( t1 != NULL );
  term_list.push_back(t0);
  term_list.push_back(t1);

  const SmtTerm* term = TermMgr->make_list(term_list);
  CHECK( term != NULL );

  LONGS_EQUAL( 2, term->elem_num() );
  CHECK( t0 == term->elem(0) );
  CHECK( t1 == term->elem(1) );
}
