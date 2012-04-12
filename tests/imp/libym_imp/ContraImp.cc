
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

#include "ym_networks/BdnMgr.h"


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
		    const ImpInfo& direct_imp,
		    ImpInfo& imp_info)
{
  ymuint n = imp_mgr.node_num();

  imp_info.set_size(n);

  // まず direct_imp の情報を imp_list_array にコピーする．
  // 同時に対偶も imp_list_array に追加する．
  vector<vector<ImpVal> > imp_list_array(n * 2);
  for (ymuint src_id = 0; src_id < n; ++ src_id) {
    for (ymuint src_val = 0; src_val <= 1; ++ src_val) {
      const vector<ImpVal>& imp_list = direct_imp.get(src_id, src_val);
      for (vector<ImpVal>::const_iterator p = imp_list.begin();
	   p != imp_list.end(); ++ p) {
	ymuint dst_id = p->id();
	ymuint dst_val = p->val();
	imp_list_array[src_id * 2 + src_val].push_back(ImpVal(dst_id, dst_val));
	imp_list_array[dst_id * 2 + (dst_val ^ 1)].push_back(ImpVal(src_id, src_val ^ 1));
      }
    }
  }

  // imp_list_array の内容を imp_info にコピーする．
  imp_info.set(imp_list_array);
}

END_NAMESPACE_YM_NETWORKS
