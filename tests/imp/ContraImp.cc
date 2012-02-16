
/// @file ContraImp.cc
/// @brief ContraImp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ContraImp.h"
#include "StrImp.h"
#include "ImpInfo.h"

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
// @param[in] network 対象のネットワーク
// @param[in] imp_info 間接含意のリスト
void
ContraImp::learning(const BdnMgr& network,
		    ImpInfo& imp_info)
{
  ymuint n = network.max_node_id();

  imp_info.set_size(n);

  // 直接含意を求める．
  StrImp strimp;
  ImpInfo d_imp;
  strimp.learning(network, d_imp);

  // 対偶が d_imp に登録されていなかったら imp_info に追加する．
  for (ymuint src_id = 0; src_id < n; ++ src_id) {
    for (ymuint src_val = 0; src_val <= 1; ++ src_val) {
      const list<ImpCell>& imp_list = d_imp.get(src_id, src_val);
      for (list<ImpCell>::const_iterator p = imp_list.begin();
	   p != imp_list.end(); ++ p) {
	const ImpCell& imp = *p;
	ymuint dst_id = imp.dst_id();
	ymuint dst_val = imp.dst_val();
	if ( !d_imp.check(dst_id, dst_val ^ 1, src_id, src_val ^ 1) ) {
	  if ( 0 ) {
	    const list<ImpCell>& imp_list = d_imp.get(dst_id, dst_val ^ 1);
	    for (list<ImpCell>::const_iterator p = imp_list.begin();
		 p != imp_list.end(); ++ p) {
	      const ImpCell& imp = *p;
	      cout << "  Node#" << dst_id << ": " << (dst_val ^ 1)
		   << " ==> Node#" << imp.dst_id()
		   << ": " << imp.dst_val()
		   << endl;
	    }
	  }
	  imp_info.put(dst_id, dst_val ^ 1, src_id, src_val ^ 1);
	}
      }
    }
  }
}

END_NAMESPACE_YM_NETWORKS
