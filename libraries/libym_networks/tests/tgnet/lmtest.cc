
/// @file lmtest.cc
/// @brief LogicMgr のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "LogicMgr.h"


BEGIN_NAMESPACE_YM_NETWORKS_TGNET

void
test1(const Expr& expr,
      LogicMgr& lm)
{
  cout << "Registering: " << expr << endl;
  ymuint32 id;
  tTgGateType type = lm.reg_logic(expr, id);
  ymuint ni = expr.input_size();

  cout << "  TYPE = " << type << "(" << ni << ")";
  if ( type == kTgGateCplx ) {
    cout << ", ID = " << id;
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
    Expr v0 = Expr::make_posiliteral(VarId(0));
    Expr v1 = Expr::make_posiliteral(VarId(1));
    Expr v2 = Expr::make_posiliteral(VarId(2));
    Expr v3 = Expr::make_posiliteral(VarId(3));
    Expr v4 = Expr::make_posiliteral(VarId(4));
    Expr v5 = Expr::make_posiliteral(VarId(5));
    Expr v6 = Expr::make_posiliteral(VarId(6));
    Expr v7 = Expr::make_posiliteral(VarId(7));

    LogicMgr lm;

    Expr tmp0 = v0;
    test1(tmp0, lm);

    Expr tmp1 = (v0 & v1) | ~v1;
    test1(tmp1, lm);

    Expr tmp2 = v0 | ~v1;
    test1(tmp2, lm);

    Expr tmp3 = v0 & v1 | v2 & v3 | v4 & v5 | v6 & v7;
    test1(tmp3, lm);
  }
  catch ( AssertError x) {
    cout << x << endl;
  }

  return 0;
}
