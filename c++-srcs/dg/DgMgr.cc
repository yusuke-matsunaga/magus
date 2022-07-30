
/// @file DgMgr.cc
/// @brief DgMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "DgMgr.h"
#include "DgNode.h"
#include "DgEdge.h"
#include "NodeMark.h"
#include "ym/BddVarSet.h"


BEGIN_NAMESPACE_DG

BEGIN_NONAMESPACE

bool debug = false;

void
dfs(
  DgEdge edge,
  vector<DgNode*>& node_list,
  unordered_set<SizeType>& node_set
)
{
  if ( edge.is_const() ) {
    return;
  }
  auto node = edge.node();
  if ( node_set.count(node->id()) == 0 ) {
    node_set.emplace(node->id());
    node_list.push_back(node);
    SizeType n = node->child_num();
    for ( SizeType i = 0; i < n; ++ i ) {
      dfs(node->child(i), node_list, node_set);
    }
  }
}

void
print(
  ostream& s,
  DgEdge edge
)
{
  vector<DgNode*> node_list;
  unordered_set<SizeType> node_set;
  dfs(edge, node_list, node_set);

  s << "root: ";
  edge.print(s);
  s << endl;
  for ( auto node: node_list ) {
    node->print(s);
  }
  s << endl;
}

void
print_sup(
  ostream& s,
  const BddVarSet& sup
)
{
  auto v_list = sup.to_varlist();
  for ( auto var: v_list ) {
    cout << " " << var;
  }
  cout << endl;
}

END_NONAMESPACE

// @brief コンストラクタ
DgMgr::DgMgr(
  BddMgr& mgr
) : mBddMgr{mgr}
{
}

// @brief デストラクタ
DgMgr::~DgMgr()
{
  for ( auto node: mNodeList ) {
    delete node;
  }
}

// @brief 与えられた関数の Disjoint Graph を作る．
DgEdge
DgMgr::decomp(
  const Bdd& func
)
{
  return decomp_step(func);
}

// @brief decomp の下請け関数
DgEdge
DgMgr::decomp_step(
  const Bdd& func
)
{
  if ( func.is_zero() ) {
    return DgEdge::zero();
  }
  if ( func.is_one() ) {
    return DgEdge::one();
  }

  if ( debug ) {
    cout << "decomp_step begin" << endl
	 << " func" << endl;
    func.display(cout);
  }

  DgEdge result;
  // 登録済みかどうか調べる．
  if ( find_node(func, result) ) {
    if ( debug ) {
      cout << "  already exists" << endl;
      cout << " result" << endl;
      print(cout, result);
    }
    // 結果を返す．
    return result;
  }

  // 根本の変数で分解する．
  Bdd func0;
  Bdd func1;
  auto top_var = func.root_decomp(func0, func1);
  if ( debug ) {
    cout << " top_var = " << top_var << endl;
    cout << " func0" << endl;
    func0.display(cout);
    cout << " func1" << endl;
    func1.display(cout);
    cout << endl;
  }

  // コファクターに対して再帰的に処理を行う．
  auto r0 = decomp_step(func0);
  auto r1 = decomp_step(func1);
  // 結果をマージする．
  if ( debug ) {
    cout << "merge at " << top_var.val() << endl;
    cout << "r0" << endl;
    print(cout, r0);
    cout << "r1" << endl;
    print(cout, r1);
    cout << endl;
  }
  result = merge(top_var.val(), r0, r1);
  if ( debug ) {
    cout << "merge end" << endl;
    func.display(cout);
    cout << "merge at " << top_var.val() << endl;
    cout << "r0" << endl;
    print(cout, r0);
    cout << "r1" << endl;
    print(cout, r1);
    cout << endl;
    cout << "  ==> ";
    print(cout, result);
    ASSERT_COND( result.global_func() == func );
  }
  // 結果を登録する．
  put_node(func, result);

  if ( debug ) {
    cout << "decomp_step end" << endl;
    cout << " func" << endl;
    func.display(cout);
    cout << " result" << endl;
    print(cout, result);
  }

  return result;
}

