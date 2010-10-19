
/// @file libym_techmap/test/patgen_test.cc
/// @brief PatGen のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "patgen/patgen_nsdef.h"
#include "patgen/PatGen.h"
#include "ym_lexp/LogExpr.h"


BEGIN_NAMESPACE_YM_TECHMAP_PATGEN

void
dump_pat(PgHandle h)
{

}

void
test(const LogExpr& expr)
{
  PatGen patgen;
  PgNodeMgr mgr;
  vector<PgHandle> pg_list;
  patgen(expr, mgr, pg_list);

  for (vector<PgHandle>::iterator p = pg_list.begin();
       p != pg_list.end(); ++ p) {
    PgHandle h = *p;
    dump_pat(h);
  }
}

END_NAMESPACE_YM_TECHMAP_PATGEN


int
main(int argc,
     char** argv)
{
  using nsYm::LogExpr;
  using nsYm::nsTechmap::nsPatgen::test;

  LogExpr var0 = LogExpr::make_posiliteral(0);
  LogExpr var1 = LogExpr::make_posiliteral(0);
  LogExpr var2 = LogExpr::make_posiliteral(0);
  LogExpr var3 = LogExpr::make_posiliteral(0);

  LogExpr expr1 = var0 & var1 & var2 & var3;
  test(expr1);

  LogExpr expr2 = (var0 & var1) | (var2 & var3);
  test(expr2);

  return 0;
}
