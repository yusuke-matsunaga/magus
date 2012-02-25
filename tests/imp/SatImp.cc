
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

BEGIN_NONAMESPACE

bool debug = true;

END_NONAMESPACE

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

ymuint
count_list(const vector<list<ImpVal> >& cand_info)
{
  ymuint c = 0;
  for (vector<list<ImpVal> >::const_iterator p = cand_info.begin();
       p != cand_info.end(); ++ p) {
    const list<ImpVal>& imp_list = *p;
    c += imp_list.size();
  }
  return c;
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

  if ( debug ) {
    cout << "CNF generated" << endl;
  }

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
  vector<list<ImpVal> > cand_info(n * 2);
  for (ymuint i = 0; i < n; ++ i) {
    if ( debug ) {
      if ( (i % 100) == 0 ) {
	cout << i << " / " << n << endl;
      }
    }
    StrNode* node0 = node_array[i];
    if ( node0 == NULL ) continue;
    ymuint64 val0 = node0->bitval();
    for (ymuint j = 0; j < n; ++ j) {
      if ( i == j ) continue;
      StrNode* node1 = node_array[j];
      if ( node1 == NULL ) continue;
      ymuint64 val1 = node1->bitval();
      if ( ~val0 != 0UL ) {
	// node0 が 0 の時に 0 となるノードを探す．
	if ( (~val0 & val1) == 0UL ) {
	  if ( !d_imp.check(i, 0, j, 0) &&
	       !d_imp.check(j, 1, i, 1) ) {
	    cand_info[i * 2 + 0].push_back(ImpVal(j, 0));
	  }
	}
	// node0 が 0 の時に 1 となるノードを探す．
	else if ( (~val0 & ~val1) == 0UL ) {
	  if ( !d_imp.check(i, 0, j, 1) &&
	       !d_imp.check(j, 0, i, 1) ) {
	    cand_info[i * 2 + 0].push_back(ImpVal(j, 1));
	  }
	}
      }
      if ( val0 != 0UL ) {
	// node0 が 1 の時に 0 となるノードを探す．
	if ( (val0 & val1) == 0UL ) {
	  if ( !d_imp.check(i, 1, j, 0) &&
	       !d_imp.check(j, 1, i, 0) ) {
	    cand_info[i * 2 + 1].push_back(ImpVal(j, 0));
	  }
	}
	// node0 が 1 の時に 1 となるノードを探す．
	else if ( (val0 & ~val1) == 0UL ) {
	  if ( !d_imp.check(i, 1, j, 1) &&
	       !d_imp.check(j, 0, i, 0) ) {
	    cand_info[i * 2 + 1].push_back(ImpVal(j, 1));
	  }
	}
      }
    }
  }

  ymuint prev_size = count_list(cand_info);
  if ( debug ) {
    cout << "nsat0 = " << prev_size << endl;
  }
  ymuint count = 1;
  ymuint nochg = 0;
  for ( ; ; ) {
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
    }

    for (ymuint src_id = 0; src_id < n; ++ src_id) {
      StrNode* node0 = node_array[src_id];
      if ( node0 == NULL ) continue;
      for (ymuint src_val = 0; src_val < 2; ++ src_val) {
	ymuint64 val0 = node0->bitval();
	if ( src_val == 0 ) {
	  val0 = ~val0;
	}
	list<ImpVal>& imp_list = cand_info[src_id * 2 + src_val];
	for (list<ImpVal>::iterator p = imp_list.begin();
	     p != imp_list.end(); ) {
	  const ImpVal& imp = *p;
	  ymuint dst_id = imp.id();
	  StrNode* node1 = node_array[dst_id];
	  assert_cond( node1 != NULL, __FILE__, __LINE__);
	  ymuint dst_val = imp.val();
	  ymuint64 val1 = node1->bitval();
	  if ( dst_val == 0 ) {
	    val1 = ~val1;
	  }
	  if ( (val0 & ~val1) != 0UL ) {
	    list<ImpVal>::iterator q = p;
	    ++ p;
	    imp_list.erase(q);
	  }
	  else {
	    ++ p;
	  }
	}
      }
    }
    ymuint cur_size = count_list(cand_info);
    ymuint diff = prev_size - cur_size;
    prev_size = cur_size;

    if ( debug ) {
      if ( (count % 100) == 0 ) {
	cout << "nsat" << count << " = " << cur_size << endl;
      }
    }
    ++ count;

    if ( diff < 10 ) {
      ++ nochg;
      if ( nochg >= 10 ) {
	break;
      }
    }
    else {
      nochg = 0;
    }
  }

  if ( debug ) {
    cout << "random simulation end." << endl;
  }

  ymuint remain = count_list(cand_info);
  count = 1;
  for (ymuint src_id = 0; src_id < n; ++ src_id) {
    StrNode* node0 = node_array[src_id];
    if ( node0 == NULL ) continue;
    for (ymuint src_val = 0; src_val < 2; ++ src_val) {
      Literal lit0(VarId(src_id), src_val == 0 ? kPolNega : kPolPosi);
      const list<ImpVal>& imp_list = cand_info[src_id * 2 + src_val];
      for (list<ImpVal>::const_iterator p = imp_list.begin();
	   p != imp_list.end(); ++ p) {
	if ( debug ) {
	  cout << "sat#" << count << " / " << remain << endl;
	}
	++ count;
	const ImpVal& imp = *p;
	ymuint dst_id = imp.id();
	ymuint dst_val = imp.val();
	Literal lit1(VarId(dst_id), dst_val == 0 ? kPolNega : kPolPosi);
	vector<Literal> tmp(2);
	vector<Bool3> model;
	tmp[0] = lit0;
	tmp[1] = ~lit1;
	if ( solver.solve(tmp, model) == kB3False ) {
	  imp_info.put(src_id, src_val, dst_id, dst_val);
	}
      }
    }
  }
}

END_NAMESPACE_YM_NETWORKS
