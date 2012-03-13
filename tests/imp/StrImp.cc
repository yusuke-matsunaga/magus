
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

  for (ymuint src_id = 0; src_id < n; ++ src_id) {
    ImpNode* node = imp_mgr.node(src_id);

    // node に 0 を割り当てる．
    vector<ImpDst> imp_list0;
    bool ok0 = imp_mgr.assert(node, 0, imp_list0);
    if ( ok0 ) {
      for (vector<ImpDst>::iterator p = imp_list0.begin();
	   p != imp_list0.end(); ++ p) {
	const ImpDst& imp = *p;
	ImpNode* dst_node = imp.node();
	ymuint dst_id = dst_node->id();
	imp_info.put(src_id, 0, dst_id, imp.val());
      }
    }
    else {
      // 単一の割り当てで矛盾が起こった．
      // node は 1 固定
    }
    imp_mgr.backtrack();

    // node に 1 を割り当てる．
    vector<ImpDst> imp_list1;
    bool ok1 = imp_mgr.assert(node, 1, imp_list1);
    if ( ok1 ) {
      for (vector<ImpDst>::iterator p = imp_list1.begin();
	   p != imp_list1.end(); ++ p) {
	const ImpDst& imp = *p;
	ImpNode* dst_node = imp.node();
	ymuint dst_id = dst_node->id();
	imp_info.put(src_id, 1, dst_id, imp.val());
      }
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
