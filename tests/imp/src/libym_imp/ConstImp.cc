
/// @file ConstImp.cc
/// @brief ConstImp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ConstImp.h"
#include "ImpNode.h"
#include "ImpInfo.h"
#include "ImpMgr.h"
#include "StrImp.h"
#include "ym_logic/SatSolver.h"
#include "ym_utils/RandGen.h"
#include "ym_utils/StopWatch.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
// クラス ConstImp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ConstImp::ConstImp()
{
}

// @brief デストラクタ
ConstImp::~ConstImp()
{
}

BEGIN_NONAMESPACE

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
void
ConstImp::learning(ImpMgr& imp_mgr)
{
  StopWatch timer;
  timer.start();

  ymuint n = imp_mgr.node_num();

  // 各ノードから到達可能な入力ノードのリストを求める．
  vector<vector<ymuint> > input_list_array(n);
  for (ymuint i = 0; i < n; ++ i) {
    ImpNode* node0 = imp_mgr.node(i);

    vector<bool> tfi_mark(n, false);
    mark_tfi(node0, tfi_mark, input_list_array[i]);
    sort(input_list_array[i].begin(), input_list_array[i].end());
  }

  // シミュレーションでフィルタリングして残った候補を
  // SAT で調べる．
  imp_mgr.random_sim();
  vector<list<ImpDst> > cand_info(n * 2);
  vector<ymuint32> const_flag(n, 0U);
  for (ymuint i = 1; i < n; ++ i) {
    if ( imp_mgr.is_const(i) ) {
      continue;
    }

    ImpNode* node0 = imp_mgr.node(i);

    ymuint64 val0 = node0->bitval();

    if ( val0 != ~0UL ) {
      const_flag[i] |= 1UL;
    }
    if ( val0 != 0UL ) {
      const_flag[i] |= 2UL;
    }
  }
  timer.stop();
  USTime pre_time = timer.time();

  timer.reset();
  timer.start();

  ymuint count_sat = 0;
  ymuint count_unsat = 0;
  ymuint count_abort = 0;
  ymuint count_solve = 0;
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
	Bool3 stat1 = solver.solve(tmp, model);
	++ count_solve;
	if ( stat1 == kB3False ) {
	  // node0 は 1 固定
	  solver.add_clause(~lit);
#if 1
	  cout << "Node#" << i << " is const-1" << endl;
#endif
	  imp_mgr.set_const(i, 1);
	  const_list.push_back(i);
	  ++ count_unsat;
	}
	else if ( stat1 == kB3True ) {
	  ++ count_sat;
	}
	else {
	  ++ count_abort;
	}
      }
      else if ( (const_flag[i] & 2U) == 0U ) {
	// 1 になったことがない．
	VarId vid(i);
	Literal lit(vid, kPolPosi);
	vector<Literal> tmp(1, lit);
	vector<Bool3> model;
	Bool3 stat2 = solver.solve(tmp, model);
	++ count_solve;
	if ( stat2 == kB3False ) {
	  // node0 は 0 固定
	  solver.add_clause(~lit);
#if 1
	  cout << "Node#" << i << " is const-0" << endl;
#endif
	  imp_mgr.set_const(i, 0);
	  const_list.push_back(i);
	  ++ count_unsat;
	}
	else if ( stat2 == kB3True ) {
	  ++ count_sat;
	}
	else {
	  ++ count_abort;
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
	  if ( imp_mgr.is_const(dst_id) ) {
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
  timer.stop();
  USTime sat_time = timer.time();

  cout << "CONST-SAT statistics" << endl
       << " " << setw(10) << count_unsat << " + " << setw(10) << count_sat
       << " + " << count_abort
       << " / " << count_solve << endl
       << "  simulation: " << pre_time << endl
       << "  SAT:        " << sat_time << endl;

#if 0
  // 検証
  {
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
    for (list<ymuint>::const_iterator p = const_list.begin();
	 p != const_list.end(); ++ p) {
      ymuint id = *p;
      if ( imp_info.is_const0(id) ) {
	Literal lit(to_literal(id, 1));
	vector<Literal> tmp(1, lit);
	vector<Bool3> model;
	if ( solver1.solve(tmp, model) != kB3False ) {
	  cout << "ERROR: Node#" << src_id << "is not CONST-0" << endl;
	  ++ nerr;
	}
      }
      else if ( imp_info.is_const1(id) ) {
	Literal lit(to_literal(id, 0));
	vector<Literal> tmp(1, lit);
	vector<Bool3> model;
	if ( solver1.solve(tmp, model) != kB3False ) {
	  cout << "ERROR: Node#" << src_id << "is not CONST-1" << endl;
	  ++ nerr;
	}
      }
    }
    cout << "Total " << nerr << " errors" << endl;
  }
#endif
}

END_NAMESPACE_YM_NETWORKS
