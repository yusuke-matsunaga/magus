
/// @file NaImp2.cc
/// @brief NaImp2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "NaImp2.h"
#include "ImpMgr.h"
#include "ImpInfo.h"
#include "ImpNode.h"
#include "ImpList.h"
#include "ImpVals.h"


BEGIN_NAMESPACE_YM_NETWORKS

BEGIN_NONAMESPACE

#if defined(YM_DEBUG)
bool debug = true;
#else
bool debug = false;
#endif

void
print_list(const list<ImpVals>& imp_list)
{
  for (list<ImpVals>::const_iterator p = imp_list.begin();
       p != imp_list.end(); ++ p) {
    const ImpVals& vals = *p;
    cout << " ";
    vals.print(cout);
  }
  cout << endl;
}

void
merge(list<ImpVals>& dst_list,
      const list<ImpVals>& src_list)
{
  list<ImpVals>::iterator p = dst_list.begin();
  list<ImpVals>::iterator p_end = dst_list.end();
  list<ImpVals>::const_iterator q = src_list.begin();
  list<ImpVals>::const_iterator q_end = src_list.end();
  while ( p != p_end && q != q_end ) {
    const ImpVals& imp1 = *p;
    const ImpVals& imp2 = *q;
    if ( imp1 < imp2 ) {
      ++ p;
    }
    else if ( imp1 > imp2 ) {
      dst_list.insert(p, imp2);
      ++ q;
    }
    else { // id1 == id2
      ++ p;
      ++ q;
    }
  }
  for ( ; q != q_end; ++ q) {
    const ImpVals& imp = *q;
    dst_list.push_back(imp);
  }
}

void
and_merge(list<ImpVals>& dst_list,
	  const list<ImpVals>& src_list1,
	  const list<ImpVals>& src_list2)
{
  list<ImpVals>::iterator p = dst_list.begin();
  list<ImpVals>::iterator p_end = dst_list.end();
  list<ImpVals>::const_iterator q1 = src_list1.begin();
  list<ImpVals>::const_iterator q1_end = src_list1.end();
  list<ImpVals>::const_iterator q2 = src_list2.begin();
  list<ImpVals>::const_iterator q2_end = src_list2.end();
  while ( p != p_end && q1 != q1_end && q2 != q2_end ) {
    const ImpVals& imp = *p;
    const ImpVals& imp1 = *q1;
    const ImpVals& imp2 = *q2;
    if ( id1 < id2 ) {
      ++ p;
    }
    else if ( id1 > id2 ) {
      dst_list.insert(p, imp2);
      ++ q;
    }
    else { // id1 == id2
      ++ p;
      ++ q;
    }
  }
}

void
extract(const list<ImpVals>& src_list,
	list<ImpVal>& imp_list)
{
  for (list<ImpVals>::const_iterator p = src_list.begin();
       p != src_list.end(); ++ p) {
    const ImpVals& vals = *p;
    if ( vals.num() > 1 ) {
      continue;
    }
    imp_list.push_back(vals.val(0));
  }
}

void
put(ymuint src_id,
    ymuint src_val,
    ymuint dst_id,
    ymuint dst_val,
    ImpInfo& imp_info,
    const ImpInfo& direct_imp)
{
  if ( src_id == dst_id && src_val == dst_val ) return;
  if ( !direct_imp.check(src_id, src_val, dst_id, dst_val) &&
       !direct_imp.check(dst_id, dst_val ^ 1, src_id, src_val ^ 1) ) {
    imp_info.put(src_id, src_val, dst_id, dst_val);
    imp_info.put(dst_id, dst_val ^ 1, src_id, src_val ^ 1);
  }
}

