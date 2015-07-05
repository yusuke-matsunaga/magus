
/// @file magus/src/seal/SealCmd.cc
/// @brief SealCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SealCmd.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "SealCmd.h"
#include "YmNetworks/BNetManip.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// ソフトエラー解析を行うコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SealCmd::SealCmd(MagMgr* mgr) :
  BNetCmd(mgr)
{
}

// @brief デストラクタ
SealCmd::~SealCmd()
{
}

// コマンド処理関数
int
SealCmd::cmd_proc(TclObjVector& objv)
{
  BNetwork& network = *cur_network();
  BNetManip manip(&network);

  vector<BNode*> fanins;
  Expr l0 = Expr::make_posiliteral(VarId(0));
  Expr l1 = Expr::make_posiliteral(VarId(1));
  Expr l2 = Expr::make_posiliteral(VarId(2));
  Expr maj_exp = (l0 & l1) | (l1 & l2) | (l2 & l0);
  for (BNodeList::const_iterator p = network.outputs_begin();
       p != network.outputs_end(); ++ p) {
    // 外部出力
    BNode* obnode = *p;
    // 外部出力から参照されているノード
    BNode* ibnode = obnode->fanin(0);

    // ibnode の複製を3つ作る．
    size_t ni = ibnode->fanin_num();
    fanins.resize(ni);
    for (size_t i = 0; i < ni; ++ i) {
      fanins[i] = ibnode->fanin(i);
    }
    Expr lexp = ibnode->func();
    BNode* ibnode0 = manip.new_logic();
    bool stat = manip.change_logic(ibnode0, lexp, fanins);
    ASSERT_COND(stat );
    BNode* ibnode1 = manip.new_logic();
    stat = manip.change_logic(ibnode1, lexp, fanins);
    ASSERT_COND(stat );
    BNode* ibnode2 = manip.new_logic();
    stat = manip.change_logic(ibnode2, lexp, fanins);
    ASSERT_COND(stat );

    // その3つの多数決論理を作る．
    fanins.resize(3);
    fanins[0] = ibnode0;
    fanins[1] = ibnode1;
    fanins[2] = ibnode2;
    stat = manip.change_logic(ibnode, maj_exp, fanins);
    ASSERT_COND(stat );
  }

  return TCL_OK;
}

END_NAMESPACE_MAGUS
