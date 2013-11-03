
/// @file SmtSolverTest.cc
/// @brief SmtSolver のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "CppUTest/TestHarness.h"

#include "ym_logic/SmtSolver.h"
#include "ym_logic/SmtSort.h"
#include "ym_logic/SmtVar.h"
#include "ym_logic/SmtFun.h"
#include "ym_logic/SmtTerm.h"


using namespace nsYm;
using namespace nsYm::nsSmt;


TEST_GROUP(SmtSolverTestGroup)
{
  SmtSolver* mSolver;

  TEST_SETUP() {
    mSolver = SmtSolver::new_solver();
  }

  TEST_TEARDOWN() {
    delete mSolver;
    ShString::free_all_memory();
  }

  // tSmtSortId が適正かテストする関数
  void sortid_test(tSmtSortId sort)
  {
    CHECK( sort != kSmtSort_None );
    CHECK( sort != kSmtSort_Bool );
    CHECK( sort != kSmtSort_Int );
    CHECK( sort != kSmtSort_Real );
  }

  // SmtVar のテスト関数
  void var_test(const SmtVar* var,
		tSmtSortId sort,
		tSmtVar var_type)
  {
    CHECK( var != NULL );
    LONGS_EQUAL( sort, var->sort() );
    LONGS_EQUAL( var_type, var->type() );
    switch ( var_type ) {
    case kSmtVar_Global:
      CHECK( var->is_global() );
      CHECK( !var->is_fun_arg() );
      CHECK( !var->is_forall() );
      CHECK( !var->is_exists() );
      break;

    case kSmtVar_FunArg:
      CHECK( !var->is_global() );
      CHECK( var->is_fun_arg() );
      CHECK( !var->is_forall() );
      CHECK( !var->is_exists() );
      break;

    case kSmtVar_Forall:
      CHECK( !var->is_global() );
      CHECK( !var->is_fun_arg() );
      CHECK( var->is_forall() );
      CHECK( !var->is_exists() );
      break;

    case kSmtVar_Exists:
      CHECK( !var->is_global() );
      CHECK( !var->is_fun_arg() );
      CHECK( !var->is_forall() );
      CHECK( var->is_exists() );
      break;
    }
  }

  // SmtFun のテスト関数
  void fun_test(const SmtFun* fun,
		const vector<tSmtSortId>& input_sort_list,
		tSmtSortId output_sort,
		const SmtTerm* body)
  {
    CHECK( fun != NULL );
    ymuint n = input_sort_list.size();
    LONGS_EQUAL( n, fun->input_num() );
    for (ymuint i = 0; i < n; ++ i) {
      CHECK( input_sort_list[i] == fun->input_sort(i) );
    }
    CHECK( output_sort == fun->output_sort() );
    CHECK( body == fun->body() );
  }

  // 引数なしの builtin function term のテスト
  void builtin_fun_test0(tSmtFun fun_type,
			 bool exp)
  {
    const SmtTerm* term = mSolver->make_fun_term(fun_type);
    if ( exp ) {
      CHECK( term != NULL );
      LONGS_EQUAL( SmtTerm::kFunTerm, term->type() );
      LONGS_EQUAL( fun_type, term->function_type() );
      CHECK( term->function() == NULL );
    }
    else {
      CHECK( term == NULL );
    }
  }

  // 1引数の builtin function term のテスト
  void builtin_fun_test1(tSmtFun fun_type,
			 tSmtSortId input_sort1,
			 bool exp)
  {
    const SmtVar* var1 = mSolver->make_var(input_sort1);
    CHECK( var1 != NULL );
    const SmtTerm* term1 = mSolver->make_var_term(var1);
    CHECK( term1 != NULL );

    vector<const SmtTerm*> arg_list(1);
    arg_list[0] = term1;
    const SmtTerm* term = mSolver->make_fun_term(fun_type, arg_list);
    if ( exp ) {
      CHECK( term != NULL );
      LONGS_EQUAL( SmtTerm::kFunTerm, term->type() );
      LONGS_EQUAL( fun_type, term->function_type() );
      CHECK( term->function() == NULL );
      LONGS_EQUAL( 1, term->input_num() );
      CHECK( term1 == term->input(0) );
    }
    else {
      CHECK( term == NULL );
    }
  }

  // 2引数の builtin function term のテスト
  void builtin_fun_test2(tSmtFun fun_type,
			 tSmtSortId input_sort1,
			 tSmtSortId input_sort2,
			 bool exp)
  {
    const SmtVar* var1 = mSolver->make_var(input_sort1);
    CHECK( var1 != NULL );
    const SmtTerm* term1 = mSolver->make_var_term(var1);
    CHECK( term1 != NULL );

    const SmtVar* var2 = mSolver->make_var(input_sort2);
    CHECK( var2 != NULL );
    const SmtTerm* term2 = mSolver->make_var_term(var2);
    CHECK( term2 != NULL );

    vector<const SmtTerm*> arg_list(2);
    arg_list[0] = term1;
    arg_list[1] = term2;
    const SmtTerm* term = mSolver->make_fun_term(fun_type, arg_list);
    if ( exp ) {
      CHECK( term != NULL );
      LONGS_EQUAL( SmtTerm::kFunTerm, term->type() );
      LONGS_EQUAL( fun_type, term->function_type() );
      CHECK( term->function() == NULL );
      LONGS_EQUAL( 2, term->input_num() );
      CHECK( term1 == term->input(0) );
      CHECK( term2 == term->input(1) );
    }
    else {
      CHECK( term == NULL );
    }
  }

  // 3引数の builtin function term のテスト
  void builtin_fun_test3(tSmtFun fun_type,
			 tSmtSortId input_sort1,
			 tSmtSortId input_sort2,
			 tSmtSortId input_sort3,
			 bool exp)
  {
    const SmtVar* var1 = mSolver->make_var(input_sort1);
    CHECK( var1 != NULL );
    const SmtTerm* term1 = mSolver->make_var_term(var1);
    CHECK( term1 != NULL );

    const SmtVar* var2 = mSolver->make_var(input_sort2);
    CHECK( var2 != NULL );
    const SmtTerm* term2 = mSolver->make_var_term(var2);
    CHECK( term2 != NULL );

    const SmtVar* var3 = mSolver->make_var(input_sort3);
    CHECK( var3 != NULL );
    const SmtTerm* term3 = mSolver->make_var_term(var3);
    CHECK( term3 != NULL );

    vector<const SmtTerm*> arg_list(3);
    arg_list[0] = term1;
    arg_list[1] = term2;
    arg_list[2] = term3;
    const SmtTerm* term = mSolver->make_fun_term(fun_type, arg_list);
    if ( exp ) {
      CHECK( term != NULL );
      LONGS_EQUAL( SmtTerm::kFunTerm, term->type() );
      LONGS_EQUAL( fun_type, term->function_type() );
      CHECK( term->function() == NULL );
      LONGS_EQUAL( 3, term->input_num() );
      CHECK( term1 == term->input(0) );
      CHECK( term2 == term->input(1) );
      CHECK( term3 == term->input(2) );
    }
    else {
      CHECK( term == NULL );
    }
  }

};

