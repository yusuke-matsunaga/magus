
/// @file SatImp.cc
/// @brief SatImp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "SatImp.h"
#include "StrNode.h"
#include "SnInput.h"
#include "SnAnd.h"
#include "SnXor.h"
#include "ImpInfo.h"
#include "StrImp.h"
#include "ym_networks/BdnMgr.h"
#include "ym_networks/BdnNode.h"
#include "ym_logic/SatSolver.h"
#include "ym_utils/RandGen.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
// クラス SatImp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SatImp::SatImp()
{
}

// @brief デストラクタ
SatImp::~SatImp()
{
}

BEGIN_NONAMESPACE
void
check(SatSolver& solver,
      ymuint src_id,
      ymuint src_val,
      ymuint src_bv,
      ymuint dst_id,
      ymuint dst_val,
      ymuint dst_bv)
{
  ymuint val0 = (src_val == 1) ? src_bv : ~src_bv;
  ymuint val1 = (dst_val == 1) ? dst_bv : ~dst_bv;
  if ( (val0 & ~val1) != 0UL ) {
    cout << "Error(bitval): Node#" << src_id << ": " << src_val
	 << " -> Node#" << dst_id << ": " << dst_val << endl;
  }
  vector<Literal> tmp(2);
  vector<Bool3> model;
  Literal lit0(VarId(src_id), src_val ? kPolPosi : kPolNega);
  Literal lit1(VarId(dst_id), dst_val ? kPolPosi : kPolNega);
  tmp[0] = lit0;
  tmp[1] = ~lit1;
  if ( solver.solve(tmp, model) != kB3False ) {
    cout << "Error(sat): Node#" << src_id << ": " << src_val
	 << " -> Node#" << dst_id << ": " << dst_val << endl;
  }
}

END_NONAMESPACE