// @brief コファクターの結果をマージする．
DgEdge
DgMgr::merge(
  SizeType top,
  DgEdge r0,
  DgEdge r1
)
{
  // 一方もしくは両方の結果が定数の場合の処理
  if ( r0.is_zero() ) {
    if ( r1.is_one() ) {
      // 肯定のリテラル
      auto result = make_lit(top);
      return result;
    }
    else {
      // リテラルとのAND ( x & r1 )
      auto result = make_lit_and(top, false, r1);
      return result;
    }
  }
  else if ( r0.is_one() ) {
    if ( r1.is_zero() ) {
      // 否定のリテラル
      auto result = make_lit(top) ^ true;
      return result;
    }
    else {
      // リテラルとのOR ( ~x | r1 )
      auto result = make_lit_or(top, true, r1);
      return result;
    }
  }
  else if ( r1.is_zero() ) {
    // リテラルとのAND ( ~x & r0 )
    auto result = make_lit_and(top, true, r0);
    return result;
  }
  else if ( r1.is_one() ) {
    // リテラルとのOR ( x | r0 )
    auto result = make_lit_or(top, false, r0);
    return result;
  }
  else if ( DgEdge::check_complement(r0, r1) ) {
    // リテラルとのXOR ( ~x & r0 | x & ~r0 )
    auto result = make_lit_xor(top, false, r0);
    return result;
  }

  // ここまで来たらどちらも定数ではない．
  bool inv0 = r0.inv();
  bool inv1 = r1.inv();
  DgNode* node0 = r0.node();
  DgNode* node1 = r1.node();
  SizeType nc0 = node0->child_num();
  SizeType nc1 = node1->child_num();

  // node0, node1 に共通な子ノードのリスト
  vector<DgEdge> common_list;
  // node0 のみの子ノードのリスト
  vector<DgEdge> rest0_list;
  // node1 のみの子ノードのリスト
  vector<DgEdge> rest1_list;
  { // 共通な子ノードのリストを作る．
    SizeType i0 = 0;
    SizeType i1 = 0;
    while ( i0 < nc0 && i1 < nc1 ) {
      DgEdge ce0 = node0->child(i0);
      DgEdge ce1 = node1->child(i1);
      DgNode* cnode0 = ce0.node();
      DgNode* cnode1 = ce1.node();
      int diff = cnode0->top() - cnode1->top();
      if ( diff < 0 ) {
	rest0_list.push_back(ce0);
	++ i0;
      }
      else if ( diff == 0 ) {
	if ( ce0 == ce1 ) {
	  common_list.push_back(ce0);
	}
	else {
	  rest0_list.push_back(ce0);
	  rest1_list.push_back(ce1);
	}
	++ i0;
	++ i1;
      }
      else { // diff > 0
	rest1_list.push_back(ce1);
	++ i1;
      }
    }
    while ( i0 < nc0 ) {
      DgEdge ce0 = node0->child(i0);
      DgNode* cnode0 = ce0.node();
      rest0_list.push_back(ce0);
      ++ i0;
    }
    while ( i1 < nc1 ) {
      DgEdge ce1 = node1->child(i1);
      DgNode* cnode1 = ce1.node();
      rest1_list.push_back(ce1);
      ++ i1;
    }
  }

  if ( debug ) {
    cout << "common:" << endl;
    for ( auto edge: common_list ) {
      print(cout, edge);
      cout << "===" << endl;
    }
    cout << "rest0:" << endl;
    for ( auto edge: rest0_list ) {
      print(cout, edge);
      cout << "===" << endl;
    }
    cout << "rest1:" << endl;
    for ( auto edge: rest1_list ) {
      print(cout, edge);
      cout << "===" << endl;
    }
  }

  if ( common_list.size() >= 1 ) {
    // Case1-OR
    if ( inv0 == inv1 && node0->is_or() && node1->is_or() ) {
      auto result = case1_or(top, common_list, rest0_list, rest1_list) ^ inv0;
      return result;
    }

    // Case1-XOR
    if ( node0->is_xor() && node1->is_xor() ) {
      auto result = case1_xor(top, common_list, rest0_list, rest1_list, inv0, inv1);
      return result;
    }
  }

  // Case2-OR
  if ( node0->is_or() ) {
    for ( SizeType i = 0; i < nc0; ++ i ) {
      if ( (node0->child(i) ^ inv0) == r1 ) {
	auto result = case2_or(top, true, node0, inv0, i);
	return result;
      }
    }
  }
  if ( node1->is_or() ) {
    for ( SizeType i = 0; i < nc1; ++ i ) {
      if ( (node1->child(i) ^ inv1) == r0 ) {
	auto result = case2_or(top, false, node1, inv1, i);
	return result;
      }
    }
  }

  // Case2-XOR
  if ( node0->is_xor() ) {
    for ( SizeType i = 0; i < nc0; ++ i ) {
      if ( node0->child(i).node() == node1 ) {
	auto result = case2_xor(top, true, node0, inv0, i, inv1);
	return result;
      }
    }
  }
  if ( node1->is_xor() ) {
    for ( SizeType i = 0; i < nc1; ++ i ) {
      if ( node1->child(i).node() == node0 ) {
	auto result = case2_xor(top, false, node1, inv1, i, inv0);
	return result;
      }
    }
  }

  // ここまで来ると CPLX タイプなのでグローバル関数が必要になる．
  auto f0 = r0.global_func();
  auto f1 = r1.global_func();
  auto var = mBddMgr.posi_literal(VarId{top});
  auto f = ite(var, f1, f0);
  if ( node0->is_cplx() && node1->is_cplx() ) {
    if ( rest0_list.size() == 1 && rest1_list.size() == 1 ) {
      // r0, r1 が共に CPLX でただ一つの子ノード以外共通の場合
      auto r0_1 = rest0_list[0];
      auto r1_1 = rest1_list[0];

      // 根の関数が等しいか調べる．
      // f0 に対して r0_1 が 0 に束縛した関数と
      // f1 に対して r1_1 を 0 に束縛した関数が等価かどうか調べる．
      auto f0_0 = f0 / r0_1.pat_0();
      auto f1_0 = f1 / r1_1.pat_0();

      // f0 に対して r0_1 が 1 に束縛した関数と
      // f1 に対して r1_1 を 1 に束縛した関数が等価かどうか調べる．
      auto f0_1 = f0 / r0_1.pat_1();
      auto f1_1 = f1 / r1_1.pat_1();

      if ( f0_0 == f1_0 && f0_1 == f1_1 ) {
	// 一致した．
	auto result = case1_cplx(f, top, node0, node1, common_list, r0_1, r1_1);
	return result;
      }

      // f0 に対して r0_1 が 0 に束縛した関数と
      // f1 に対して r1_1 を 1 に束縛した関数が等価かどうか調べる．

      // f0 に対して r0_1 が 1 に束縛した関数と
      // f1 に対して r1_1 を 0 に束縛した関数が等価かどうか調べる．
      if ( f0_0 == f1_1 && f0_1 == f1_0 ) {
	// 一致した．
	auto result = case1_cplx(f, top, node0, node1, common_list, ~r0_1, r1_1);
	return result;
      }
    }
    else if ( rest0_list.empty() && rest1_list.empty() ) {
      // どれかの子ノードを反転すると一致する場合
      for ( SizeType i = 0; i < common_list.size(); ++ i ) {
	auto chd = common_list[i];
	auto f0_0 = f0 / chd.pat_0();
	auto f0_1 = f0 / chd.pat_1();
	auto f1_0 = f1 / chd.pat_0();
	auto f1_1 = f1 / chd.pat_1();
	if ( f0_0 == f1_1 && f0_1 == f1_0 ) {
	  // 一致した．
	  auto result = case1_cplx2(f, top, common_list, i);
	  return result;
	}
      }
    }
  }

  auto sup0 = node0->support();
  auto sup1 = node1->support();
  if ( !(sup0 && sup1) ) {
    // ITE(top, r1, r0) となる．
    auto lit = make_lit(top);
    auto result = make_cplx(f, {lit, r0, r1});
    return result;
  }

  auto sup0_diff = sup0 - sup1;
  auto sup1_diff = sup1 - sup0;
  if ( node0->is_cplx() && sup0_diff.size() > 0 && sup1_diff.size() == 0 ) {
    for ( SizeType i = 0; i < nc0; ++ i ) {
      auto cedge = node0->child(i);
      auto cnode = cedge.node();
      auto& csup = cnode->support();
      if ( !(csup && sup1) ) {
	// f0 に対して cedge が 0 になる割当を行うと f1 と一致する場合
	auto f0_0 = f0 / cedge.pat_0();
	if ( f0_0 == f1 ) {
	  auto result = case2_cplx(f, top, cedge, true, true, node0);
	  return result;
	}
	// f0 に対して cedge が 1 になる割当を行うと f1 と一致する場合
	auto f0_1 = f0 / cedge.pat_1();
	if ( f0_1 == f1 ) {
	  auto result = case2_cplx(f, top, cedge, false, false, node0);
	  return result;
	}
      }
    }
  }
  if ( node1->is_cplx() && sup1_diff.size() > 0 && sup0_diff.size() == 0 ) {
    for ( SizeType i = 0; i < nc1; ++ i ) {
      auto cedge = node1->child(i);
      auto cnode = cedge.node();
      auto& csup = cnode->support();
      if ( !(csup && sup0) ) {
	// f1 に対して cedge が 0 になる割当を行うと f0 と一致する場合
	auto f1_0 = f1 / cedge.pat_0();
	if ( f1_0 == f0 ) {
	  auto result = case2_cplx(f, top, cedge, false, true, node1);
	  return result;
	}
	// f1 に対して cedge が 1 になる割当を行うと f0 と一致する場合
	auto f1_1 = f1 / cedge.pat_1();
	if ( f1_1 == f0 ) {
	  auto result = case2_cplx(f, top, cedge, true, false, node1);
	  return result;
	}
      }
    }
  }

  if ( debug ) {
    cout << "last resort" << endl;
  }

  // node0 と node1 の推移的な子ノードのうち，共通なノードを求める．
  // それらを新しい子供とした CPLX ノードを作る．
  // 面倒なのは OR/XOR ノードが極大化されているということ．
  NodeMark mark;
  // node0 の推移的ファンインに 1 のマークをつける．
  mark.mark_recur(node0, 1);
  // node1 の推移的ファンインに 2 のマークをつける．
  mark.mark_recur(node1, 2);
  // 自分には 3 のマークがなく，推移的ファンインに 3
  // のマークがついているノードのマークを 0 にする．
  mark.tfimark_recur(node0);
  mark.tfimark_recur(node1);
  // 境界ノードに印をつける．
  vector<DgNode*> or_list0;
  vector<DgNode*> xor_list0;
  mark.get_boundary(node0, or_list0, xor_list0);
  vector<DgNode*> or_list1;
  vector<DgNode*> xor_list1;
  mark.get_boundary(node1, or_list1, xor_list1);

  // 入力を求める．
  vector<DgEdge> tmp_inputs;
  find_uncommon_inputs(or_list0, mark, 1, tmp_inputs);
  find_uncommon_inputs(or_list1, mark, 2, tmp_inputs);
  find_uncommon_inputs(xor_list0, mark, 1, tmp_inputs);
  find_uncommon_inputs(xor_list1, mark, 2, tmp_inputs);
  find_common_inputs(or_list0, or_list1, mark, tmp_inputs);
  find_common_inputs(xor_list0, xor_list1, mark, tmp_inputs);
  mark.find_bnode(node0, tmp_inputs);
  mark.find_bnode(node1, tmp_inputs);
  auto top_edge = make_lit(top);
  tmp_inputs.push_back(top_edge);
  auto result = make_cplx(f, tmp_inputs);
  return result;
}

