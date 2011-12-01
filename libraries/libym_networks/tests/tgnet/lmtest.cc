
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
  TgGateTemplate id = lm.reg_logic(expr);
  cout << "  ID = " << id << endl;
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

    lm.dump(cout);
  }
  catch ( AssertError x) {
    cout << x << endl;
  }

  return 0;
}
