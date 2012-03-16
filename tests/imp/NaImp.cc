
/// @file NaImp.cc
/// @brief NaImp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "NaImp.h"
#include "ImpMgr.h"
#include "ImpInfo.h"
#include "ImpNode.h"
#include "ImpList.h"
#include "ym_logic/SatSolver.h"


BEGIN_NAMESPACE_YM_NETWORKS

BEGIN_NONAMESPACE

bool debug = true;

void
print_list(const list<ImpVal>& imp_list);

bool
cup(const list<ImpVal>& src1,
    const list<ImpVal>& src2,
    list<ImpVal>& dst)
{
  list<ImpVal>::const_iterator p1 = src1.begin();
  list<ImpVal>::const_iterator e1 = src1.end();
  list<ImpVal>::const_iterator p2 = src2.begin();
  list<ImpVal>::const_iterator e2 = src2.end();
  while ( p1 != e1 && p2 != e2 ) {
    const ImpVal& imp1 = *p1;
    const ImpVal& imp2 = *p2;
    ymuint id1 = imp1.id();
    ymuint id2 = imp2.id();
    if ( id1 < id2 ) {
      dst.push_back(imp1);
      ++ p1;
    }
    else if ( id1 > id2 ) {
      dst.push_back(imp2);
      ++ p2;
    }
    else { // id1 == id2
      if ( imp1.val() != imp2.val() ) {
	return false;
      }
      dst.push_back(imp1);
      ++ p1;
      ++ p2;
    }
  }
  for ( ; p1 != e1; ++ p1) {
    const ImpVal& imp = *p1;
    dst.push_back(imp);
  }
  for ( ; p2 != e2; ++ p2) {
    const ImpVal& imp = *p2;
    dst.push_back(imp);
  }
  return true;
}

void
cap(const list<ImpVal>& src1,
    const list<ImpVal>& src2,
    list<ImpVal>& dst)
{
  list<ImpVal>::const_iterator p1 = src1.begin();
  list<ImpVal>::const_iterator e1 = src1.end();
  list<ImpVal>::const_iterator p2 = src2.begin();
  list<ImpVal>::const_iterator e2 = src2.end();
  while ( p1 != e1 && p2 != e2 ) {
    const ImpVal& imp1 = *p1;
    const ImpVal& imp2 = *p2;
    ymuint id1 = imp1.id();
    ymuint id2 = imp2.id();
    if ( id1 < id2 ) {
      ++ p1;
    }
    else if ( id1 > id2 ) {
      ++ p2;
    }
    else { // id1 == id2
      if ( imp1.val() == imp2.val() ) {
	dst.push_back(imp1);
      }
      ++ p1;
      ++ p2;
    }
  }
}