// @brief ネットワーク中の間接含意を求める．
// @param[in] network 対象のネットワーク
// @param[in] imp_info 間接含意のリスト
void
SatImp::learning(const BdnMgr& network,
		 ImpInfo& imp_info)
{
  RandGen rg;

  ymuint n = network.max_node_id();

  imp_info.set_size(n);

  SatSolver solver;
  for (ymuint i = 0; i < n; ++ i) {
    VarId vid = solver.new_var();
    assert_cond( vid.val() == i, __FILE__, __LINE__);
  }

  // BDN の情報を StrNode にコピーする．
  vector<StrNode*> node_array(n);

  vector<BdnNode*> node_list;
  network.sort(node_list);
  vector<ymuint> fo_count(n, 0);
  for (vector<BdnNode*>::iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    const BdnNode* bnode = *p;
    const BdnNode* bnode0 = bnode->fanin0();
    ++ fo_count[bnode0->id()];
    const BdnNode* bnode1 = bnode->fanin1();
    ++ fo_count[bnode1->id()];
  }

  const BdnNodeList& input_list = network.input_list();
  vector<StrNode*> inputs;
  inputs.reserve(input_list.size());
  for (BdnNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    const BdnNode* bnode = *p;
    ymuint id = bnode->id();
    StrNode* node = new SnInput(id);
    node_array[id] = node;
    node->set_nfo(fo_count[id]);
    inputs.push_back(node);
  }

  vector<StrNode*> nodes;
  nodes.reserve(node_list.size());
  for (vector<BdnNode*>::iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    const BdnNode* bnode = *p;
    ymuint id = bnode->id();
    Literal lit(VarId(id), kPolPosi);

    const BdnNode* bnode0 = bnode->fanin0();
    bool inv0 = bnode->fanin0_inv();
    StrNode* node0 = node_array[bnode0->id()];
    assert_cond(node0 != NULL, __FILE__, __LINE__);
    Literal lit0(VarId(node0->id()), inv0 ? kPolNega : kPolPosi);

    const BdnNode* bnode1 = bnode->fanin1();
    bool inv1 = bnode->fanin1_inv();
    StrNode* node1 = node_array[bnode1->id()];
    assert_cond(node1 != NULL, __FILE__, __LINE__);
    Literal lit1(VarId(node1->id()), inv1 ? kPolNega : kPolPosi);

    StrNode* node = NULL;
    if ( bnode->is_and() ) {
      node = new SnAnd(id, node0, inv0, node1, inv1);
      solver.add_clause(lit0, ~lit);
      solver.add_clause(lit1, ~lit);
      solver.add_clause(~lit0, ~lit1, lit);
    }
    else if ( bnode->is_xor() ) {
      node = new SnXor(id, node0, inv0, node1, inv1);
      solver.add_clause(lit0, ~lit1, lit);
      solver.add_clause(~lit0, lit1, lit);
      solver.add_clause(~lit0, ~lit1, ~lit);
      solver.add_clause(lit0, lit1, ~lit);
    }
    else {
      assert_not_reached(__FILE__, __LINE__);
    }
    node_array[id] = node;
    node->set_nfo(fo_count[id]);
    nodes.push_back(node);
  }

#if 1
  for (ymuint i = 0; i < n; ++ i) {
    StrNode* node = node_array[i];
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

  for (vector<StrNode*>::iterator p = inputs.begin();
       p != inputs.end(); ++ p) {
    StrNode* node = *p;
    ymuint64 val0 = rg.int32();
    ymuint64 val1 = rg.int32();
    ymuint64 bitval = (val0 << 32) | val1;
    node->set_bitval(bitval);
  }

  for (vector<StrNode*>::iterator p = nodes.begin();
       p != nodes.end(); ++ p) {
    StrNode* node = *p;
    node->calc_bitval();
#if 0
    cout << "Node#" << node->id()
	 << ": " << hex << node->bitval() << dec << endl;
#endif
  }

  // 直接含意を求める．
  StrImp strimp;
  ImpInfo d_imp;
  strimp.learning(network, d_imp);

  // シミュレーションでフィルタリングして残った候補を
  // SAT で調べる．
  ymuint nsat = 0;
  for (ymuint i = 0; i < n; ++ i) {
    StrNode* node0 = node_array[i];
    if ( node0 == NULL ) continue;
    Literal lit0(VarId(i), kPolPosi);
    ymuint64 val0 = node0->bitval();
    for (ymuint j = 0; j < n; ++ j) {
      if ( i == j ) continue;
      StrNode* node1 = node_array[j];
      if ( node1 == NULL ) continue;
      Literal lit1(VarId(j), kPolPosi);
      ymuint64 val1 = node1->bitval();
      // node0 が 0 の時に 0 となるノードを探す．
      if ( (~val0 & val1) == 0UL ) {
	if ( !d_imp.check(i, 0, j, 0) &&
	     !d_imp.check(j, 1, i, 1) ) {
	  vector<Literal> tmp(2);
	  vector<Bool3> model;
	  tmp[0] = ~lit0;
	  tmp[1] = lit1;
	  ++ nsat;
	  if ( solver.solve(tmp, model) == kB3False ) {
	    imp_info.put(i, 0, j, 0);
	  }
	}
      }
      // node0 が 0 の時に 1 となるノードを探す．
      else if ( (~val0 & ~val1) == 0UL ) {
	if ( !d_imp.check(i, 0, j, 1) &&
	     !d_imp.check(j, 0, i, 1) ) {
	  vector<Literal> tmp(2);
	  vector<Bool3> model;
	  tmp[0] = ~lit0;
	  tmp[1] = ~lit1;
	  ++ nsat;
	  if ( solver.solve(tmp, model) == kB3False ) {
	    imp_info.put(i, 0, j, 1);
	  }
	}
      }
      // node0 が 1 の時に 0 となるノードを探す．
      if ( (val0 & val1) == 0UL ) {
	if ( !d_imp.check(i, 1, j, 0) &&
	     !d_imp.check(j, 1, i, 0) ) {
	  vector<Literal> tmp(2);
	  vector<Bool3> model;
	  tmp[0] = lit0;
	  tmp[1] = lit1;
	  ++ nsat;
	  if ( solver.solve(tmp, model) == kB3False ) {
	    imp_info.put(i, 1, j, 0);
	  }
	}
      }
      // node0 が 1 の時に 1 となるノードを探す．
      else if ( (val0 & ~val1) == 0UL ) {
	if ( !d_imp.check(i, 1, j, 1) &&
	     !d_imp.check(j, 0, i, 0) ) {
	  vector<Literal> tmp(2);
	  vector<Bool3> model;
	  tmp[0] = lit0;
	  tmp[1] = ~lit1;
	  ++ nsat;
	  if ( solver.solve(tmp, model) == kB3False ) {
	    imp_info.put(i, 1, j, 1);
	  }
	}
      }
    }
  }
  cout << "nsat = " << nsat << endl;
}

END_NAMESPACE_YM_NETWORKS
