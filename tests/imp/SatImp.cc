
/// @file SatImp.cc
/// @brief SatImp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "SatImp.h"
#include "ImpNode.h"
#include "ImpInfo.h"
#include "ImpList.h"
#include "ImpMgr.h"
#include "StrImp.h"
#include "ym_logic/SatSolver.h"
#include "ym_utils/MFSet.h"
#include "ym_utils/RandGen.h"
#include "ym_utils/StopWatch.h"


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
    cerr << "Error(bitval): Node#" << src_id << ": " << src_val
	 << " -> Node#" << dst_id << ": " << dst_val << endl;
  }
  vector<Literal> tmp(2);
  vector<Bool3> model;
  Literal lit0(VarId(src_id), src_val ? kPolPosi : kPolNega);
  Literal lit1(VarId(dst_id), dst_val ? kPolPosi : kPolNega);
  tmp[0] = lit0;
  tmp[1] = ~lit1;
  if ( solver.solve(tmp, model) != kB3False ) {
    cerr << "Error(sat): Node#" << src_id << ": " << src_val
	 << " -> Node#" << dst_id << ": " << dst_val << endl;
  }
}

ymuint
count_list(const vector<list<ImpDst> >& cand_info)
{
  ymuint c = 0;
  for (vector<list<ImpDst> >::const_iterator p = cand_info.begin();
       p != cand_info.end(); ++ p) {
    const list<ImpDst>& imp_list = *p;
    c += imp_list.size();
  }
  return c;
}

void
mark_tfi(ImpNode* node,
	 vector<bool>& mark,
	 vector<ymuint>& input_list)
{
  if ( mark[node->id()] ) {
    return;
  }
  mark[node->id()] = true;

  if ( node->is_input() ) {
    input_list.push_back(node->id());
  }
  else {
    mark_tfi(node->fanin0().src_node(), mark, input_list);
    mark_tfi(node->fanin1().src_node(), mark, input_list);
  }
}

bool
check_intersect(const vector<ymuint>& list1,
		const vector<ymuint>& list2)
{
  ymuint n1 = list1.size();
  ymuint n2 = list2.size();
  ymuint i1 = 0;
  ymuint i2 = 0;
  while ( i1 < n1 && i2 < n2 ) {
    if ( list1[i1] < list2[i2] ) {
      ++ i1;
    }
    else if ( list1[i1] > list2[i2] ) {
      ++ i2;
    }
    else {
      return true;
    }
  }
  return false;
}

void
print_fanin(ostream& s,
	    const ImpEdge& e)
{
  if ( e.src_inv() ) {
    s << "~";
  }
  s << e.src_node()->id();
}

inline
Literal
to_literal(ymuint id,
	   ymuint val)
{
  return Literal(VarId(id), (val == 0) ? kPolNega : kPolPosi);
}

struct NodeInfo
{
  NodeInfo()
  {
  }

  NodeInfo(ImpNode* node,
	   bool inv) :
    mNode(node),
    mInv(inv)
  {
  }

  ImpNode* mNode;
  bool mInv;
};

END_NONAMESPACE