void
merge(list<ImpVal>& dst,
      list<ImpVal>& src)
{
  list<ImpVal>::iterator p = dst.begin();
  list<ImpVal>::iterator p_end = dst.end();
  list<ImpVal>::iterator q = src.begin();
  list<ImpVal>::iterator q_end = src.end();
  while ( p != p_end && q != q_end ) {
    const ImpVal& imp1 = *p;
    const ImpVal& imp2 = *q;
    ymuint id1 = imp1.id();
    ymuint id2 = imp2.id();
    if ( id1 < id2 ) {
      ++ p;
    }
    else if ( id1 > id2 ) {
      dst.insert(p, imp2);
      ++ q;
    }
    else { // id1 == id2
      assert_cond( imp1.val() == imp2.val(), __FILE__, __LINE__);
      ++ p;
      ++ q;
    }
  }
  for ( ; q != q_end; ++ q) {
    const ImpVal& imp = *q;
    dst.push_back(imp);
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
void
print_val(ymuint id,
	  ymuint val)
{
  if ( val == 0 ) {
    cout << "~";
  }
  cout << id;
}

void
print_list(const list<ImpVal>& imp_list)
{
  for (list<ImpVal>::const_iterator p = imp_list.begin();
       p != imp_list.end(); ++ p) {
    cout << " ";
    print_val(p->id(), p->val());
  }
  cout << endl;
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
// クラス NaImp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
NaImp::NaImp()
{
}

// @brief デストラクタ
NaImp::~NaImp()
{
}

// @brief ネットワーク中の間接含意を求める．
// @param[in] imp_mgr マネージャ
// @param[in] direct_imp 直接含意のリスト
// @param[in] imp_info 間接含意のリスト
void
NaImp::learning(ImpMgr& imp_mgr,
		const ImpInfo& direct_imp,
		ImpInfo& imp_info)
{
  imp_mgr.print_network(cout);

  ymuint n = imp_mgr.node_num();

  imp_info.set_size(n);

  vector<list<ImpVal> > imp_lists(n * 2);

  // 外部入力ノードの割り当て情報を作る．
  ymuint npi = imp_mgr.input_num();
  for (ymuint i = 0; i < npi; ++ i) {
    ImpNode* node = imp_mgr.input_node(i);
    ymuint id = node->id();
    ymuint idx_0 = id * 2 + 0;
    ymuint idx_1 = id * 2 + 1;
    // 入力は自分自身だけ
    imp_lists[idx_0].push_back(ImpVal(id, 0));
    imp_lists[idx_1].push_back(ImpVal(id, 1));
  }

  // 論理ノードの割り当て情報を作る．
  vector<ImpNode*> node_list;
  imp_mgr.get_node_list(node_list);
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

    // 出力が0になる割り当ては入力が0になる割り当ての共通部分
    {
      const list<ImpVal>& imp_list0 = imp_lists[idx0_0];
      const list<ImpVal>& imp_list1 = imp_lists[idx1_0];
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
      list<ImpVal>& imp_list = imp_lists[idx_0];
      cap(imp_list0, imp_list1, imp_list);
      for (list<ImpVal>::iterator p = imp_list.begin();
	   p != imp_list.end(); ++ p) {
	const ImpVal& imp = *p;
	ymuint id1 = imp.id();
	ymuint val1 = imp.val();
	put(id, 0, id1, val1, imp_info, direct_imp);
      }
      // 最後に自分自身を足しておく
      bool done = false;
      for (list<ImpVal>::iterator p = imp_list.begin();
	   p != imp_list.end(); ++ p) {
	if ( p->id() > id ) {
	  imp_list.insert(p, ImpVal(id, 0));
	  done = true;
	  break;
	}
      }
      if ( !done ) {
	imp_list.push_back(ImpVal(id, 0));
      }
      if ( debug ) {
	cout << "  result" << endl
	     << "   ";
	print_list(imp_list);
	cout << endl
	     << endl;
      }
    }
    // 出力が1になる割り当ては入力が1になる割り当ての和
    {
      const list<ImpVal>& imp_list0 = imp_lists[idx0_1];
      const list<ImpVal>& imp_list1 = imp_lists[idx1_1];
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
      bool stat = cup(imp_list0, imp_list1, imp_list);
      if ( stat ) {
	for (list<ImpVal>::iterator p = imp_list.begin();
	     p != imp_list.end(); ++ p) {
	  const ImpVal& imp = *p;
	  ymuint id1 = imp.id();
	  ymuint val1 = imp.val();
	  put(id, 1, id1, val1, imp_info, direct_imp);
	}
      }
      // 最後に自分自身を足しておく
      bool done = false;
      for (list<ImpVal>::iterator p = imp_list.begin();
	   p != imp_list.end(); ++ p) {
	if ( p->id() > id ) {
	  imp_list.insert(p, ImpVal(id, 1));
	  done = true;
	  break;
	}
      }
      if ( !done ) {
	imp_list.push_back(ImpVal(id, 1));
      }
      if ( debug ) {
	cout << "  result" << endl
	     << "   ";
	print_list(imp_list);
	cout << endl
	     << endl;
      }
    }
  }

#if 0
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

#if 1
    { // 出力の0とファンイン0の1の共通部分がファンイン1の0の含意となる．
      list<ImpVal>& imp_list_o_0 = imp_lists[idx_0];
      list<ImpVal>& imp_list_i0_1 = imp_lists[idx0_1];
      list<ImpVal>& imp_list_i1_0 = imp_lists[idx1_0];
      list<ImpVal> tmp_list;
      cap(imp_list_o_0, imp_list_i0_1, tmp_list);
      for (list<ImpVal>::iterator p = tmp_list.begin();
	   p != tmp_list.end(); ++ p) {
	const ImpVal& imp = *p;
	put(id1, 0, imp.id(), imp.val(), imp_info, direct_imp);
      }
      merge(imp_list_i1_0, tmp_list);
    }
    { // 出力の0とファンイン1の1の共通部分がファンイン0の0の含意となる．
      list<ImpVal>& imp_list_o_0 = imp_lists[idx_0];
      list<ImpVal>& imp_list_i1_1 = imp_lists[idx1_1];
      list<ImpVal>& imp_list_i0_0 = imp_lists[idx0_0];
      list<ImpVal> tmp_list;
      cap(imp_list_o_0, imp_list_i1_1, tmp_list);
      for (list<ImpVal>::iterator p = tmp_list.begin();
	   p != tmp_list.end(); ++ p) {
	const ImpVal& imp = *p;
	put(id0, 0, imp.id(), imp.val(), imp_info, direct_imp);
      }
      merge(imp_list_i0_0, tmp_list);
    }
#endif
#if 0
    { // 出力の1がファンイン0の1の含意となる．
      list<ImpVal>& imp_list_o_1 = imp_lists[idx_1];
      list<ImpVal>& imp_list_i0_1 = imp_lists[idx0_1];
      for (list<ImpVal>::iterator p = imp_list_o_1.begin();
	   p != imp_list_o_1.end(); ++ p) {
	const ImpVal& imp = *p;
	put(id0, 1, imp.id(), imp.val(), imp_info, direct_imp);
      }
      merge(imp_list_i0_1, imp_list_o_1);
    }
    { // 出力の1がファンイン1の1の含意となる．
      list<ImpVal>& imp_list_o_1 = imp_lists[idx_1];
      list<ImpVal>& imp_list_i1_1 = imp_lists[idx1_1];
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
      for (list<ImpVal>::iterator p = imp_list_o_1.begin();
	   p != imp_list_o_1.end(); ++ p) {
	const ImpVal& imp = *p;
	put(id1, 1, imp.id(), imp.val(), imp_info, direct_imp);
      }
      merge(imp_list_i1_1, imp_list_o_1);
      if ( debug ) {
	cout << "  result" << endl
	     << "   ";
	print_list(imp_list_i1_1);
	cout << endl
	     << endl;
      }
    }
#endif
  }
#endif

#if 1
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