// setup(), teardown() でメモリリークが発生していないことを確認するためのテスト
TEST(SmtSolverTestGroup, empty)
{
}

// set_logic テスト
TEST(SmtSolverTestGroup, set_logic)
{
  // 最初は成功するはず．
  bool stat1 = mSolver->set_logic(kSmtLogic_QF_UF);
  CHECK( stat1 );

  // 二回目は失敗するはず．
  bool stat2 = mSolver->set_logic(kSmtLogic_AUFLIA);
  CHECK( !stat2 );
}

// make_sort テスト0
TEST(SmtSolverTestGroup, make_sort0)
{
  // set_logic() を呼ばないと失敗する．

  // 空の引数
  tSmtSortId sort_id = mSolver->make_sort();
  LONGS_EQUAL( kSmtSort_None, sort_id );
}

// make_sort テスト1
TEST(SmtSolverTestGroup, make_sort1)
{
  bool stat1 = mSolver->set_logic(kSmtLogic_QF_LIA);
  CHECK( stat1 );

  // 空の引数
  tSmtSortId sort_id = mSolver->make_sort();
  sortid_test( sort_id );
  const SmtSort* sort = mSolver->get_sort(sort_id);
  CHECK( sort != NULL );
  LONGS_EQUAL( sort_id, sort->id() );
  LONGS_EQUAL( 0, sort->elem_num() );

  // 引数を持つ型
  vector<tSmtSortId> elem_list(1);
  elem_list[0] = sort_id;
  tSmtSortId sort_id2 = mSolver->make_sort(elem_list);
  sortid_test(sort_id2 );
  const SmtSort* sort2 = mSolver->get_sort(sort_id2);
  CHECK( sort2 != NULL );
  LONGS_EQUAL( sort_id2, sort2->id() );
  LONGS_EQUAL( 1, sort2->elem_num() );
  LONGS_EQUAL( sort_id, sort2->elem(0) );
}