inline
Literal
to_literal(ymuint id,
	   ymuint val)
{
  return Literal(VarId(id), (val == 0) ? kPolNega : kPolPosi);
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス NaImp2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
NaImp2::NaImp2()
{
}

// @brief デストラクタ
NaImp2::~NaImp2()
{
}

// @brief ネットワーク中の間接含意を求める．
// @param[in] imp_mgr マネージャ
// @param[in] direct_imp 直接含意のリスト
// @param[in] imp_info 間接含意のリスト
void
NaImp2::learning(ImpMgr& imp_mgr,
		 const ImpInfo& direct_imp,
		 ImpInfo& imp_info)
{
  //imp_mgr.print_network(cout);

  ymuint n = imp_mgr.node_num();

  vector<list<ImpVals> > imp_lists(n * 2);

  // direct_imp の情報を imp_lists にコピーする．
  for (ymuint src_id = 0; src_id < n; ++ src_id) {
    ImpNode* node = imp_mgr.node(src_id);
    if ( node == NULL ) {
      continue;
    }
    // 自分自身を追加する．
    for (ymuint src_val = 0; src_val < 2; ++ src_val) {
      list<ImpVals>& imp_list = imp_lists[src_id * 2 + src_val];
      imp_list.push_back(ImpVals(ImpVal(src_id, src_val)));
    }

    for (ymuint src_val = 0; src_val < 2; ++ src_val) {
      const ImpList& src_list = direct_imp.get(src_id, src_val);
      for (ImpList::iterator p = src_list.begin();
	   p != src_list.end(); ++ p) {
	const ImpCell& imp = *p;
	ymuint dst_id = imp.dst_id();
	ymuint dst_val = imp.dst_val();
	imp_lists[dst_id * 2 + dst_val].push_back(ImpVals(ImpVal(src_id, src_val)));
	ymuint src_val1 = src_val ^ 1;
	ymuint dst_val1 = dst_val ^ 1;
	if ( !direct_imp.check(dst_id, dst_val1, src_id, src_val1) ) {
	  imp_lists[src_id * 2 + src_val1].push_back(ImpVals(ImpVal(dst_id, dst_val1)));
	}
      }
    }
  }

  for (ymuint id = 0; id < n; ++ id) {
    for (ymuint val = 0; val < 2; ++ val) {
      imp_lists[id * 2 + val].sort();
    }
  }

  // 論理ノードの割り当て情報を作る．
  vector<ImpNode*> node_list;
  imp_mgr.get_node_list(node_list);

  for ( ; ; ) {
    bool delta = false;
    for (vector<ImpNode*>::iterator p = node_list.begin();
	 p != node_list.end(); ++ p) {
      ImpNode* node = *p;
      ymuint id = node->id();
      ymuint idx_0 = id * 2 + 0;
      ymuint idx_1 = id * 2 + 1;

      const ImpEdge& e0 = node->fanin0();
      ImpNode* node0 = e0.src_node();
      ymuint id0 = node0->id();
      bool inv0 = e0.src_inv();
      ymuint idx0_0 = id0 * 2 + (inv0 ? 1 : 0);
      ymuint idx0_1 = idx0_0 ^ 1;

      const ImpEdge& e1 = node->fanin1();
      ImpNode* node1 = e1.src_node();
      ymuint id1 = node1->id();
      bool inv1 = e1.src_inv();
      ymuint idx1_0 = id1 * 2 + (inv1 ? 1: 0);
      ymuint idx1_1 = idx1_0 ^ 1;

      // 出力が0になる条件は入力が0になる条件のユニオン
      {
	const list<ImpVals>& imp_list0 = imp_lists[idx0_0];
	const list<ImpVals>& imp_list1 = imp_lists[idx1_0];
	if ( debug ) {
	  cout << "Node#" << id << ": 0" << endl
	     << "  fanin0: ";
	  if ( inv0 ) {
	    cout << "~";
	  }
	  cout << "Node#" << id0 << " ";
	  print_list(imp_list0);
	  cout << "  fanin1: ";
	  if ( inv1 ) {
	    cout << "~";
	  }
	  cout << "Node#" << id1 << " ";
	  print_list(imp_list1);
	}
	list<ImpVals>& imp_list = imp_lists[idx_0];
	ymuint nprev = imp_list.size();
	merge(imp_list, imp_list0);
	merge(imp_list, imp_list1);
	delta += imp_list.size() - nprev;
	if ( debug ) {
	  cout << "  result" << endl
	       << "   ";
	  print_list(imp_list);
	  cout << endl;
	}
      }
      // 出力が1になる条件は入力が1になる条件のインターセクション
      {
	const list<ImpVals>& imp_list0 = imp_lists[idx0_1];
	const list<ImpVals>& imp_list1 = imp_lists[idx1_1];
	if ( debug ) {
	  cout << "Node#" << id << ": 1" << endl
	       << "  fanin0: ";
	  if ( inv0 ) {
	    cout << "~";
	  }
	  cout << "Node#" << id0 << " ";
	  print_list(imp_list0);
	  cout << "  fanin1: ";
	  if ( inv1 ) {
	    cout << "~";
	  }
	  cout << "Node#" << id1 << " ";
	  print_list(imp_list1);
	}
	list<ImpVal>& imp_list = imp_lists[idx_1];
	ymuint nprev = imp_list.size();
	and_merge(imp_list, imp_list0, imp_list1);
	delta += imp_list.size() - nprev;
	if ( debug ) {
	  cout << "  result" << endl
	       << "   ";
	  print_list(imp_list);
	  cout << endl;
	}
      }
    }
    cout << "phase1: delta = " << delta << endl;

    for (vector<ImpNode*>::reverse_iterator p = node_list.rbegin();
	 p != node_list.rend(); ++ p) {
      ImpNode* node = *p;
      ymuint id = node->id();
      ymuint idx_0 = id * 2 + 0;
      ymuint idx_1 = id * 2 + 1;

      const ImpEdge& e0 = node->fanin0();
      ImpNode* node0 = e0.src_node();
      ymuint id0 = node0->id();
      bool inv0 = e0.src_inv();
      ymuint idx0_0 = id0 * 2 + (inv0 ? 1 : 0);
      ymuint idx0_1 = idx0_0 ^ 1;

      const ImpEdge& e1 = node->fanin1();
      ImpNode* node1 = e1.src_node();
      ymuint id1 = node1->id();
      bool inv1 = e1.src_inv();
      ymuint idx1_0 = id1 * 2 + (inv1 ? 1: 0);
      ymuint idx1_1 = idx1_0 ^ 1;

      { // 出力の0の条件とファンイン0の1の条件の共通部分が
	// ファンイン1の0の条件となる．
	const list<ImpVals>& imp_list_o_0 = imp_lists[idx_0];
	const list<ImpVals>& imp_list_i0_1 = imp_lists[idx0_1];
	list<ImpVals>& imp_list_i1_0 = imp_lists[idx1_0];
	if ( debug ) {
	  cout << "Node#" << id << ":0";
	  print_list(imp_list_o_0);
	  cout << "fanin0: ";
	  if ( inv0 ) {
	    cout << "~";
	  }
	  cout << "Node#" << id0 << ":1";
	  print_list(imp_list_i0_1);
	}
	ymuint nprev = imp_list_i1_0.size();
	and_merge(imp_list_i1_0, imp_list_o_0, imp_list_i0_1);
	delta += imp_list_i1_0.size() - nprev;
	if ( debug ) {
	  cout << "  result" << endl
	       << "   ";
	  print_list(imp_list_i1_0);
	  cout << endl;
	}
      }
      { // 出力の0の条件とファンイン1の1の条件の共通部分が
	// ファンイン0の0の条件となる．
	const list<ImpVals>& imp_list_o_0 = imp_lists[idx_0];
	const list<ImpVals>& imp_list_i1_1 = imp_lists[idx1_1];
	list<ImpVals>& imp_list_i0_0 = imp_lists[idx0_0];
	if ( debug ) {
	  cout << "Node#" << id << ":0";
	  print_list(imp_list_o_0);
	  cout << "fanin1: ";
	  if ( inv1 ) {
	    cout << "~";
	  }
	  cout << "Node#" << id1 << ":1";
	  print_list(imp_list_i1_1);
	}
	ymuint nprev = imp_list_i0_0.size();
	and_merge(imp_list_i0_0, imp_list_o_0, imp_list_i1_1);
	delta += imp_list_i0_0.size() - nprev;
	if ( debug ) {
	  cout << "  result" << endl
	       << "   ";
	  print_list(imp_list_i0_0);
	  cout << endl;
	}
      }
      { // 出力の1の条件がファンイン0の1の条件となる．
	const list<ImpVals>& imp_list_o_1 = imp_lists[idx_1];
	list<ImpVals>& imp_list_i0_1 = imp_lists[idx0_1];
	if ( debug ) {
	  cout << "Node#" << id << ":1";
	  print_list(imp_list_o_1);
	  cout << "  fanin0: ";
	  if ( inv0 ) {
	    cout << "~";
	  }
	  cout << "Node#" << id0 << ":1";
	  print_list(imp_list_i0_1);
	}
	ymuint nprev = imp_list_i0_1.size();
	merge(imp_list_i0_1, imp_list_o_1);
	delta += imp_list_i0_1.size() - nprev;
	if ( debug ) {
	  cout << "  result" << endl
	       << "   ";
	  print_list(imp_list_i0_1);
	  cout << endl;
	}
      }
      { // 出力の1の条件がファンイン1の1の条件となる．
	const list<ImpVals>& imp_list_o_1 = imp_lists[idx_1];
	list<ImpVals>& imp_list_i1_1 = imp_lists[idx1_1];
	if ( debug ) {
	  cout << "Node#" << id << ":1";
	  print_list(imp_list_o_1);
	  cout << "  fanin1: ";
	  if ( inv1 ) {
	    cout << "~";
	  }
	  cout << "Node#" << id1 << ":1";
	  print_list(imp_list_i1_1);
	}
	ymuint nprev = imp_list_i1_1.size();
	merge(imp_list_i1_1, imp_list_o_1);
	delta += imp_list_i1_1.size() - nprev;
	if ( debug ) {
	  cout << "  result" << endl
	       << "   ";
	  print_list(imp_list_i1_1);
	  cout << endl;
	}
      }
    }
    cout << "phase2: delta = " << delta << endl;
    if ( delta == 0 ) {
      break;
    }
  }

  // imp_lists の情報から imp_info を作る．
  imp_info.set_size(n);
  for (ymuint dst_id = 0; dst_id < n; ++ dst_id) {
    ImpNode* node = imp_mgr.node(dst_id);
    if ( node == NULL ) {
      continue;
    }
    cout << "dst_id = " << dst_id << endl;
    for (ymuint dst_val = 0; dst_val < 2; ++ dst_val) {
      const list<ImpVals>& src_list = imp_lists[dst_id * 2 + dst_val];
      list<ImpVal> imp_list;
      extract(src_list, imp_list);
      for (list<ImpVal>::const_iterator p = imp_list.begin();
	   p != imp_list.end(); ++ p) {
	const ImpVal& val = *p;
	ymuint src_id = val.id();
	ymuint src_val = val.val();
	if ( src_id == dst_id ) {
	  continue;
	}
	if ( !imp_info.check(src_id, src_val, dst_id, dst_val) ) {
	  imp_info.put(src_id, src_val, dst_id, dst_val);
	}
	if ( !imp_info.check(dst_id, dst_val ^ 1, src_id, src_val ^ 1) ) {
	  imp_info.put(dst_id, dst_val ^ 1, src_id, src_val ^ 1);
	}
      }
    }
  }

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

      assert_cond( node->is_and(), __FILE__, __LINE__);

      Literal lit(VarId(id), kPolPosi);

      const ImpEdge& e0 = node->fanin0();
      ImpNode* node0 = e0.src_node();
      bool inv0 = e0.src_inv();
      Literal lit0(VarId(node0->id()), inv0 ? kPolNega : kPolPosi);

      const ImpEdge& e1 = node->fanin1();
      ImpNode* node1 = e1.src_node();
      bool inv1 = e1.src_inv();
      Literal lit1(VarId(node1->id()), inv1 ? kPolNega : kPolPosi);

      solver1.add_clause(lit0, ~lit);
      solver1.add_clause(lit1, ~lit);
      solver1.add_clause(~lit0, ~lit1, lit);
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
