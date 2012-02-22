
/// @file RlImp.cc
/// @brief RlImp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "RlImp.h"
#include "ImpMgr.h"
#include "StrNode.h"
#include "SnInput.h"
#include "SnAnd.h"
#include "SnXor.h"
#include "ImpInfo.h"
#include "ym_networks/BdnMgr.h"
#include "ym_networks/BdnNode.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
// クラス RlImp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
RlImp::RlImp()
{
}

// @brief デストラクタ
RlImp::~RlImp()
{
}

// @brief ネットワーク中の間接含意を求める．
// @param[in] network 対象のネットワーク
// @param[in] imp_info 間接含意のリスト
void
RlImp::learning(const BdnMgr& network,
		ImpInfo& imp_info)
{
  ymuint n = network.max_node_id();

  imp_info.set_size(n);

  // BDN の情報を ImpMgr にコピーする．
  ImpMgr imp_mgr;
  imp_mgr.set(network);

#if 0
  for (ymuint i = 0; i < n; ++ i) {
    StrNode* node = mNodeArray[i];
    if ( node == NULL ) continue;
    cout << "Node#" << node->id() << ":";
    if ( node->is_input() ) {
      cout << "INPUT";
    }
    else if ( node->is_and() ) {
      cout << "AND";
    }
    else if ( node->is_xor() ) {
      cout << "XOR";
    }
    cout << endl;

    if ( node->is_and() || node->is_xor() ) {
      const StrEdge& e0 = node->fanin0();
      cout << "  Fanin0: " << e0.src_node()->id();
      if ( e0.src_inv() ) {
	cout << "~";
      }
      cout << endl;
      const StrEdge& e1 = node->fanin1();
      cout << "  Fanin1: " << e1.src_node()->id();
      if ( e1.src_inv() ) {
	cout << "~";
      }
      cout << endl;
    }

    cout << "  Fanouts: ";
    const vector<StrEdge*>& fo_list = node->fanout_list();
    for (vector<StrEdge*>::const_iterator p = fo_list.begin();
	 p != fo_list.end(); ++ p ) {
      StrEdge* e = *p;
      cout << " (" << e->dst_node()->id() << ", " << e->dst_pos() << ")";
    }
    cout << endl;
  }
#endif

  for (ymuint i = 0; i < n; ++ i) {
    StrNode* node = imp_mgr.node(i);
    if ( node == NULL ) continue;

    ymuint src_id = node->id();

    // node に値を割り当てる．
    for (ymuint val = 0; val < 2; ++ val) {
      vector<ImpCell> imp_list;
      bool ok = make_all_implication(imp_mgr, node, val, 1, imp_list);
      imp_info.put(src_id, val, imp_list);
    }
  }
#if 0
  cout << "DIRECT IMPLICATION" << endl;
  imp_info.print(cout);
#endif
}

// @brief recursive learning を行なう．
// @param[in] imp_mgr ImpMgr
// @param[in] node ノード
// @param[in] val 値
// @param[in] level レベル
// @param[in] imp_list 含意のリスト
bool
RlImp::make_all_implication(ImpMgr& imp_mgr,
			    StrNode* node,
			    ymuint val,
			    ymuint level,
			    vector<ImpCell>& imp_list)
{
  if ( level == 0 ) {
    imp_list.clear();
    return true;
  }
  else {
    bool ok = imp_mgr.assert(node, val, imp_list);
    if ( ok ) {
      vector<StrNode*> unode_list;
      imp_mgr.get_unodelist(unode_list);
      for (vector<StrNode*>::iterator p = unode_list.begin();
	   p != unode_list.end(); ++ p) {
	StrNode* unode = *p;

      }
    }
    else {
      // 単一の割り当てで矛盾が起こった．
      // node は !val 固定
    }
    imp_mgr.backtrack();

    return ok;
  }
}

END_NAMESPACE_YM_NETWORKS
