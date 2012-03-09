
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
  ymuint n = imp_mgr.max_bnode_id();

  imp_info.set_size(n);

  for (ymuint i = 0; i < n; ++ i) {
    ImpNodeHandle handle = imp_mgr.bnode_handle(i);
    if ( handle.is_const() ) continue;

    ImpNode* node = handle.node();
    ymuint src_id = node->id();

    // node に 0 を割り当てる．
    vector<ImpVal> imp_list0;
    bool ok0 = imp_mgr.assert(node, 0, imp_list0);
    if ( ok0 ) {
      imp_info.put(src_id, 0, imp_list0);
    }
    else {
      // 単一の割り当てで矛盾が起こった．
      // node は 1 固定
    }
    imp_mgr.backtrack();

    // node に 1 を割り当てる．
    vector<ImpVal> imp_list1;
    bool ok1 = imp_mgr.assert(node, 1, imp_list1);
    if ( ok1 ) {
      imp_info.put(src_id, 1, imp_list1);
    }
    else {
      // 単一の割り当てで矛盾が起こった．
      // node は 0 固定
    }
    imp_mgr.backtrack();
  }
#if 0
  cout << "DIRECT IMPLICATION" << endl;
  imp_info.print(cout);
#endif
}

END_NAMESPACE_YM_NETWORKS
