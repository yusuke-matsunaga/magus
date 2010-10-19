
/// @file libym_techmap/test/patgen_test.cc
/// @brief PatGen のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "patgen/patgen_nsdef.h"
#include "patgen/PatGen.h"


BEGIN_NAMESPACE_YM_TECHMAP_PATGEN

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

  nsYm::nsTechmap::nsPatgen::test(expr);

  return 0;
}