// make_var テスト0
TEST(SmtSolverTestGroup, make_var0)
{
  // set_logic() を呼ばないと失敗する．

  const SmtVar* var1 = mSolver->make_var(kSmtSort_Int);
  CHECK( var1 == NULL );
}

// make_var テスト1
TEST(SmtSolverTestGroup, make_var1)
{
  bool stat1 = mSolver->set_logic(kSmtLogic_QF_LIA);
  CHECK( stat1 );

  tSmtSortId sort_id1 = mSolver->make_sort();
  sortid_test( sort_id1 );

  const SmtVar* var1 = mSolver->make_var(kSmtSort_Int);
  var_test( var1, kSmtSort_Int, kSmtVar_Global);

  const SmtVar* var2 = mSolver->make_var(sort_id1, kSmtVar_FunArg);
  var_test( var2, sort_id1, kSmtVar_FunArg);

  const SmtVar* var3 = mSolver->make_var(kSmtSort_Bool, kSmtVar_Forall);
  var_test( var3, kSmtSort_Bool, kSmtVar_Forall);

  const SmtVar* var4 = mSolver->make_var(kSmtSort_Bool, kSmtVar_Exists);
  var_test( var4, kSmtSort_Bool, kSmtVar_Exists);

}

// make_fun テスト0
TEST(SmtSolverTestGroup, make_fun0)
{
  // set_logic() を呼ばないと失敗する．

  const SmtFun* fun1 = mSolver->make_fun(kSmtSort_Int);
  CHECK( fun1 == NULL );
}

// make_fun テスト1
TEST(SmtSolverTestGroup, make_fun1)
{
  bool stat1 = mSolver->set_logic(kSmtLogic_QF_LIA);
  CHECK( stat1 );

  tSmtSortId sort_id1 = mSolver->make_sort();
  sortid_test( sort_id1 );

  const SmtFun* fun1 = mSolver->make_fun(kSmtSort_Int);
  fun_test( fun1, vector<tSmtSortId>(0), kSmtSort_Int, NULL);

  const SmtFun* fun2 = mSolver->make_fun(vector<tSmtSortId>(0), kSmtSort_Int);
  fun_test( fun2, vector<tSmtSortId>(0), kSmtSort_Int, NULL);

  vector<tSmtSortId> input_sort_list1(2);
  input_sort_list1[0] = kSmtSort_Bool;
  input_sort_list1[1] = kSmtSort_Bool;
  const SmtFun* fun3 = mSolver->make_fun(input_sort_list1, kSmtSort_Bool);
  fun_test( fun3, input_sort_list1, kSmtSort_Bool, NULL);
}

// make_numeral_term テスト
TEST(SmtSolverTestGroup, make_numeral_term)
{
  bool stat1 = mSolver->set_logic(kSmtLogic_QF_LIA);
  CHECK( stat1 );

  const ymuint32 int_val = 1234;
  const SmtTerm* term1 = mSolver->make_numeral_term(int_val);
  CHECK( term1 != NULL );
  LONGS_EQUAL( SmtTerm::kNumConst, term1->type() );
  LONGS_EQUAL( int_val, term1->int_value() );
}

// make_decimal_term テスト
TEST(SmtSolverTestGroup, make_decimal_term)
{
  bool stat1 = mSolver->set_logic(kSmtLogic_QF_LIA);
  CHECK( stat1 );

  const char* dec_str = "1.234";
  const SmtTerm* term1 = mSolver->make_decimal_term(ShString(dec_str));
  CHECK( term1 != NULL );
  LONGS_EQUAL( SmtTerm::kDecConst, term1->type() );
  STRCMP_EQUAL( dec_str, static_cast<const char*>(term1->str_value()) );
}

// make_hexadecimal_term テスト
TEST(SmtSolverTestGroup, make_hexadecimal_term)
{
  bool stat1 = mSolver->set_logic(kSmtLogic_QF_LIA);
  CHECK( stat1 );

  const char* hex_str = "x12AF";
  const SmtTerm* term1 = mSolver->make_hexadecimal_term(ShString(hex_str));
  CHECK( term1 != NULL );
  LONGS_EQUAL( SmtTerm::kHexConst, term1->type() );
  STRCMP_EQUAL( hex_str, static_cast<const char*>(term1->str_value()) );
}

// make_binary_term テスト
TEST(SmtSolverTestGroup, make_binary_term)
{
  bool stat1 = mSolver->set_logic(kSmtLogic_QF_LIA);
  CHECK( stat1 );

  const char* bin_str = "b1001";
  const SmtTerm* term1 = mSolver->make_binary_term(ShString(bin_str));
  CHECK( term1 != NULL );
  LONGS_EQUAL( SmtTerm::kBinConst, term1->type() );
  STRCMP_EQUAL( bin_str, static_cast<const char*>(term1->str_value()) );
}

