
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

  // SmtVar のテスト関数
  void var_test(const SmtVar* var,
		const SmtSort* sort,
		tSmtVar var_type)
  {
    CHECK( var != NULL );
    CHECK( var->sort() == sort );
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
		const vector<const SmtSort*>& input_sort_list,
		const SmtSort* output_sort,
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

// make_sort テスト
TEST(SmtSolverTestGroup, make_sort)
{
  bool stat1 = mSolver->set_logic(kSmtLogic_QF_LIA);
  CHECK( stat1 );

  // 空の引数
  const SmtSort* sort = mSolver->make_sort();
  CHECK( sort != NULL );
  LONGS_EQUAL( kSmtSort_UserDef, sort->type() );
  LONGS_EQUAL( 0, sort->elem_num() );

  // 引数を持つ型
  vector<const SmtSort*> elem_list(1);
  elem_list[0] = sort;
  const SmtSort* sort2 = mSolver->make_sort(elem_list);
  CHECK( sort2 != NULL );
  LONGS_EQUAL( kSmtSort_UserDef, sort2->type() );
  LONGS_EQUAL( 1, sort2->elem_num() );
  CHECK( sort == sort2->elem(0) );

  // 組み込み型
  const SmtSort* bool_sort = mSolver->make_builtin_sort(kSmtSort_Bool);
  LONGS_EQUAL( kSmtSort_Bool, bool_sort->type() );

  const SmtSort* int_sort = mSolver->make_builtin_sort(kSmtSort_Int);
  LONGS_EQUAL( kSmtSort_Int, int_sort->type() );

  const SmtSort* real_sort = mSolver->make_builtin_sort(kSmtSort_Real);
  LONGS_EQUAL( kSmtSort_Real, real_sort->type() );
}

// make_var テスト
TEST(SmtSolverTestGroup, make_var)
{
  bool stat1 = mSolver->set_logic(kSmtLogic_QF_LIA);
  CHECK( stat1 );

  const SmtSort* int_sort = mSolver->make_builtin_sort(kSmtSort_Int);
  CHECK( int_sort != NULL );

  const SmtSort* bool_sort = mSolver->make_builtin_sort(kSmtSort_Bool);
  CHECK( bool_sort );

  const SmtSort* sort1 = mSolver->make_sort();
  CHECK( sort1 != NULL);

  const SmtVar* var1 = mSolver->make_var(int_sort);
  var_test( var1, int_sort, kSmtVar_Global);

  const SmtVar* var2 = mSolver->make_var(sort1, kSmtVar_FunArg);
  var_test( var2, sort1, kSmtVar_FunArg);

  const SmtVar* var3 = mSolver->make_var(bool_sort, kSmtVar_Forall);
  var_test( var3, bool_sort, kSmtVar_Forall);

  const SmtVar* var4 = mSolver->make_var(bool_sort, kSmtVar_Exists);
  var_test( var4, bool_sort, kSmtVar_Exists);

}

// make_fun テスト
TEST(SmtSolverTestGroup, make_fun)
{
  bool stat1 = mSolver->set_logic(kSmtLogic_QF_LIA);
  CHECK( stat1 );

  const SmtSort* int_sort = mSolver->make_builtin_sort(kSmtSort_Int);
  CHECK( int_sort != NULL );

  const SmtSort* bool_sort = mSolver->make_builtin_sort(kSmtSort_Bool);
  CHECK( bool_sort );

  const SmtSort* sort1 = mSolver->make_sort();
  CHECK( sort1 != NULL);

  const SmtFun* fun1 = mSolver->make_fun(int_sort);
  fun_test( fun1, vector<const SmtSort*>(0), int_sort, NULL);

  const SmtFun* fun2 = mSolver->make_fun(vector<const SmtSort*>(0), int_sort);
  fun_test( fun2, vector<const SmtSort*>(0), int_sort, NULL);

  vector<const SmtSort*> input_sort_list1(2);
  input_sort_list1[0] = bool_sort;
  input_sort_list1[1] = bool_sort;
  const SmtFun* fun3 = mSolver->make_fun(input_sort_list1, bool_sort);
  fun_test( fun3, input_sort_list1, bool_sort, NULL);
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
