
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
make_cnf(SatSolver& solver,
	 ImpNode* node,
	 vector<bool>& mark)
{
  if ( mark[node->id()] ) {
    return;
  }
  mark[node->id()] = true;

  if ( node->is_and() ) {
    VarId vid(node->id());
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
    if ( node->is_and() ) {
      solver.add_clause(lit0, ~lit);
      solver.add_clause(lit1, ~lit);
      solver.add_clause(~lit0, ~lit1, lit);
    }

    make_cnf(solver, node0, mark);
    make_cnf(solver, node1, mark);
  }
}

Bool3
justify(ImpMgr& imp_mgr,
	ymuint depth)
{
  vector<ImpNode*> unode_list;
  imp_mgr.get_unodelist(unode_list);
  if ( unode_list.empty() ) {
    return kB3True;
  }

  ImpNode* unode0 = unode_list[0];
  ymuint np = unode0->justification_num();
#if 0
  bool has_x = false;
  bool sat = false;
  for (ymuint i = 0; i < np && !sat; ++ i) {
    ImpDst imp = unode0->get_justification(i);
    ImpNode* node = imp.node();
    ymuint val = imp.val();
    vector<ImpDst> imp_list;
    bool stat1 = imp_mgr.assert(node, val, imp_list);
    if ( stat1 ) {
      if ( depth > 0 ) {
	Bool3 stat2 = justify(imp_mgr, depth - 1);
	if ( stat2 == kB3True ) {
	  sat = true;
	}
	if ( stat2 == kB3X ) {
	  has_x = true;
	}
      }
      else {
	has_x = true;
      }
    }
    imp_mgr.backtrack();
  }
  if ( has_x ) {
    return kB3X;
  }
  else {
    return kB3False;
  }
#else
  bool sat = false;
  for (ymuint i = 0; i < np && !sat; ++ i) {
    ImpDst imp = unode0->get_justification(i);
    ImpNode* node = imp.node();
    ymuint val = imp.val();
    vector<ImpDst> imp_list;
    bool stat1 = imp_mgr.assert(node, val, imp_list);
    if ( stat1 ) {
      if ( depth > 0 ) {
	Bool3 stat2 = justify(imp_mgr, depth - 1);
	if ( stat2 == kB3True ) {
	  sat = true;
	}
      }
    }
    imp_mgr.backtrack();
    if ( stat1 ) {
      break;
    }
  }
  if ( sat ) {
    return kB3True;
  }
  else {
    return kB3X;
  }
#endif
}

Bool3
str_sat(ImpMgr& imp_mgr,
	ImpNode* node1,
	ymuint val1,
	ImpNode* node2,
	ymuint val2)
{
  vector<ImpDst> imp_list;
  bool stat1 = imp_mgr.assert(node1, val1, imp_list);
  if ( stat1 == false ) {
    imp_mgr.backtrack();
    return kB3False;
  }
  bool stat2 = imp_mgr.assert(node2, val2, imp_list);
  if ( stat2 == false ) {
    imp_mgr.backtrack();
    imp_mgr.backtrack();
    return kB3False;
  }

  Bool3 stat3 = justify(imp_mgr, 100);

  imp_mgr.backtrack();
  imp_mgr.backtrack();

  return stat3;
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
  StopWatch timer;
  timer.start();

  ymuint n = imp_mgr.node_num();

  imp_info.set_size(n);

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
  // 定数ノードの情報をコピーしておく．
  imp_info.copy_const(d_imp);

  // シミュレーションでフィルタリングして残った候補を
  // SAT で調べる．
  imp_mgr.random_sim();
  vector<list<ImpDst> > cand_info(n * 2);
  for (ymuint i = 1; i < n; ++ i) {
    if ( imp_info.is_const0(i) || imp_info.is_const1(i) ) {
      continue;
    }
    if ( debug ) {
      if ( (i % 100) == 0 ) {
	cerr << i << " / " << n << endl;
      }
    }

    ImpNode* node0 = imp_mgr.node(i);

    ymuint64 val0 = node0->bitval();

    for (ymuint j = 0; j < i; ++ j) {
      if ( imp_info.is_const0(j) || imp_info.is_const1(j) ) {
	continue;
      }

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

  ymuint remain = count_list(cand_info);
  count = 1;
  ymuint count_solve = 0;
  ymuint count_sat = 0;
  ymuint count_unsat = 0;
  ymuint count_abort = 0;
  for (ymuint src_id = 0; src_id < n; ++ src_id) {
    if ( imp_info.is_const0(src_id) || imp_info.is_const1(src_id) ) {
      continue;
    }

    if ( cand_info[src_id * 2 + 0].empty() &&
	 cand_info[src_id * 2 + 1].empty() ) {
      continue;
    }

    ImpNode* node = imp_mgr.node(src_id);

    SatSolver solver;
    vector<bool> cnf_mark(n, false);
    for (ymuint id = 0; id < n; ++ id) {
      VarId vid = solver.new_var();
      assert_cond( vid.val() == id, __FILE__, __LINE__);
      if ( imp_info.is_const0(id) ) {
	Literal lit(vid, kPolNega);
	solver.add_clause(lit);
      }
      else if ( imp_info.is_const1(id) ) {
	Literal lit(vid, kPolPosi);
	solver.add_clause(lit);
      }
    }
    make_cnf(solver, node, cnf_mark);

    for (ymuint src_val = 0; src_val < 2; ++ src_val) {
      Literal lit0(VarId(src_id), src_val == 0 ? kPolNega : kPolPosi);
      const list<ImpDst>& imp_list = cand_info[src_id * 2 + src_val];
      for (list<ImpDst>::const_iterator p = imp_list.begin();
	   p != imp_list.end(); ++ p) {
	if ( debug ) {
	  if ( (count % 1000) == 0 ) {
	    cerr << "sat#" << count << " / " << remain;
	    cerr << ": ";
	    imp_info.print_stats(cerr);
	  }
	}
	++ count;
	ImpNode* node1 = p->node();
	ymuint dst_id = node1->id();
	ymuint dst_val = p->val();
	Literal lit1(VarId(dst_id), dst_val == 0 ? kPolNega : kPolPosi);

	if ( imp_info.is_const0(dst_id) || imp_info.is_const1(dst_id) ) {
	  continue;
	}

	if ( imp_info.check(src_id, src_val, dst_id, dst_val) ) {
	  continue;
	}

	make_cnf(solver, node1, cnf_mark);

	vector<Literal> tmp(2);
	vector<Bool3> model;
	tmp[0] = lit0;
	tmp[1] = ~lit1;
	++ count_solve;
	Bool3 stat = solver.solve(tmp, model);
	if ( stat == kB3False ) {
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
	else if ( stat == kB3True ) {
	  ++ count_sat;
	}
	else {
	  ++ count_abort;
	}
      }
    }
  }
  timer.stop();
  USTime sat_time = timer.time();

  cout << "SAT statistics" << endl
       << " " << setw(10) << count_unsat << " + " << setw(10) << count_sat
       << " + " << count_abort
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
