
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
test()
{
  LogExpr var0 = LogExpr::make_posiliteral(0);
  LogExpr var1 = LogExpr::make_posiliteral(1);
  LogExpr var2 = LogExpr::make_posiliteral(2);
  LogExpr var3 = LogExpr::make_posiliteral(3);

  PatGen patgen;

  LogExpr expr1 = var0 & var1;
  vector<PgHandle> pg_list1;
  patgen(expr1, pg_list1);

  LogExpr expr2 = var0 & var1 & var2 & var3;
  vector<PgHandle> pg_list2;
  patgen(expr2, pg_list2);

  LogExpr expr3 = (var0 & var1) | (var2 & var3);
  vector<PgHandle> pg_list3;
  patgen(expr3, pg_list3);

  cout << "PatGen::dump_graph() " << endl;
  patgen.display_graph(cout);
  cout << endl;

  cout << "Patterns for " << expr1 << endl;
  for (vector<PgHandle>::iterator p = pg_list1.begin();
       p != pg_list1.end(); ++ p) {
    PgHandle h = *p;
    patgen.display_edgelist(cout, h);
  }
  cout << endl;

  cout << "Patterns for " << expr2 << endl;
  for (vector<PgHandle>::iterator p = pg_list2.begin();
       p != pg_list2.end(); ++ p) {
    PgHandle h = *p;
    patgen.display_edgelist(cout, h);
  }
  cout << endl;

  cout << "Patterns for " << expr3 << endl;
  for (vector<PgHandle>::iterator p = pg_list3.begin();
       p != pg_list3.end(); ++ p) {
    PgHandle h = *p;
    patgen.display_edgelist(cout, h);
  }
  cout << endl;

}

END_NAMESPACE_YM_TECHMAP_PATGEN


int
main(int argc,
     char** argv)
{
  nsYm::nsTechmap::nsPatgen::test();

  return 0;
}
