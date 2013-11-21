
/// @file ConstImp.cc
/// @brief ConstImp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ConstImp.h"
#include "ImpMgr.h"
#include "ImpNode.h"
#include "ImpInfo.h"
#include "ImpListRec.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
// クラス ConstImp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ConstImp::ConstImp()
{
}

// @brief デストラクタ
ConstImp::~ConstImp()
{
}

// @brief ネットワーク中の間接含意を求める．
// @param[in] imp_mgr マネージャ
void
ConstImp::learning(ImpMgr& imp_mgr)
{
  ymuint n = imp_mgr.node_num();
  for (ymuint src_id = 0; src_id < n; ++ src_id) {
    ImpNode* node = imp_mgr.node(src_id);

    for (ymuint val = 0; val < 2; ++ val) {
      // node に val を割り当てる．
      bool ok = imp_mgr.assert(node, val);
      imp_mgr.backtrack();
      if ( !ok ) {
	// 単一の割り当てで矛盾が起こった．
	// node は val ^ 1 固定
	imp_mgr.set_const(src_id, val ^ 1);
      }
    }
  }
}

END_NAMESPACE_YM_NETWORKS