// @brief 共通でないファンインを求める．
void
DgMgr::find_uncommon_inputs(
  const vector<DgNode*>& node_list,
  NodeMark& mark,
  int mval,
  vector<DgEdge>& inputs
)
{
  for ( auto node: node_list ) {
    SizeType nc = node->child_num();
    vector<DgEdge> tmp_inputs;
    for ( SizeType i = 0; i < nc; ++ i ) {
      auto cedge = node->child(i);
      auto cnode = cedge.node();
      if ( mark.bmark(cnode) == 1 && mark.mark(cnode) == mval ) {
	tmp_inputs.push_back(cedge);
      }
    }

    if ( tmp_inputs.size() > 1 ) {
      if ( node_list[0]->is_or() ) {
	auto tmp_edge = make_or(tmp_inputs);
	inputs.push_back(tmp_edge);
      }
      else {
	auto tmp_edge = make_xor(tmp_inputs);
	inputs.push_back(tmp_edge);
      }
      for ( auto edge: tmp_inputs ) {
	auto node = edge.node();
	mark.set_bmark(node, 3);
      }
    }
  }
}

// @brief 共通なファンインを求める．
void
DgMgr::find_common_inputs(
  const vector<DgNode*>& node_list1,
  const vector<DgNode*>& node_list2,
  NodeMark& mark,
  vector<DgEdge>& inputs
)
{
  for ( auto node1: node_list1 ) {
    SizeType nc1 = node1->child_num();
    unordered_set<DgEdge> common_set;
    for ( SizeType i = 0; i < nc1; ++ i ) {
      auto cedge = node1->child(i);
      auto cnode = cedge.node();
      if ( mark.bmark(cnode) == 1 && mark.mark(cnode) == 3 ) {
	common_set.emplace(cedge);
      }
    }
    for ( auto node2: node_list2 ) {
      SizeType nc2 = node2->child_num();
      vector<DgEdge> common;
      for ( SizeType i = 0; i < nc2; ++ i ) {
	auto cedge = node2->child(i);
	auto cnode = cedge.node();
	if ( mark.bmark(cnode) == 1 ) {
	  if ( common_set.count(cedge) > 0 ) {
	    common.push_back(cedge);
	  }
	}
      }
      SizeType nc = common.size();
      if ( nc > 1 ) {
	DgNode* cnode;
	if ( nc == nc1 ) {
	  inputs.push_back(DgEdge{node1});
	}
	else if ( nc == nc2 ) {
	  inputs.push_back(DgEdge{node2});
	}
	else {
	  if ( node1->is_or() ) {
	    auto cedge = make_or(common);
	    inputs.push_back(cedge);
	  }
	  else {
	    auto cedge = make_xor(common);
	    inputs.push_back(cedge);
	  }
	}
	for ( auto edge: common ) {
	  auto node = edge.node();
	  mark.set_bmark(node, 3);
	}
      }
    }
  }
}