// make_string_term テスト
TEST(SmtSolverTestGroup, make_string_term)
{
  bool stat1 = mSolver->set_logic(kSmtLogic_QF_LIA);
  CHECK( stat1 );

  const char* str_str = "abcdef";
  const SmtTerm* term1 = mSolver->make_string_term(ShString(str_str));
  CHECK( term1 != NULL );
  LONGS_EQUAL( SmtTerm::kStrConst, term1->type() );
  STRCMP_EQUAL( str_str, static_cast<const char*>(term1->str_value()) );
}

// make_var_term テスト
TEST(SmtSolverTestGroup, make_var_term)
{
  bool stat1 = mSolver->set_logic(kSmtLogic_QF_LIA);
  CHECK( stat1 );

  const SmtVar* var1 = mSolver->make_var(kSmtSort_Bool);
  CHECK( var1 != NULL );

  const SmtTerm* term1 = mSolver->make_var_term(var1);
  CHECK( term1 != NULL );
  LONGS_EQUAL( SmtTerm::kVarTerm, term1->type() );
  CHECK( var1 == term1->var() );
}

// make_fun_term テスト1
TEST(SmtSolverTestGroup, make_fun_term1)
{
  bool stat1 = mSolver->set_logic(kSmtLogic_QF_LIA);
  CHECK( stat1 );

  const SmtFun* fun0 = mSolver->make_fun(kSmtSort_Bool);
  CHECK( fun0 != NULL );
  const SmtTerm* term0 = mSolver->make_fun_term(fun0);
  CHECK( term0 != NULL );
  LONGS_EQUAL( SmtTerm::kFunTerm, term0->type() );
  CHECK( fun0 == term0->function() );

  const SmtTerm* term1 = mSolver->make_fun_term(fun0, vector<const SmtTerm*>(0));
  CHECK( term1 != NULL );
  LONGS_EQUAL( SmtTerm::kFunTerm, term1->type() );
  CHECK( fun0 == term1->function() );

  vector<tSmtSortId> input_sort_list1(2);
  input_sort_list1[0] = kSmtSort_Bool;
  input_sort_list1[1] = kSmtSort_Bool;
  const SmtFun* fun1 = mSolver->make_fun(input_sort_list1, kSmtSort_Bool);
  CHECK( fun1 != NULL );

  const SmtVar* var1 = mSolver->make_var(kSmtSort_Bool);
  CHECK( var1 != NULL );

  const SmtVar* var2 = mSolver->make_var(kSmtSort_Bool);
  CHECK( var2 != NULL );

  vector<const SmtTerm*> arg_list(2);
  arg_list[0] = mSolver->make_var_term(var1);
  arg_list[1] = mSolver->make_var_term(var2);
  const SmtTerm* term2 = mSolver->make_fun_term(fun1, arg_list);
  CHECK( term2 != NULL );
  LONGS_EQUAL( SmtTerm::kFunTerm, term2->type() );
  CHECK( fun1 == term2->function() );
  LONGS_EQUAL( arg_list.size(), term2->input_num() );
  for (ymuint i = 0; i < arg_list.size(); ++ i) {
    CHECK( arg_list[i] == term2->input(i) );
  }
}

// make_fun_term テスト2
TEST(SmtSolverTestGroup, make_fun_term2)
{
  // 引数なしの組み込み関数

  bool stat1 = mSolver->set_logic(kSmtLogic_QF_LIA);
  CHECK( stat1 );

  // まずは成功する例
  builtin_fun_test0(kSmtFun_True, true);
  builtin_fun_test0(kSmtFun_True, true);

  // 失敗する例
#if 0
  builtin_fun_test0(kSmtFun_Not, false);
  builtin_fun_test0(kSmtFun_And, false);
  builtin_fun_test0(kSmtFun_Or, false);
  builtin_fun_test0(kSmtFun_Xor, false);
  builtin_fun_test0(kSmtFun_Imp, false);
  builtin_fun_test0(kSmtFun_Eq, false);
  builtin_fun_test0(kSmtFun_Diseq, false);
  builtin_fun_test0(kSmtFun_Ite, false);
  builtin_fun_test0(kSmtFun_Uminus, false);
  builtin_fun_test0(kSmtFun_Add, false);
  builtin_fun_test0(kSmtFun_Sub, false);
  builtin_fun_test0(kSmtFun_Mul, false);
  builtin_fun_test0(kSmtFun_Div, false);
  builtin_fun_test0(kSmtFun_Le, false);
  builtin_fun_test0(kSmtFun_Lt, false);
  builtin_fun_test0(kSmtFun_Ge, false);
  builtin_fun_test0(kSmtFun_Gt, false);
#endif
}

