
/// @file libym_techmap/test/patgen_test.cc
/// @brief PatGen のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "patgen/patgen_nsdef.h"
#include "patgen/PgFuncMgr.h"
#include "patgen/PgFuncRep.h"
#include "patgen/PgFunc.h"
#include "ym_lexp/LogExpr.h"
#include "PatMgr.h"


BEGIN_NAMESPACE_YM_TECHMAP_PATGEN

void
test()
{
  LogExpr var0 = LogExpr::make_posiliteral(0);
  LogExpr var1 = LogExpr::make_posiliteral(1);
  LogExpr var2 = LogExpr::make_posiliteral(2);
  LogExpr var3 = LogExpr::make_posiliteral(3);

  LogExpr expr1 = var0 & var1;
  LogExpr expr2 = var0 | var1;
  LogExpr expr3 = var0 ^ var1;
  LogExpr expr4 = var0 & var1 & var2 & var3;
  LogExpr expr5 = (var0 & var1) | (var2 & var3);
  LogExpr expr6 = ~(~var0 | ~var1);

  PgFuncMgr pgf_mgr;

  ymuint fid1 = pgf_mgr.reg_expr(expr1);
  ymuint fid2 = pgf_mgr.reg_expr(expr2);
  ymuint fid3 = pgf_mgr.reg_expr(expr3);
  ymuint fid4 = pgf_mgr.reg_expr(expr4);
  ymuint fid5 = pgf_mgr.reg_expr(expr5);
  ymuint fid6 = pgf_mgr.reg_expr(expr6);

  cout << "*** PgFuncMgr ***" << endl;
  cout << "# of functions:     " << pgf_mgr.func_num() << endl
       << "# of rep functions: " << pgf_mgr.rep_num() << endl
       << endl;

  for (ymuint i = 0; i < pgf_mgr.func_num(); ++ i) {
    const PgFunc* func = pgf_mgr.func(i);
    assert_cond( func->id() == i, __FILE__, __LINE__);
    cout << "FUNC#" << i << ": REP#" << func->rep()->id()
	 << ": " << func->map() << endl;
  }
  cout << endl;

  for (ymuint i = 0; i < pgf_mgr.rep_num(); ++ i) {
    const PgFuncRep* rep = pgf_mgr.rep(i);
    assert_cond( rep->id() == i , __FILE__, __LINE__);
    cout << "REP#" << i << ": ";
    rep->rep_func().dump(cout, 2);
    cout << endl;
    cout << "  equivalence = ";
    for (ymuint j = 0; j < rep->func_num(); ++ j) {
      cout << " FUNC#" << rep->func(j)->id();
    }
    cout << endl;
    cout << "  patterns = ";
    for (ymuint j = 0; j < rep->pat_num(); ++ j) {
      cout << " PAT#" << rep->pat_id(j);
    }
    cout << endl;
  }
  cout << endl;

  cout << "pg_display() " << endl;
  pg_display(cout, pgf_mgr.pat_gen());
  cout << endl;

  cout << "func id for " << expr1 << " = " << fid1 << endl;
  cout << "func id for " << expr2 << " = " << fid2 << endl;
  cout << "func id for " << expr3 << " = " << fid3 << endl;
  cout << "func id for " << expr4 << " = " << fid4 << endl;
  cout << "func id for " << expr5 << " = " << fid5 << endl;
  cout << "func id for " << expr6 << " = " << fid6 << endl;
}

END_NAMESPACE_YM_TECHMAP_PATGEN


int
main(int argc,
     char** argv)
{
  nsYm::nsTechmap::nsPatgen::test();

  return 0;
}