// @brief Case1-OR のマージを行う．
DgEdge
DgMgr::case1_or(
  SizeType index,
  const vector<DgEdge>& common_list,
  const vector<DgEdge>& rest0_list,
  const vector<DgEdge>& rest1_list
)
{
  // rest0_list, rest1_list が空の場合でも merge() が
  // 適切に処理してくれる．
  auto tmp0_edge = make_or(rest0_list);
  auto tmp1_edge = make_or(rest1_list);
  if ( debug ) {
    cout << "[case1_or] merge at " << index << endl;
    cout << "r0" << endl;
    print(cout, tmp0_edge);
    cout << "r1" << endl;
    print(cout, tmp1_edge);
    cout << endl;
  }
  auto new_edge = merge(index, tmp0_edge, tmp1_edge);
  if ( debug ) {
    cout << "[case1_or] merge end" << endl;
    cout << "merge at " << index << endl;
    cout << "r0" << endl;
    print(cout, tmp0_edge);
    cout << "r1" << endl;
    print(cout, tmp1_edge);
    cout << endl;
    cout << "  ==> ";
    print(cout, new_edge);
  }

  vector<DgEdge> child_list{new_edge};
  child_list.insert(child_list.end(),
		    common_list.begin(), common_list.end());
  return make_or(child_list);
}

