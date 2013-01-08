
/// @file lmtest.cc
/// @brief LogicMgr のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "LogicMgr.h"


BEGIN_NAMESPACE_YM_NETWORKS_TGNET

void
test1(const LogExpr& expr,
      LogicMgr& lm)
{
  cout << "Registering: " << expr << endl;
  ymuint32 id;
  tTgNodeType type = lm.reg_logic(expr, id);
  ymuint ni = expr.input_size();

  cout << "  TYPE = ";
  switch ( type ) {
  case kTgUndef:  cout << "--UNDEF--"; break;

  case kTgInput:  cout << "--INPUT--"; break;
  case kTgOutput: cout << "--OUTPUT--"; break;

  case kTgConst0: cout << "--CONST0--"; break;
  case kTgConst1: cout << "--CONST1--"; break;

  case kTgBuff:   cout << "--BUFF--"; break;
  case kTgNot:    cout << "--NOT--"; break;

  case kTgAnd:    cout << "--AND(" << ni << ")--"; break;

  case kTgNand:   cout << "--NAND(" << ni << ")--"; break;

  case kTgOr:     cout << "--OR(" << ni << ")--"; break;

  case kTgNor:    cout << "--NOR(" << ni << ")--"; break;

  case kTgXor:    cout << "--XOR(" << ni << ")--"; break;

  case kTgXnor:   cout << "--XNOR(" << ni << ")--"; break;

  case kTgCplx:   cout << "--CPLX#" << id << " (" << ni << ")--"; break;

  default:
    cout << "--ERROR--"; break;
  }
  cout << endl;
}

END_NAMESPACE_YM_NETWORKS_TGNET


///////////////////////////////////////////////////////////////////////////////
// Main.
///////////////////////////////////////////////////////////////////////////////

int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm;
  using namespace nsYm::nsNetworks::nsTgnet;

  try {
    LogExpr v0 = LogExpr::make_posiliteral(VarId(0));
    LogExpr v1 = LogExpr::make_posiliteral(VarId(1));
    LogExpr v2 = LogExpr::make_posiliteral(VarId(2));
    LogExpr v3 = LogExpr::make_posiliteral(VarId(3));
    LogExpr v4 = LogExpr::make_posiliteral(VarId(4));
    LogExpr v5 = LogExpr::make_posiliteral(VarId(5));
    LogExpr v6 = LogExpr::make_posiliteral(VarId(6));
    LogExpr v7 = LogExpr::make_posiliteral(VarId(7));

    LogicMgr lm;

    LogExpr tmp0 = v0;
    test1(tmp0, lm);

    LogExpr tmp1 = (v0 & v1) | ~v1;
    test1(tmp1, lm);

    LogExpr tmp2 = v0 | ~v1;
    test1(tmp2, lm);

    LogExpr tmp3 = v0 & v1 | v2 & v3 | v4 & v5 | v6 & v7;
    test1(tmp3, lm);
  }
  catch ( AssertError x) {
    cout << x << endl;
  }

  return 0;
}
