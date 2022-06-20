﻿
/// @file libym_techmap/test/pgfuncmgr_test.cc
/// @brief PatGen のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "cellmap/patgen/patgen_nsdef.h"
#include "cellmap/patgen/PgFuncMgr.h"
#include "cellmap/patgen/PgFuncRep.h"
#include "cellmap/patgen/PgFunc.h"
#include "ym_lexp/Expr.h"
#include "cellmap/PatMgr.h"


BEGIN_NAMESPACE_YM_CELLMAP_PATGEN

Expr
str_to_expr(char* str,
	    ymuint& pos)
{
  // 最初の空白を読み飛ばす．
  while ( isspace(str[pos]) ) {
    ++ pos;
  }

  // 次の空白の位置を求める．
  ymuint end;
  for (end = pos + 1; str[end] != '\0' && !isspace(str[end]); ++ pos) { }

  Expr expr;
  if ( end == pos + 1 ) {
    char op = str[pos];
    ++ pos;
    if ( op == '~' || op == '!' ) {
      Expr expr1 = str_to_expr(str, pos);
      expr = ~expr1;
    }
    else if ( op == '&' || op == '*' ) {
      Expr expr1 = str_to_expr(str, pos);
      Expr expr2 = str_to_expr(str, pos);
      expr = expr1 & expr2;
    }
    else if ( op == '|' || op == '+' ) {
      Expr expr1 = str_to_expr(str, pos);
      Expr expr2 = str_to_expr(str, pos);
      expr = expr1 | expr2;
    }
    else if ( op == '^' ) {
      Expr expr1 = str_to_expr(str, pos);
      Expr expr2 = str_to_expr(str, pos);
      expr = expr1 ^ expr2;
    }
    else if ( '0' <= op && op <= '9' ) {
      int v = static_cast<int>(op) - '0';
      expr = Expr::make_posiliteral(v);
    }
  }
  else {
    // 数字でなければならない．
    char oc = str[end];
    str[end] = '\0';
    int v = atoi(str);
    expr = Expr::make_posiliteral(v);
    str[end] = oc;
  }

  return expr;
}

void
test(istream& s)
{
#if 0
  // s の各行が逆ポーランド式の論理式だと思う．
  vector<Expr> expr_list;
  char buff[4096];
  while ( s.getline(buff, sizeof(buff), '\n') ) {
    ymuint pos = 0;
    Expr expr = str_to_expr(buff, pos);
    expr_list.push_back(expr);
  }

  PgFuncMgr pgf_mgr;

  for (vector<Expr>::iterator p = expr_list.begin();
       p != expr_list.end(); ++ p) {
    Expr expr = *p;
    ymuint fid = pgf_mgr.reg_expr(expr);
    cout << "Function ID for " << expr << " = " << fid << endl;
  }
  pg_display(cout, pgf_mgr);
#endif

}

END_NAMESPACE_YM_CELLMAP_PATGEN


int
main(int argc,
     char** argv)
{
  using namespace std;
  using nsYm::nsCellmap::nsPatgen::test;

  if ( argc == 2 ) {
    ifstream ifs;
    ifs.open(argv[1]);
    if ( !ifs ) {
      cerr << "Error opening " << argv[2] << endl;
      return 1;
    }
    test(ifs);
  }
  else {
    test(cin);
  }

  return 0;
}