// @brief Case1-XOR のマージを行う．
DgEdge
DgMgr::case1_xor(
  SizeType index,
  const vector<DgEdge>& common_list,
  const vector<DgEdge>& rest0_list,
  const vector<DgEdge>& rest1_list,
  bool inv0,
  bool inv1
)
{
  // rest0_list, rest1_list が空の場合でも merge() が
  // 適切に処理してくれる．
  auto tmp0_edge = make_xor(rest0_list) ^ inv0;
  auto tmp1_edge = make_xor(rest1_list) ^ inv1;
  if ( debug ) {
    cout << "[case1_xor] merge at " << index << endl;
    cout << "r0" << endl;
    print(cout, tmp0_edge);
    cout << "r1" << endl;
    print(cout, tmp1_edge);
    cout << endl;
  }
  auto new_edge = merge(index, tmp0_edge, tmp1_edge);
  if ( debug ) {
    cout << "[case1_xor] merge end" << endl;
    cout << "merge at " << index << endl;
    cout << "r0" << endl;
    print(cout, tmp0_edge);
    cout << "r1" << endl;
    print(cout, tmp1_edge);
    cout << endl;
    cout << "  ==> ";
    print(cout, new_edge);
  }

  vector<DgEdge> child_list{new_edge};
  child_list.insert(child_list.end(),
		    common_list.begin(), common_list.end());
  return make_xor(child_list);
}

