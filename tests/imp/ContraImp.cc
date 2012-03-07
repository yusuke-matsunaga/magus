
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
#include "ImpList.h"

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
  ymuint n = imp_mgr.max_node_id();

  imp_info.set_size(n);

  // 対偶が direct_imp に登録されていなかったら imp_info に追加する．
  for (ymuint src_id = 0; src_id < n; ++ src_id) {
    for (ymuint src_val = 0; src_val <= 1; ++ src_val) {
      const ImpList& imp_list = direct_imp.get(src_id, src_val);
      for (ImpList::iterator p = imp_list.begin();
	   p != imp_list.end(); ++ p) {
	const ImpCell& imp = *p;
	ymuint dst_id = imp.dst_id();
	ymuint dst_val = imp.dst_val();
	if ( !direct_imp.check(dst_id, dst_val ^ 1, src_id, src_val ^ 1) ) {
	  imp_info.put(dst_id, dst_val ^ 1, src_id, src_val ^ 1);
	}
      }
    }
  }
}

END_NAMESPACE_YM_NETWORKS
