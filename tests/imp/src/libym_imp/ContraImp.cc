
/// @file ContraImp.cc
/// @brief ContraImp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ContraImp.h"
#include "ImpMgr.h"
#include "ImpInfo.h"
#include "ImpVal.h"
#include "ImpListRec2.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
// クラス ContraImp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ContraImp::ContraImp()
{
}

// @brief デストラクタ
ContraImp::~ContraImp()
{
}

// @brief ネットワーク中の間接含意を求める．
// @param[in] imp_mgr マネージャ
// @param[in] imp_info 間接含意のリスト
void
ContraImp::learning(ImpMgr& imp_mgr,
		    ImpInfo& imp_info)
{
  ymuint n = imp_mgr.node_num();

  imp_info.set_size(n);

  // まず direct_imp の情報を imp_list_array にコピーする．
  // 同時に対偶も imp_list_array に追加する．
  vector<vector<ImpVal> > imp_list_array(n * 2);
  ImpListRec2 rec(imp_list_array);
  for (ymuint src_id = 0; src_id < n; ++ src_id) {
    ImpNode* node = imp_mgr.node(src_id);

    for (ymuint val = 0; val < 2; ++ val) {
      // node に val を割り当てる．
      bool ok = imp_mgr.assert(node, val, rec);
      imp_mgr.backtrack();
      if ( !ok ) {
	// 単一の割り当てで矛盾が起こった．
	// node は val ^ 1 固定
	imp_mgr.set_const(src_id, val ^ 1);
      }
    }
  }

  // imp_list_array の内容を imp_info にコピーする．
  imp_info.set(imp_list_array);
}

END_NAMESPACE_YM_NETWORKS