// @brief Case1-CPLX のマージを行う．
DgEdge
DgMgr::case1_cplx(
  const Bdd& f,
  SizeType index,
  DgNode* node0,
  DgNode* node1,
  const vector<DgEdge>& common_list,
  DgEdge rest0,
  DgEdge rest1
)
{
  if ( debug ) {
    cout << "merge at " << index << endl;
    cout << "r0" << endl;
    print(cout, rest0);
    cout << "r1" << endl;
    print(cout, rest1);
    cout << endl;
  }
  auto new_edge = merge(index, rest0, rest1);
  if ( debug ) {
    cout << "merge end" << endl;
    f.display(cout);
    cout << "merge at " << index << endl;
    cout << "r0" << endl;
    print(cout, rest0);
    cout << "r1" << endl;
    print(cout, rest1);
    cout << endl;
    cout << "  ==> ";
    print(cout, new_edge);
  }
  vector<DgEdge> tmp_list{new_edge};
  tmp_list.insert(tmp_list.end(), common_list.begin(), common_list.end());
  auto result = make_cplx(f, tmp_list);
  return result;
}

// @brief CASE1-CPLX(Type 2) のマージを行う．
DgEdge
DgMgr::case1_cplx2(
  const Bdd& f,
  SizeType index,
  const vector<DgEdge>& child_list,
  SizeType pos
)
{
  // child_list の pos 番目の子ノードと index との XOR を作る．
  auto chd0 = child_list[pos];
  auto tmp_edge = make_lit_xor(index, false, chd0);
  vector<DgEdge> tmp_list{child_list};
  tmp_list[pos] = tmp_edge;
  auto result = make_cplx(f, tmp_list);
  return result;
}

// @brief CASE2-OR のマージを行う．
DgEdge
DgMgr::case2_or(
  SizeType index,
  bool lit_inv,
  DgNode* node,
  bool oinv,
  SizeType pos
)
{
  // pos 以外の子ノードのORノードを作る．
  vector<DgEdge> tmp_list;
  SizeType n = node->child_num();
  tmp_list.reserve(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    if ( i != pos ) {
      tmp_list.push_back(node->child(i));
    }
  }
  auto tmp_edge = make_or(tmp_list);
  // それと index とのANDを作る．
  auto new_edge = make_lit_and(index, lit_inv, tmp_edge);
  // それと r1 との ORを作る．
  auto r1 = node->child(pos);
  auto result = make_or({new_edge, r1}) ^ oinv;
  return result;
}

// @brief CASE2-XOR のマージを行う．
DgEdge
DgMgr::case2_xor(
  SizeType index,
  bool lit_inv,
  DgNode* node,
  bool oinv,
  SizeType pos,
  bool inv1
)
{
  if ( debug ) {
    cout << "case2_xor" << endl;
  }
  // pos 以外の子ノードのXORノードを作る．
  vector<DgEdge> tmp_list;
  SizeType n = node->child_num();
  tmp_list.reserve(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    if ( i != pos ) {
      tmp_list.push_back(node->child(i));
    }
  }
  auto tmp_edge = make_xor(tmp_list);
  if ( debug ) {
    cout << "tmp_edge" << endl;
    print(cout, tmp_edge);
  }
  tmp_edge ^= oinv;
  // それと index との AND/OR を作る．
  DgEdge new_edge;
  if ( inv1 ) {
    new_edge = make_lit_or(index, !lit_inv, tmp_edge);
  }
  else {
    new_edge = make_lit_and(index, lit_inv, tmp_edge);
  }
  // それと r1 との XOR を作る．
  auto r1 = node->child(pos);
  auto result = make_xor({new_edge, r1});
  return result;
}

