
/// @file SatImp.cc
/// @brief SatImp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "SatImp.h"
#include "StrNode.h"
#include "SnInput.h"
#include "SnAnd.h"
#include "SnXor.h"
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

void
mark_tfi(StrNode* node,
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
	    const StrEdge& e)
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

END_NONAMESPACE


// @brief ネットワーク中の間接含意を求める．
// @param[in] imp_mgr マネージャ
// @param[in] imp_info 間接含意のリスト
void
SatImp::learning(ImpMgr& imp_mgr,
		 const ImpInfo& d_imp,
		 ImpInfo& imp_info)
{
  ymuint n = imp_mgr.max_node_id();

  imp_info.set_size(n);

  SatSolver solver;
  for (ymuint id = 0; id < n; ++ id) {
    VarId vid = solver.new_var();
    assert_cond( vid.val() == id, __FILE__, __LINE__);
  }

  // ImpMgr から CNF を作る．
  for (ymuint id = 0; id < n; ++ id) {
    StrNode* node = imp_mgr.node(id);
    if ( node == NULL ) continue;
    if ( node->is_input() ) continue;

    Literal lit(VarId(id), kPolPosi);

    const StrEdge& e0 = node->fanin0();
    StrNode* node0 = e0.src_node();
    bool inv0 = e0.src_inv();
    Literal lit0(VarId(node0->id()), inv0 ? kPolNega : kPolPosi);

    const StrEdge& e1 = node->fanin1();
    StrNode* node1 = e1.src_node();
    bool inv1 = e1.src_inv();
    Literal lit1(VarId(node1->id()), inv1 ? kPolNega : kPolPosi);

    if ( node->is_and() ) {
      solver.add_clause(lit0, ~lit);
      solver.add_clause(lit1, ~lit);
      solver.add_clause(~lit0, ~lit1, lit);
    }
    else if ( node->is_xor() ) {
      solver.add_clause(lit0, ~lit1, lit);
      solver.add_clause(~lit0, lit1, lit);
      solver.add_clause(~lit0, ~lit1, ~lit);
      solver.add_clause(lit0, lit1, ~lit);
    }
    else {
      assert_not_reached(__FILE__, __LINE__);
    }
  }

  if ( debug ) {
    cerr << "CNF generated" << endl;
  }

  StopWatch timer;
  timer.start();

  // 各ノードから到達可能な入力ノードのリストを求める．
  vector<vector<ymuint> > input_list_array(n);
  for (ymuint i = 0; i < n; ++ i) {
    StrNode* node0 = imp_mgr.node(i);
    if ( node0 == NULL ) continue;

    vector<bool> tfi_mark(n, false);
    mark_tfi(node0, tfi_mark, input_list_array[i]);
    sort(input_list_array[i].begin(), input_list_array[i].end());
  }

  // 直接含意と対偶の含意をコピーしておく
  for (ymuint src_id = 0; src_id < n; ++ src_id) {
    StrNode* node0 = imp_mgr.node(src_id);
    if ( node0 == NULL ) continue;

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
  vector<list<ImpVal> > cand_info(n * 2);
  vector<ymuint32> const_flag(n, 0U);

  for (ymuint i = 0; i < n; ++ i) {
    if ( debug ) {
      if ( (i % 100) == 0 ) {
	cerr << i << " / " << n << endl;
      }
    }
    StrNode* node0 = imp_mgr.node(i);
    if ( node0 == NULL ) continue;

    ymuint64 val0 = node0->bitval();

    if ( val0 != ~0UL ) {
      const_flag[i] |= 1UL;
    }
    if ( val0 != 0UL ) {
      const_flag[i] |= 2UL;
    }

    for (ymuint j = i + 1; j < n; ++ j) {
      StrNode* node1 = imp_mgr.node(j);
      if ( node1 == NULL ) continue;

      if ( !check_intersect(input_list_array[i], input_list_array[j]) ) {
	continue;
      }
      ymuint64 val1 = node1->bitval();

      // node0 が 0 の時に 0 となるノードを探す．
      if ( (~val0 & val1) == 0UL ) {
	if ( !imp_info.check(i, 0, j, 0) ) {
	  cand_info[i * 2 + 0].push_back(ImpVal(j, 0));
	}
      }
      // node0 が 0 の時に 1 となるノードを探す．
      if ( (~val0 & ~val1) == 0UL ) {
	if ( !imp_info.check(i, 0, j, 1) ) {
	  cand_info[i * 2 + 0].push_back(ImpVal(j, 1));
	}
      }
      // node0 が 1 の時に 0 となるノードを探す．
      if ( (val0 & val1) == 0UL ) {
	if ( !imp_info.check(i, 1, j, 0) ) {
	  cand_info[i * 2 + 1].push_back(ImpVal(j, 0));
	}
      }
      // node0 が 1 の時に 1 となるノードを探す．
      if ( (val0 & ~val1) == 0UL ) {
	if ( !imp_info.check(i, 1, j, 1) ) {
	  cand_info[i * 2 + 1].push_back(ImpVal(j, 1));
	}
      }
    }
  }

  list<ymuint> const_list;
  for (ymuint i = 0; i < n; ++ i) {
    StrNode* node0 = imp_mgr.node(i);
    if ( node0 == NULL ) continue;

    if ( (const_flag[i] & 1U) == 0U ) {
      // 0 になったことがない．
      Literal lit(VarId(i), kPolNega);
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
      else {
	const_flag[i] = 3U;
      }
    }
    else if ( (const_flag[i] & 2U) == 0U ) {
      // 1 になったことがない．
      Literal lit(VarId(i), kPolPosi);
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
      else {
	const_flag[i] = 3U;
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
    StrNode* node0 = imp_mgr.node(src_id);
    if ( node0 == NULL ) continue;

    for (ymuint src_val = 0; src_val < 2; ++ src_val) {
      list<ImpVal>& imp_list = cand_info[src_id * 2 + src_val];
      for (list<ImpVal>::iterator p = imp_list.begin(); p != imp_list.end(); ) {
	ymuint dst_id = p->id();
	if ( imp_info.is_const0(dst_id) || imp_info.is_const1(dst_id) ) {
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

  ymuint prev_size = count_list(cand_info);
  if ( debug ) {
    cerr << "sim#0: " << prev_size << endl;
  }
  ymuint count = 1;
  ymuint nochg = 0;

  for ( ; ; ) {
    imp_mgr.random_sim();

    for (ymuint src_id = 0; src_id < n; ++ src_id) {
      StrNode* node0 = imp_mgr.node(src_id);
      if ( node0 == NULL ) continue;

      ymuint64 orig_val0 = node0->bitval();

      for (ymuint src_val = 0; src_val < 2; ++ src_val) {
	ymuint64 val0 = orig_val0;
	if ( src_val == 0 ) {
	  val0 = ~val0;
	}
	list<ImpVal>& imp_list = cand_info[src_id * 2 + src_val];
	for (list<ImpVal>::iterator p = imp_list.begin();
	     p != imp_list.end(); ) {
	  const ImpVal& imp = *p;
	  ymuint dst_id = imp.id();
	  StrNode* node1 = imp_mgr.node(dst_id);
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
  for (ymuint src_id = 0; src_id < n; ++ src_id) {
    if ( imp_info.is_const0(src_id) || imp_info.is_const1(src_id) ) {
      continue;
    }

    StrNode* node0 = imp_mgr.node(src_id);
    if ( node0 == NULL ) continue;

    for (ymuint src_val = 0; src_val < 2; ++ src_val) {
      Literal lit0(VarId(src_id), src_val == 0 ? kPolNega : kPolPosi);
      const list<ImpVal>& imp_list = cand_info[src_id * 2 + src_val];
      for (list<ImpVal>::const_iterator p = imp_list.begin();
	   p != imp_list.end(); ++ p) {
	if ( debug ) {
	  if ( (count % 1000) == 0 ) {
	    cerr << "sat#" << count << " / " << remain << endl;
	  }
	}
	++ count;
	const ImpVal& imp = *p;
	ymuint dst_id = imp.id();
	ymuint dst_val = imp.val();

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
      StrNode* node = imp_mgr.node(id);
      if ( node == NULL ) continue;
      if ( node->is_input() ) continue;

      Literal lit(VarId(id), kPolPosi);

      const StrEdge& e0 = node->fanin0();
      StrNode* node0 = e0.src_node();
      bool inv0 = e0.src_inv();
      Literal lit0(VarId(node0->id()), inv0 ? kPolNega : kPolPosi);

      const StrEdge& e1 = node->fanin1();
      StrNode* node1 = e1.src_node();
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
