
/// @file libym_techmap/test/patgen_test.cc
/// @brief PatGen のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "patgen/patgen_nsdef.h"
#include "patgen/PatGen.h"
#include "patgen/PgNode.h"
#include "ym_lexp/LogExpr.h"
#include "PatMgr.h"


BEGIN_NAMESPACE_YM_TECHMAP_PATGEN

void
display_pat(ostream& s,
	    PgHandle h)
{
  if ( h.inv() ) {
    s << "~";
  }
  s << "Node#" << h.node()->id();
}

void
test()
{
  LogExpr var0 = LogExpr::make_posiliteral(0);
  LogExpr var1 = LogExpr::make_posiliteral(1);
  LogExpr var2 = LogExpr::make_posiliteral(2);
  LogExpr var3 = LogExpr::make_posiliteral(3);

  PatGen patgen;

  LogExpr expr1 = var0 & var1;
  vector<PgHandle> pat_list1;
  patgen(expr1, pat_list1);

  LogExpr expr11 = var0 ^ var1;
  vector<PgHandle> pat_list11;
  patgen(expr11, pat_list11);

  LogExpr expr2 = var0 & var1 & var2 & var3;
  vector<PgHandle> pat_list2;
  patgen(expr2, pat_list2);

  LogExpr expr3 = (var0 & var1) | (var2 & var3);
  vector<PgHandle> pat_list3;
  patgen(expr3, pat_list3);

  cout << "pg_display() " << endl;
  pg_display(cout, patgen);
  cout << endl;

  cout << "Patterns for " << expr1 << ":";
  for (vector<PgHandle>::iterator p = pat_list1.begin();
       p != pat_list1.end(); ++ p) {
    display_pat(cout, *p);
  }
  cout << endl;

  cout << "Patterns for " << expr11 << ":";
  for (vector<PgHandle>::iterator p = pat_list11.begin();
       p != pat_list11.end(); ++ p) {
    display_pat(cout, *p);
  }
  cout << endl;

  cout << "Patterns for " << expr2 << ":";
  for (vector<PgHandle>::iterator p = pat_list2.begin();
       p != pat_list2.end(); ++ p) {
    display_pat(cout, *p);
  }
  cout << endl;

  cout << "Patterns for " << expr3 << ":";
  for (vector<PgHandle>::iterator p = pat_list3.begin();
       p != pat_list3.end(); ++ p) {
    display_pat(cout, *p);
  }
  cout << endl;

  const char* filename = "patgraph_test.data";
  {
    ofstream ofs;
    ofs.open(filename, ios::binary);
    if ( !ofs ) {
      // エラー
      cerr << "Could not create " << filename << endl;
      return;
    }

    pg_dump(ofs, patgen);
  }

  PatMgr pat_mgr;
  {
    ifstream ifs;
    ifs.open(filename, ios::binary);
    if ( !ifs ) {
      // エラー
      cerr << "Could not open " << filename << endl;
      return;
    }


    pat_mgr.load(ifs);
  }

  dump(cout, pat_mgr);
}

END_NAMESPACE_YM_TECHMAP_PATGEN


int
main(int argc,
     char** argv)
{
  nsYm::nsTechmap::nsPatgen::test();

  return 0;
}