// @brief CASE2-CPLX のマージを行う．
DgEdge
DgMgr::case2_cplx(
  const Bdd& f,
  SizeType index,
  DgEdge cedge,
  bool lit_inv,
  bool is_and,
  DgNode* node
)
{
  DgEdge new_edge;
  if ( is_and ) {
    new_edge = make_lit_and(index, lit_inv, cedge);
  }
  else {
    new_edge = make_lit_or(index, lit_inv, cedge);
  }

  SizeType nc = node->child_num();
  vector<DgEdge> tmp_list(nc);
  for ( SizeType i = 0; i < nc; ++ i ) {
    auto chd = node->child(i);
    if ( chd == cedge ) {
      tmp_list[i] = new_edge;
    }
    else {
      tmp_list[i] = chd;
    }
  }
  auto result = make_cplx(f, tmp_list);
  return result;
}

// @brief LITノードを作る．
DgEdge
DgMgr::make_lit(
  SizeType index
)
{
  auto f = mBddMgr.posi_literal(VarId{index});
  DgEdge result;
  if ( find_node(f, result) ) {
    return result;
  }
  auto sup = f.get_support();
  SizeType id = mNodeList.size();
  auto node = new DgLitNode{id, f, sup};
  mNodeList.push_back(node);
  result = DgEdge{node};
  put_node(f, result);
  return result;
}

// @brief ORノードを作る．
DgEdge
DgMgr::make_or(
  const vector<DgEdge>& child_list
)
{
  if ( child_list.empty() ) {
    // 空の場合は定数0を返す．
    return DgEdge::zero();
  }
  if ( child_list.size() == 1 ) {
    // 子供が1人ならその子供を返す．
    return child_list[0];
  }

  if ( debug ) {
    cout << "make_or begin" << endl;
    for ( auto child: child_list ) {
      print(cout, child);
      cout << "===" << endl;
    }
  }
  // グローバル関数を作る．
  auto f = child_list[0].global_func();
  for ( SizeType i = 1; i < child_list.size(); ++ i ) {
    f |= child_list[i].global_func();
  }
  // 等価なノードがあるか調べる．
  DgEdge result;
  if ( !find_node(f, result) ) {
    // child_list の中で肯定のORノードがあれば
    // その子ノードを本当の子ノードにする．
    vector<DgEdge> tmp_list;
    BddVarSet support{mBddMgr};
    for ( auto child: child_list ) {
      if ( child.inv() == false && child.node()->is_or() ) {
	for ( SizeType i = 0; i < child.node()->child_num(); ++ i ) {
	  tmp_list.push_back(child.node()->child(i));
	}
      }
      else {
	tmp_list.push_back(child);
      }
      support += child.node()->support();
    }
    // top の昇順になるようにソートする．
    sort(tmp_list.begin(), tmp_list.end(),
	 [](DgEdge e1, DgEdge e2)
	 { return e1.node()->top() < e2.node()->top(); });
    SizeType id = mNodeList.size();
    auto node = new DgOrNode(id, f, support, tmp_list);
    mNodeList.push_back(node);
    result = DgEdge{node};
    put_node(f, result);
  }
  return result;
}

