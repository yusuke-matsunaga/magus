
/// @file StrImp.cc
/// @brief StrImp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "StrImp.h"
#include "ImpMgr.h"
#include "ImpNode.h"
#include "ImpInfo.h"
#include "ImpListRec.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
// クラス StrImp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
StrImp::StrImp()
{
}

// @brief デストラクタ
StrImp::~StrImp()
{
}

// @brief ネットワーク中の間接含意を求める．
// @param[in] imp_mgr マネージャ
// @param[in] imp_info 間接含意のリスト
void
StrImp::learning(ImpMgr& imp_mgr,
		 ImpInfo& imp_info)
{
  ymuint n = imp_mgr.node_num();

  imp_info.set_size(n);

  vector<vector<ImpVal> > imp_list_array(n * 2);
  for (ymuint src_id = 0; src_id < n; ++ src_id) {
    ImpNode* node = imp_mgr.node(src_id);

    for (ymuint val = 0; val < 2; ++ val) {
      // node に val を割り当てる．
      ImpListRec rec(imp_list_array[src_id * 2 + val]);
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
