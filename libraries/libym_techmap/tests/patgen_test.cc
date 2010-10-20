
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
dump_node(ostream& s,
	  PgNode* node)
{
  s << "Node#" << node->id() << ": ";
  if ( node->is_input() ) {
    s << "Input#" << node->input_id() << endl;
  }
  else {
    PgNode* l_node = node->fanin(0);
    PgNode* r_node = node->fanin(1);
    bool l_inv = node->fanin_inv(0);
    bool r_inv = node->fanin_inv(1);
    if ( l_inv ) {
      s << "~";
    }
    s << l_node->id() << " ";
    if ( node->is_and() ) {
      s << "&";
    }
    else {
      s << "^";
    }
    s << " ";
    if ( r_inv ) {
      s << "~";
    }
    s << r_node->id() << endl;
    dump_node(s, l_node);
    dump_node(s, r_node);
  }
}

void
dump_pat(ostream& s,
	 PgHandle h)
{
  PgNode* node = h.node();
  bool inv = h.inv();
  s << "Root: ";
  if ( inv ) {
    s << "~";
  }
  s << node->id() << endl;
  dump_node(s, node);
  s << endl;
}

void
test(const LogExpr& expr)
{
  PatGen patgen;
  vector<PgHandle> pg_list;
  patgen.gen_pat(expr, pg_list);

  cout << "PatGen Test for " << expr << endl;
  for (vector<PgHandle>::iterator p = pg_list.begin();
       p != pg_list.end(); ++ p) {
    PgHandle h = *p;
    dump_pat(cout, h);
  }
  cout << endl;
}

END_NAMESPACE_YM_TECHMAP_PATGEN


int
main(int argc,
     char** argv)
{
  using nsYm::LogExpr;
  using nsYm::nsTechmap::nsPatgen::test;

  LogExpr var0 = LogExpr::make_posiliteral(0);
  LogExpr var1 = LogExpr::make_posiliteral(1);
  LogExpr var2 = LogExpr::make_posiliteral(2);
  LogExpr var3 = LogExpr::make_posiliteral(3);

  LogExpr expr1 = var0 & var1;
  test(expr1);

  LogExpr expr2 = var0 & var1 & var2 & var3;
  test(expr2);

  LogExpr expr3 = (var0 & var1) | (var2 & var3);
  test(expr3);

  return 0;
}
