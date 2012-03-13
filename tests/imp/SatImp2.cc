
/// @file SatImp2.cc
/// @brief SatImp2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "SatImp2.h"
#include "ImpNode.h"
#include "ImpInput.h"
#include "ImpAnd.h"
#include "ImpMgr.h"
#include "ImpInfo.h"
#include "ImpList.h"
#include "ImpMgr.h"
#include "StrImp.h"
#include "ym_logic/SatSolver.h"
#include "ym_utils/RandGen.h"
#include "ym_utils/StopWatch.h"


BEGIN_NAMESPACE_YM_NETWORKS

BEGIN_NONAMESPACE

bool debug = true;

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス SatImp2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SatImp2::SatImp2()
{
}

// @brief デストラクタ
SatImp2::~SatImp2()
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

void
print_fanin(ostream& s,
	    const ImpEdge& e)
{
  if ( e.src_inv() ) {
    s << "~";
  }
  s << e.src_node()->id();
}

END_NONAMESPACE


// @brief ネットワーク中の間接含意を求める．
// @param[in] imp_mgr マネージャ
// @param[in] imp_info 間接含意のリスト
void
SatImp2::learning(ImpMgr& imp_mgr,
		  const ImpInfo& d_imp,
		  ImpInfo& imp_info)
{
  ymuint n = imp_mgr.node_num();

  imp_info.set_size(n);

  // 各ノードから到達可能な入力ノードのリストを求める．
  vector<vector<ymuint> > input_list_array(n);
  for (ymuint i = 0; i < n; ++ i) {
    ImpNode* node = imp_mgr.node(i);

    vector<bool> tfi_mark(n, false);
    mark_tfi(node, tfi_mark, input_list_array[i]);
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
  for (ymuint i = 0; i < n; ++ i) {
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

    for (ymuint j = i + 1; j < n; ++ j) {
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
    cerr << "nsat0 = " << prev_size << endl;
  }
  ymuint count_sim = 1;
  ymuint nochg = 0;
  for ( ; ; ) {
    imp_mgr.random_sim();

    for (ymuint src_id = 0; src_id < n; ++ src_id) {
      ImpNode* node0 = imp_mgr.node(src_id);
      ymuint64 orig_val0 = node0->bitval();
      if ( orig_val0 != ~0UL ) {
	const_flag[src_id] |= 1U;
      }
      if ( orig_val0 != 0UL ) {
	const_flag[src_id] |= 2U;
      }

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
      if ( (count_sim % 100) == 0 ) {
	cerr << "nsat" << count_sim << " = " << cur_size << endl;
      }
    }
    ++ count_sim;

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

  ymuint32 count_0_success = 0;
  ymuint32 count_0_fail = 0;
  ymuint32 count_1_success = 0;
  ymuint32 count_1_fail = 0;
  ymuint32 count_imp_success = 0;
  ymuint32 count_imp_fail = 0;

  vector<ImpNode*> node_list;
  imp_mgr.get_node_list(node_list);
  for (ymuint i = 0; i < n; ++ i) {
    ImpNode* node0 = imp_mgr.node(i);

    if ( (const_flag[i] & 1U) == 0U ) {
      // 0 になったことがない．
      SatSolver solver;
      for (ymuint j = 0; j < n; ++ j) {
	VarId vid = solver.new_var();
	assert_cond( vid.val() == j, __FILE__, __LINE__);
      }
      vector<bool> mark(n, false);
      make_cnf(solver, node0, mark);

      Literal lit(VarId(i), kPolNega);
      vector<Literal> tmp(1, lit);
      vector<Bool3> model;
      if ( solver.solve(tmp, model) == kB3False ) {
	// node0 は 1 固定
	solver.add_clause(~lit);
#if 0
	cout << "Node#" << i << " is const-1" << endl;
#endif
	imp_info.set_1(i);
	++ count_1_success;
      }
      else {
	const_flag[i] = 3U;
	++ count_1_fail;
      }
    }
    else if ( (const_flag[i] & 2U) == 0U ) {
      // 1 になったことがない．
      SatSolver solver;
      for (ymuint j = 0; j < n; ++ j) {
	VarId vid = solver.new_var();
	assert_cond( vid.val() == j, __FILE__, __LINE__);
      }
      vector<bool> mark(n, false);
      make_cnf(solver, node0, mark);
      Literal lit(VarId(i), kPolPosi);

      vector<Literal> tmp(1, lit);
      vector<Bool3> model;
      if ( solver.solve(tmp, model) == kB3False ) {
	// node0 は 0 固定
	solver.add_clause(~lit);
#if 0
	cout << "Node#" << i << " is const-0" << endl;
#endif
	imp_info.set_0(i);
	++ count_0_success;
      }
      else {
	const_flag[i] = 3U;
	++ count_0_fail;
      }
    }
  }

  ymuint remain = count_list(cand_info);
  ymuint32 count_sat = 1;
  for (ymuint src_id = 0; src_id < n; ++ src_id) {
    if ( imp_info.is_const0(src_id) || imp_info.is_const1(src_id) ) continue;

    ImpNode* src_node = imp_mgr.node(src_id);

    for (ymuint src_val = 0; src_val < 2; ++ src_val) {
      Literal lit0(VarId(src_id), src_val == 0 ? kPolNega : kPolPosi);
      const list<ImpDst>& imp_list = cand_info[src_id * 2 + src_val];
      for (list<ImpDst>::const_iterator p = imp_list.begin();
	   p != imp_list.end(); ++ p) {
	if ( debug ) {
	  cerr << "sat#" << count_sat << " / " << remain << endl;
	}
	++ count_sat;
	ImpNode* dst_node = p->node();
	ymuint dst_id = dst_node->id();
	ymuint dst_val = p->val();
	if ( imp_info.is_const0(dst_id) || imp_info.is_const1(dst_id) ) continue;

	SatSolver solver;
	for (ymuint j = 0; j < n; ++ j) {
	  VarId vid = solver.new_var();
	  assert_cond( vid.val() == j, __FILE__, __LINE__);
	}
	vector<bool> mark(n, false);
	make_cnf(solver, src_node, mark);
	make_cnf(solver, dst_node, mark);

	Literal lit1(VarId(dst_id), dst_val == 0 ? kPolNega : kPolPosi);
	vector<Literal> tmp(2);
	vector<Bool3> model;
	tmp[0] = lit0;
	tmp[1] = ~lit1;
	if ( solver.solve(tmp, model) == kB3False ) {
	  imp_info.put(src_id, src_val, dst_id, dst_val);
	  imp_info.put(dst_id, dst_val ^ 1, src_id, src_val ^ 1);
	  ++ count_imp_success;
	}
	else {
	  ++ count_imp_fail;
	}
      }
    }
  }

  cout << "SAT statistics" << endl
       << "  simulation:      " << setw(10) << count_sim << endl
       << "  const-0: success " << setw(10) << count_0_success << endl
       << "           fail    " << setw(10) << count_0_fail << endl
       << "  const-1: success " << setw(10) << count_1_success << endl
       << "           fail    " << setw(10) << count_1_fail << endl
       << "  imp:     success " << setw(10) << count_imp_success << endl
       << "           fail    " << setw(10) << count_imp_fail << endl;
}

END_NAMESPACE_YM_NETWORKS
