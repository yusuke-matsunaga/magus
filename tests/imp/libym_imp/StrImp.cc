
/// @file StrImp.cc
/// @brief StrImp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "StrImp.h"
#include "ImpMgr.h"
#include "ImpNode.h"
#include "ImpInfo.h"


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

    // node に 0 を割り当てる．
    vector<ImpVal>& imp_list0 = imp_list_array[src_id * 2 + 0];
    bool ok0 = imp_mgr.assert(node, 0, imp_list0);
    if ( !ok0 ) {
      imp_list0.clear();
      // 単一の割り当てで矛盾が起こった．
      // node は 1 固定
      imp_mgr.set_const(src_id, 1);
      cout << "Node#" << src_id << " is const-1" << endl;
    }
    imp_mgr.backtrack();

    // node に 1 を割り当てる．
    vector<ImpVal>& imp_list1 = imp_list_array[src_id * 2 + 1];
    bool ok1 = imp_mgr.assert(node, 1, imp_list1);
    if ( !ok1 ) {
      imp_list1.clear();
      // 単一の割り当てで矛盾が起こった．
      // node は 0 固定
      imp_mgr.set_const(src_id, 0);
      cout << "Node#" << src_id << " is const-0" << endl;
    }
    imp_mgr.backtrack();
  }

  // imp_list_array の内容を imp_info にコピーする．
  imp_info.set(imp_list_array);

}

END_NAMESPACE_YM_NETWORKS