// @brief XORノードを作る．
DgEdge
DgMgr::make_xor(
  const vector<DgEdge>& child_list
)
{
  if ( child_list.empty() ) {
    // 空の場合は定数0を返す．
    return DgEdge::zero();
  }
  if ( child_list.size() == 1 ) {
    // 子供が1人ならその子供を返す．
    return child_list[0];
  }

  if ( debug ) {
    cout << "make_xor begin" << endl;
    for ( auto child: child_list ) {
      print(cout, child);
      cout << "===" << endl;
    }
  }
  // グローバル関数を作る．
  auto f = child_list[0].global_func();
  for ( SizeType i = 1; i < child_list.size(); ++ i ) {
    f ^= child_list[i].global_func();
  }
  // 等価なノードがあるか調べる．
  DgEdge result;
  if ( !find_node(f, result) ) {
    // child_list の中でXORノードがあれば
    // その子ノードを本当の子ノードにする．
    // ORの場合と異なり極性は問わない．
    // ただし，XORノードの子供には反転属性をつけない．
    // 極性は根に移す．
    vector<DgEdge> tmp_list;
    BddVarSet support{mBddMgr};
    bool oinv = false;
    for ( auto child: child_list ) {
      bool inv = child.inv();
      oinv ^= inv;
      if ( child.node()->is_xor() ) {
	for ( SizeType i = 0; i < child.node()->child_num(); ++ i ) {
	  auto child1 = child.node()->child(i);
	  tmp_list.push_back(child1.normal_edge());
	  oinv ^= child1.inv();
	}
      }
      else {
	tmp_list.push_back(child.normal_edge());
      }
      support += child.node()->support();
    }
    // top の昇順になるようにソートする．
    sort(tmp_list.begin(), tmp_list.end(),
	 [](DgEdge e1, DgEdge e2)
	 { return e1.node()->top() < e2.node()->top(); });
    auto f_normal = f ^ oinv;
    SizeType id = mNodeList.size();
    auto node = new DgXorNode(id, f_normal, support, tmp_list);
    mNodeList.push_back(node);
    result = DgEdge{node, false};
    put_node(f_normal, result);
    result ^= oinv;
  }
  return result;
}

// @brief LITノードとのANDを作る．
DgEdge
DgMgr::make_lit_and(
  SizeType index,
  bool inv,
  DgEdge e
)
{
  auto lit = make_lit(index) ^ inv;
  return ~make_or({~lit, ~e});
}

// @brief LITノードとのORを作る．
DgEdge
DgMgr::make_lit_or(
  SizeType index,
  bool inv,
  DgEdge e
)
{
  auto lit = make_lit(index) ^ inv;
  return make_or({lit, e});
}

// @brief LITノードとのXORを作る．
DgEdge
DgMgr::make_lit_xor(
  SizeType index,
  bool inv,
  DgEdge e
)
{
  auto lit = make_lit(index) ^ inv;
  return make_xor({lit, e});
}

// @brief CPLXノードを作る．
DgEdge
DgMgr::make_cplx(
  const Bdd& f,
  const vector<DgEdge>& child_list
)
{
  bool oinv = f.root_inv();
  auto f_normal = f ^ oinv;
  DgEdge result;
  if ( !find_node(f_normal, result) ) {
    auto support = f_normal.get_support();
    vector<DgEdge> tmp_list;
    tmp_list.reserve(child_list.size());
    for ( auto edge: child_list ) {
      tmp_list.push_back(edge.normal_edge());
    }
    sort(tmp_list.begin(), tmp_list.end(),
	 [](DgEdge e1, DgEdge e2)
	 { return e1.node()->top() < e2.node()->top(); });
    SizeType id = mNodeList.size();
    auto node = new DgCplxNode{id, f_normal, support, tmp_list};
    mNodeList.push_back(node);
    result = DgEdge{node, false};
    put_node(f_normal, result);
  }
  return result ^ oinv;
}

// @brief 関数に対応する DgEdge を探す．
bool
DgMgr::find_node(
  const Bdd& f,
  DgEdge& result
) const
{
  auto inv = f.root_inv();
  auto f_normal = f ^ inv;
  if ( mEdgeDict.count(f_normal) == 0 ) {
    return false;
  }
  else {
    result = mEdgeDict.at(f_normal) ^ inv;
    return true;
  }
}

// @brief 関数に対応する DgEdge を登録する．
void
DgMgr::put_node(
  const Bdd& f,
  DgEdge result
)
{
  auto inv = f.root_inv();
  auto f_normal = f ^ inv;
  result ^= inv;
  if ( debug ) {
    cout << "put_node" << endl;
    f_normal.display(cout);
    print(cout, result);
  }

  if ( mEdgeDict.count(f_normal) > 0 ) {
    ASSERT_COND( mEdgeDict.at(f_normal) == result );
    return;
  }

  mEdgeDict.emplace(f_normal, result);
}

END_NAMESPACE_DG