// @brief ネットワーク中の間接含意を求める．
// @param[in] imp_mgr マネージャ
// @param[in] imp_info 間接含意のリスト
void
SatImp::learning(ImpMgr& imp_mgr,
		 const ImpInfo& d_imp,
		 ImpInfo& imp_info)
{
  ymuint n = imp_mgr.node_num();

  imp_info.set_size(n);

  if ( debug ) {
    cerr << "CNF generated" << endl;
  }

  StopWatch timer;
  timer.start();

  // 各ノードから到達可能な入力ノードのリストを求める．
  vector<vector<ymuint> > input_list_array(n);
  for (ymuint i = 0; i < n; ++ i) {
    ImpNode* node0 = imp_mgr.node(i);

    vector<bool> tfi_mark(n, false);
    mark_tfi(node0, tfi_mark, input_list_array[i]);
    sort(input_list_array[i].begin(), input_list_array[i].end());
  }

  // 直接含意と対偶の含意をコピーしておく
  for (ymuint src_id = 0; src_id < n; ++ src_id) {
    for (ymuint src_val = 0; src_val < 2; ++ src_val) {
      const ImpList& imp_list = d_imp.get(src_id, src_val);
      for (ImpList::iterator p = imp_list.begin(); p != imp_list.end(); ++ p) {
	const ImpCell& cell = *p;
	ymuint dst_id = cell.dst_id();
	ymuint dst_val = cell.dst_val();
	imp_info.put(src_id, src_val, dst_id, dst_val);
	if ( !d_imp.check(dst_id, dst_val ^ 1, src_id, src_val ^ 1) ) {
	  imp_info.put(dst_id, dst_val ^ 1, src_id, src_val ^ 1);
	}
      }
    }
  }

  // シミュレーションでフィルタリングして残った候補を
  // SAT で調べる．
  imp_mgr.random_sim();
  vector<list<ImpDst> > cand_info(n * 2);
  vector<ymuint32> const_flag(n, 0U);
  hash_map<ymuint64, list<NodeInfo> > node_hash;
  for (ymuint i = 1; i < n; ++ i) {
    if ( debug ) {
      if ( (i % 100) == 0 ) {
	cerr << i << " / " << n << endl;
      }
    }

    ImpNode* node0 = imp_mgr.node(i);

    ymuint64 val0 = node0->bitval();

    if ( val0 != ~0UL ) {
      const_flag[i] |= 1UL;
    }
    if ( val0 != 0UL ) {
      const_flag[i] |= 2UL;
    }
    if ( val0 != 0UL && val0 != ~0UL ) {
      bool inv = false;
      ymuint64 c_val0 = val0;
      if ( c_val0 & 1UL ) {
	c_val0 = ~c_val0;
	inv = true;
      }
      hash_map<ymuint64, list<NodeInfo> >::iterator p = node_hash.find(c_val0);
      if ( p == node_hash.end() ) {
	node_hash.insert(make_pair(c_val0, list<NodeInfo>()));
	p = node_hash.find(c_val0);
	assert_cond( p != node_hash.end(), __FILE__, __LINE__);
      }
      list<NodeInfo>& node_list = p->second;
      node_list.push_back(NodeInfo(node0, inv));
    }

    for (ymuint j = 0; j < i; ++ j) {
      ImpNode* node1 = imp_mgr.node(j);

      if ( !check_intersect(input_list_array[i], input_list_array[j]) ) {
	continue;
      }
      ymuint64 val1 = node1->bitval();

      // node0 が 0 の時に 0 となるノードを探す．
      if ( (~val0 & val1) == 0UL ) {
	if ( !imp_info.check(i, 0, j, 0) ) {
	  cand_info[i * 2 + 0].push_back(ImpDst(node1, 0));
	}
      }
      // node0 が 0 の時に 1 となるノードを探す．
      if ( (~val0 & ~val1) == 0UL ) {
	if ( !imp_info.check(i, 0, j, 1) ) {
	  cand_info[i * 2 + 0].push_back(ImpDst(node1, 1));
	}
      }
      // node0 が 1 の時に 0 となるノードを探す．
      if ( (val0 & val1) == 0UL ) {
	if ( !imp_info.check(i, 1, j, 0) ) {
	  cand_info[i * 2 + 1].push_back(ImpDst(node1, 0));
	}
      }
      // node0 が 1 の時に 1 となるノードを探す．
      if ( (val0 & ~val1) == 0UL ) {
	if ( !imp_info.check(i, 1, j, 1) ) {
	  cand_info[i * 2 + 1].push_back(ImpDst(node1, 1));
	}
      }
    }
  }

#if 1
  {
    // ImpMgr から CNF を作る．
    SatSolver solver;

    for (ymuint i = 0; i < n; ++ i) {
      VarId vid = solver.new_var();
      assert_cond( vid.val() == i, __FILE__, __LINE__);
    }

    for (ymuint i = 0; i < n; ++ i) {
      ImpNode* node = imp_mgr.node(i);
      if ( node->is_input() ) continue;

      VarId vid(i);
      Literal lit(vid, kPolPosi);

      const ImpEdge& e0 = node->fanin0();
      ImpNode* node0 = e0.src_node();
      VarId vid0(node0->id());
      bool inv0 = e0.src_inv();
      Literal lit0(vid0, inv0 ? kPolNega : kPolPosi);

      const ImpEdge& e1 = node->fanin1();
      ImpNode* node1 = e1.src_node();
      VarId vid1(node1->id());
      bool inv1 = e1.src_inv();
      Literal lit1(vid1, inv1 ? kPolNega : kPolPosi);

      solver.add_clause(lit0, ~lit);
      solver.add_clause(lit1, ~lit);
      solver.add_clause(~lit0, ~lit1, lit);
    }

    list<ymuint> const_list;
    for (ymuint i = 0; i < n; ++ i) {
      if ( (const_flag[i] & 1U) == 0U ) {
	// 0 になったことがない．
	VarId vid(i);
	Literal lit(vid, kPolNega);
	vector<Literal> tmp(1, lit);
	vector<Bool3> model;
	if ( solver.solve(tmp, model) == kB3False ) {
	  // node0 は 1 固定
	  solver.add_clause(~lit);
#if 1
	  cout << "Node#" << i << " is const-1" << endl;
#endif
	  imp_info.set_1(i);
	  const_list.push_back(i);
	}
      }
      else if ( (const_flag[i] & 2U) == 0U ) {
	// 1 になったことがない．
	VarId vid(i);
	Literal lit(vid, kPolPosi);
	vector<Literal> tmp(1, lit);
	vector<Bool3> model;
	if ( solver.solve(tmp, model) == kB3False ) {
	  // node0 は 0 固定
	  solver.add_clause(~lit);
#if 1
	  cout << "Node#" << i << " is const-0" << endl;
#endif
	  imp_info.set_0(i);
	  const_list.push_back(i);
	}
      }
    }
    for (list<ymuint>::const_iterator p = const_list.begin();
	 p != const_list.end(); ++ p) {
      ymuint id = *p;
      cand_info[id * 2 + 0].clear();
      cand_info[id * 2 + 1].clear();
    }
    for (ymuint src_id = 0; src_id < n; ++ src_id) {
      for (ymuint src_val = 0; src_val < 2; ++ src_val) {
	list<ImpDst>& imp_list = cand_info[src_id * 2 + src_val];
	for (list<ImpDst>::iterator p = imp_list.begin(); p != imp_list.end(); ) {
	  ImpNode* dst_node = p->node();
	  ymuint dst_id = dst_node->id();
	  if ( imp_info.is_const0(dst_id) || imp_info.is_const1(dst_id) ) {
	    list<ImpDst>::iterator q = p;
	    ++ p;
	    imp_list.erase(q);
	  }
	  else {
	    ++ p;
	  }
	}
      }
    }
  }
#endif
#if 0
  for (hash_map<ymuint64, list<NodeInfo> >::iterator p = node_hash.begin();
       p != node_hash.end(); ++ p) {
    if ( p->first == 0ULL || p->first == ~0ULL ) continue;

    const list<NodeInfo>& node_list = p->second;
    ymuint nc = node_list.size();
    if ( nc == 1 ) continue;
    vector<NodeInfo> tmp_array(n);
    {
      ymuint i = 0;
      for (list<NodeInfo>::const_iterator q = node_list.begin();
	   q != node_list.end(); ++ q, ++ i) {
	tmp_array[i] = *q;
      }
    }
    for (ymuint i = 0; i < nc; ++ i) {
      const NodeInfo& node_info1 = tmp_array[i];
      ImpNode* node1 = node_info1.mNode;
      Literal lit1(VarId(node1->id()), node_info1.mInv ? kPolNega : kPolPosi);
      for (ymuint j = i + 1; j < nc; ++ j) {
	const NodeInfo& node_info2 = tmp_array[j];
	ImpNode* node2 = node_info2.mNode;
	Literal lit2(VarId(node2->id()), node_info2.mInv ? kPolNega : kPolPosi);
	vector<Literal> tmp(2);
	tmp[0] = lit1;
	tmp[1] = ~lit2;
	vector<Bool3> model;
	if ( solver.solve(tmp, model) != kB3False ) {
	  continue;
	}
	tmp[0] = ~lit1;
	tmp[1] = lit2;
	if ( solver.solve(tmp, model) != kB3False ) {
	  continue;
	}
	cout << "Node#" << node1->id()
	     << " and Node#" << node2->id()
	     << " are equivalent" << endl;
	solver.add_clause(lit1, ~lit2);
	solver.add_clause(~lit2, lit1);
#if 0
	ymuint val2 = node_info1.mInv ^ node_info2.mInv ? 0 : 1;
	imp_info.put(node1->id(), 0, node2->id(), val2 ^ 1);
	imp_info.put(node1->id(), 1, node2->id(), val2);
#endif
      }
    }
  }
#endif

  ymuint prev_size = count_list(cand_info);
  if ( debug ) {
    cerr << "sim#0: " << prev_size << endl;
  }
  ymuint count = 1;
  ymuint nochg = 0;

  for ( ; ; ) {
    imp_mgr.random_sim();

    for (ymuint src_id = 0; src_id < n; ++ src_id) {
      ImpNode* node0 = imp_mgr.node(src_id);

      ymuint64 orig_val0 = node0->bitval();

      for (ymuint src_val = 0; src_val < 2; ++ src_val) {
	ymuint64 val0 = orig_val0;
	if ( src_val == 0 ) {
	  val0 = ~val0;
	}
	list<ImpDst>& imp_list = cand_info[src_id * 2 + src_val];
	for (list<ImpDst>::iterator p = imp_list.begin();
	     p != imp_list.end(); ) {
	  ImpNode* node1 = p->node();
	  ymuint dst_val = p->val();
	  ymuint64 val1 = node1->bitval();
	  if ( dst_val == 0 ) {
	    val1 = ~val1;
	  }
	  if ( (val0 & ~val1) != 0UL ) {
	    list<ImpDst>::iterator q = p;
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
	cerr << "sim#" << count << ": " << cur_size << endl;
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
    cerr << "random simulation end." << endl;
  }

  timer.stop();
  USTime pre_time = timer.time();

  timer.reset();
  timer.start();

  SatSolver solver;
  for (ymuint id = 0; id < n; ++ id) {
    VarId vid = solver.new_var();
    assert_cond( vid.val() == id, __FILE__, __LINE__);
#if 0
    ImpNodeHandle handle = imp_mgr.bnode_handle(id);
    if ( handle.is_const() ) continue;
    ImpNode* node = handle.node();

    if ( node->is_and() ) {
      Literal lit(VarId(id), kPolPosi);

      const ImpEdge& e0 = node->fanin0();
      ImpNode* node0 = e0.src_node();
      bool inv0 = e0.src_inv();
      Literal lit0(VarId(node0->id()), inv0 ? kPolNega : kPolPosi);

      const ImpEdge& e1 = node->fanin1();
      ImpNode* node1 = e1.src_node();
      bool inv1 = e1.src_inv();
      Literal lit1(VarId(node1->id()), inv1 ? kPolNega : kPolPosi);

      solver.add_clause(lit0, ~lit);
      solver.add_clause(lit1, ~lit);
      solver.add_clause(~lit0, ~lit1, lit);
    }
#endif
  }

  ymuint remain = count_list(cand_info);
  count = 1;
  ymuint count_solve = 0;
  ymuint count_sat = 0;
  ymuint count_unsat = 0;
  for (ymuint src_id = 0; src_id < n; ++ src_id) {
    if ( imp_info.is_const0(src_id) || imp_info.is_const1(src_id) ) {
      continue;
    }

    ImpNode* node = imp_mgr.node(src_id);

#if 1
    if ( node->is_and() ) {
      Literal lit(VarId(src_id), kPolPosi);

      const ImpEdge& e0 = node->fanin0();
      ImpNode* node0 = e0.src_node();
      bool inv0 = e0.src_inv();
      Literal lit0(VarId(node0->id()), inv0 ? kPolNega : kPolPosi);

      const ImpEdge& e1 = node->fanin1();
      ImpNode* node1 = e1.src_node();
      bool inv1 = e1.src_inv();
      Literal lit1(VarId(node1->id()), inv1 ? kPolNega : kPolPosi);

      solver.add_clause(lit0, ~lit);
      solver.add_clause(lit1, ~lit);
      solver.add_clause(~lit0, ~lit1, lit);
    }
#endif

    for (ymuint src_val = 0; src_val < 2; ++ src_val) {
      Literal lit0(VarId(src_id), src_val == 0 ? kPolNega : kPolPosi);
      const list<ImpDst>& imp_list = cand_info[src_id * 2 + src_val];
      for (list<ImpDst>::const_iterator p = imp_list.begin();
	   p != imp_list.end(); ++ p) {
	if ( debug ) {
	  if ( (count % 1000) == 0 ) {
	    cerr << "sat#" << count << " / " << remain << endl;
	  }
	}
	++ count;
	ImpNode* node1 = p->node();
	ymuint dst_id = node1->id();
	ymuint dst_val = p->val();

	if ( imp_info.is_const0(dst_id) || imp_info.is_const1(dst_id) ) {
	  continue;
	}

	if ( imp_info.check(src_id, src_val, dst_id, dst_val) ) {
	  continue;
	}

	Literal lit1(VarId(dst_id), dst_val == 0 ? kPolNega : kPolPosi);
	vector<Literal> tmp(2);
	vector<Bool3> model;
	tmp[0] = lit0;
	tmp[1] = ~lit1;
	++ count_solve;
	if ( solver.solve(tmp, model) == kB3False ) {
	  // 含意が証明された．
	  ++ count_unsat;

	  imp_info.put(src_id, src_val, dst_id, dst_val);
	  imp_info.put(dst_id, dst_val ^ 1, src_id, src_val ^ 1);
	  solver.add_clause(~lit0, lit1);

	  // src_id:src_val ==> dst_id:dst_val と
	  // dst_id:dst_val から導かれる含意を合成する．
	  const ImpList& imp_list1 = imp_info.get(dst_id, dst_val);
	  for (ImpList::iterator p1 = imp_list1.begin();
	       p1 != imp_list1.end(); ++ p1) {
	    const ImpCell& imp = *p1;
	    ymuint dst_id1 = imp.dst_id();
	    ymuint dst_val1 = imp.dst_val();

	    if ( dst_id1 == src_id ) continue;

	    if ( imp_info.is_const0(dst_id1) || imp_info.is_const1(dst_id1) ) {
	      continue;
	    }

	    if ( !imp_info.check(src_id, src_val, dst_id1, dst_val1) ) {
	      imp_info.put(src_id, src_val, dst_id1, dst_val1);
	      imp_info.put(dst_id1, dst_val1 ^ 1, src_id, src_val ^ 1);
	    }
	  }

	  // dst_id:~dst_val ==> src_id:~src_val と
	  // src_id:~src_val から導かれる含意を合成する．
	  const ImpList& imp_list2 = imp_info.get(src_id, src_val ^ 1);
	  for (ImpList::iterator p2 = imp_list2.begin();
	       p2 != imp_list2.end(); ++ p2) {
	    const ImpCell& imp = *p2;
	    ymuint dst_id2 = imp.dst_id();
	    ymuint dst_val2 = imp.dst_val();

	    if ( dst_id2 == dst_id ) continue;

	    if ( imp_info.is_const0(dst_id2) || imp_info.is_const1(dst_id2) ) {
	      continue;
	    }

	    if ( !imp_info.check(dst_id, dst_val ^ 1, dst_id2, dst_val2) ) {
	      imp_info.put(dst_id, dst_val ^ 1, dst_id2, dst_val2);
	      imp_info.put(dst_id2, dst_val2 ^ 1, dst_id, dst_val);
	    }
	  }
	}
	else {
	  ++ count_sat;
	}
      }
    }
  }
  timer.stop();
  USTime sat_time = timer.time();

  cout << "SAT statistics" << endl
       << " " << setw(10) << count_unsat << " + " << setw(10) << count_sat
       << " / " << count_solve << endl
       << "  simulation: " << pre_time << endl
       << "  SAT:        " << sat_time << endl;

#if 0
  // 検証
  if ( 1 ) {
    SatSolver solver1;
    for (ymuint id = 0; id < n; ++ id) {
      VarId vid = solver1.new_var();
      assert_cond( vid.val() == id, __FILE__, __LINE__);
    }

    // ImpMgr から CNF を作る．
    for (ymuint id = 0; id < n; ++ id) {
      ImpNode* node = imp_mgr.node(id);
      if ( node == NULL ) continue;
      if ( node->is_input() ) continue;

      Literal lit(VarId(id), kPolPosi);

      const ImpEdge& e0 = node->fanin0();
      ImpNode* node0 = e0.src_node();
      bool inv0 = e0.src_inv();
      Literal lit0(VarId(node0->id()), inv0 ? kPolNega : kPolPosi);

      const ImpEdge& e1 = node->fanin1();
      ImpNode* node1 = e1.src_node();
      bool inv1 = e1.src_inv();
      Literal lit1(VarId(node1->id()), inv1 ? kPolNega : kPolPosi);

      if ( node->is_and() ) {
	solver1.add_clause(lit0, ~lit);
	solver1.add_clause(lit1, ~lit);
	solver1.add_clause(~lit0, ~lit1, lit);
      }
      else if ( node->is_xor() ) {
	solver1.add_clause(lit0, ~lit1, lit);
	solver1.add_clause(~lit0, lit1, lit);
	solver1.add_clause(~lit0, ~lit1, ~lit);
	solver1.add_clause(lit0, lit1, ~lit);
      }
      else {
	assert_not_reached(__FILE__, __LINE__);
      }
    }

    ymuint nerr = 0;
    for (ymuint src_id = 0; src_id < n; ++ src_id) {
      for (ymuint src_val = 0; src_val < 2; ++ src_val) {
	Literal lit0(to_literal(src_id, src_val));
	const ImpList& imp_list = imp_info.get(src_id, src_val);
	for (ImpList::iterator p = imp_list.begin(); p != imp_list.end(); ++ p) {
	  const ImpCell& imp = *p;
	  ymuint dst_id = imp.dst_id();
	  ymuint dst_val = imp.dst_val();
	  assert_cond( imp_info.check(src_id, src_val, dst_id, dst_val),
		       __FILE__, __LINE__);
	  Literal lit1(to_literal(dst_id, dst_val));
	  vector<Literal> tmp(2);
	  tmp[0] = lit0;
	  tmp[1] = ~lit1;
	  vector<Bool3> model;
	  if ( solver1.solve(tmp, model) != kB3False ) {
	    cout << "ERROR: Node#" << src_id << ": " << src_val
		 << " ==> Node#" << dst_id << ": " << dst_val << endl;
	    ++ nerr;
	  }
	}
      }
    }
    cout << "Total " << nerr << " errors" << endl;
  }
#endif
}

END_NAMESPACE_YM_NETWORKS