// make_fun_term テスト3
TEST(SmtSolverTestGroup, make_fun_term3)
{
  // 1引数の組み込み関数

  bool stat1 = mSolver->set_logic(kSmtLogic_QF_LIA);
  CHECK( stat1 );

  tSmtSortId sort1 = mSolver->make_sort();
  sortid_test( sort1 );

  tSmtSortId sort2 = mSolver->make_sort();
  sortid_test( sort2 );

  // まずは成功する例
  builtin_fun_test1(kSmtFun_Not, kSmtSort_Bool, true);
  builtin_fun_test1(kSmtFun_Uminus, kSmtSort_Int, true);
  builtin_fun_test1(kSmtFun_Uminus, kSmtSort_Real, true);

#if 0
  // 型が合わなくて失敗する例
  builtin_fun_test1(kSmtFun_Not, kSmtSort_Int, false);
  builtin_fun_test1(kSmtFun_Not, kSmtSort_Real, false);
  builtin_fun_test1(kSmtFun_Not, sort1, false);

  builtin_fun_test1(kSmtFun_Uminus, kSmtSort_Bool, false);
  builtin_fun_test1(kSmtFun_Uminus, sort1, false);

  // 引数の数が合わなくて失敗する例
  builtin_fun_test1(kSmtFun_True, kSmtSort_Bool, false);
  builtin_fun_test1(kSmtFun_False, kSmtSort_Bool, false);
#endif
}

// make_fun_term テスト4
TEST(SmtSolverTestGroup, make_fun_term4)
{
  bool stat1 = mSolver->set_logic(kSmtLogic_QF_LRA);
  CHECK( stat1 );

  tSmtSortId sort1 = mSolver->make_sort();
  sortid_test( sort1 );

  tSmtSortId sort2 = mSolver->make_sort();
  sortid_test( sort2 );


  builtin_fun_test2(kSmtFun_And, kSmtSort_Bool, kSmtSort_Bool, true);
  builtin_fun_test2(kSmtFun_Or, kSmtSort_Bool, kSmtSort_Bool, true);
  builtin_fun_test2(kSmtFun_Xor, kSmtSort_Bool, kSmtSort_Bool, true);
  builtin_fun_test2(kSmtFun_Imp, kSmtSort_Bool, kSmtSort_Bool, true);

  builtin_fun_test2(kSmtFun_Eq, sort1, sort1, true);
  builtin_fun_test2(kSmtFun_Diseq, sort1, sort1, true);

  builtin_fun_test3(kSmtFun_Ite, kSmtSort_Bool, sort1, sort1, true);

  builtin_fun_test2(kSmtFun_Add, kSmtSort_Int, kSmtSort_Int, true);
  builtin_fun_test2(kSmtFun_Sub, kSmtSort_Int, kSmtSort_Int, true);
  builtin_fun_test2(kSmtFun_Mul, kSmtSort_Int, kSmtSort_Int, true);
  builtin_fun_test2(kSmtFun_Le, kSmtSort_Int, kSmtSort_Int, true);
  builtin_fun_test2(kSmtFun_Lt, kSmtSort_Int, kSmtSort_Int, true);
  builtin_fun_test2(kSmtFun_Ge, kSmtSort_Int, kSmtSort_Int, true);
  builtin_fun_test2(kSmtFun_Gt, kSmtSort_Int, kSmtSort_Int, true);

  builtin_fun_test2(kSmtFun_Add, kSmtSort_Real, kSmtSort_Real, true);
  builtin_fun_test2(kSmtFun_Sub, kSmtSort_Real, kSmtSort_Real, true);
  builtin_fun_test2(kSmtFun_Mul, kSmtSort_Real, kSmtSort_Real, true);
  builtin_fun_test2(kSmtFun_Div, kSmtSort_Real, kSmtSort_Real, true);
  builtin_fun_test2(kSmtFun_Le, kSmtSort_Real, kSmtSort_Real, true);
  builtin_fun_test2(kSmtFun_Lt, kSmtSort_Real, kSmtSort_Real, true);
  builtin_fun_test2(kSmtFun_Ge, kSmtSort_Real, kSmtSort_Real, true);
  builtin_fun_test2(kSmtFun_Gt, kSmtSort_Real, kSmtSort_Real, true);

}
