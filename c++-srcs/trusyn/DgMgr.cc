
/// @file DgMgr.cc
/// @brief DgMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "DgMgr.h"


BEGIN_NAMESPACE_TRUSYN

// @brief コンストラクタ
DgMgr::DgMgr()
{
}

// @brief デストラクタ
DgMgr::~DgMgr()
{
}

// @brief decomp の下請け関数
DgEdge
DgMgr::decomp_step(
  SizeType top,
  const TvFunc& func
)
{
  if ( func.is_zero() ) {
    DgEdge::make_zero();
  }
  if ( func.is_one() ) {
    DgEdge::make_one();
  }

  DgEdge result;
  // 登録済みかどうか調べる．
  if ( !find_node(func, result) ) {
    // 先頭の変数を求める．
    while ( !func.check_sup(VarId{toop}) ) {
      // 上で定数チェックを行っているので
      // 最低1つはサポート変数があるはず．
      ++ top;
    }
    // コファクターに対して再帰的に処理を行う．
    auto r0 = decomp_step(top + 1, func.cofactor(VarId{top}, true));
    auto r1 = decomp_step(top + 1, func.cofactor(VarId{top), false));
    // 結果をマージする．
    result = merge(top, r0, r1);
    // 結果を登録する．
    put_node(func, result);
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
      return make_lit(top);
    }
    else {
      // リテラルとのAND ( x & r1 )
      return make_lit_and(top, false, r1);
    }
  }
  else if ( r0.is_one() ) {
    if ( r1.is_zero() ) {
      // 否定のリテラル
      return make_lit(top) * true;
    }
    else {
      // リテラルとのOR ( ~x | r1 )
      return make_lit_or(top, true, r1);
    }
  }
  else if ( r1.is_zero() ) {
    // リテラルとのAND ( ~x & r0 )
    return make_lit_and(top, true, r0);
  }
  else if ( r1.is_one() ) {
    // リテラルとのOR ( x | r0 )
    return make_lit_or(top, false, r0);
  }
  else if ( DgEdge::check_complement(r0, r1) ) {
    // リテラルとのXOR ( ~x & r0 | x & ~r0 )
    return make_lit_xor(top, false, r0);
  }

  // ここまで来たらどちらも定数ではない．
  bool inv0 = r0.inv();
  bool inv1 = r1.inv();
  DgNode* node0 = r0.node();
  DgNode* node1 = i1.node();
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
  }

  if ( common_list.size() >= 1 ) {
    // Case1-OR
    if ( inv0 == inv1 && node0->is_or() && node1->is_or() ) {
      return case1_or(top, common_list, rest0_list, rest1_list) * inv0;
    }

    // Case1-XOR
    if ( node0->is_xor() && node1->is_xor() ) {
      return case1_xor(top, common_list, rest0_list, rest1_list, inv0, inv1);
    }
  }

  // Case2-OR
  if ( node0->is_or() ) {
    for ( SizeType i = 0; i < nc0; ++ i ) {
      if ( node0->child(i) * inv0 == r1 ) {
	return case2_or(top, true, node0, inv0, i);
      }
    }
  }
  if ( node1->is_or() ) {
    for ( SizeType i = 0; i < nc1; ++ i ) {
      if ( node1->child(i) * inv1 == r0 ) {
	return case2_or(top, false, node1, inv1, i);
      }
    }
  }

  // Case2-XOR
  if ( node0->is_xor() ) {
    for ( SizeType i = 0; i < nc0; ++ i ) {
      if ( node0->child(i).node() == node1 ) {
	return case2_xor(top, true, node0, inv0, i, node1, inv1);
      }
    }
  }
  if ( node1->is_xor() ) {
    for ( SizeType i = 0; i < nc1; ++ i ) {
      if ( node1->child(i).node() == node0 ) {
	return case2_xor(top, false, node1, inv1, i, node0, inv0);
      }
    }
  }

  TvFunc f0 = r0.global_func();
  TvFunc f1 = r1.global_func();

  if ( node0->is_cplx() && node1->is_cplx() ) {
    if ( rest0_list.size() == 1 && rest1_list.size() == 1 ) {
      // r0, r1 が共に CPLX でただ一つの子ノード以外共通の場合
      auto r0_1 = rest0_list[0];
      auto r1_1 = rest1_list[0];

      // 根の関数が等しいか調べる．
      // f0 に対して r0_1 が 0 に束縛した関数と
      // f1 に対して r1_1 を 0 に束縛した関数が等価かどうか調べる．

      // f0 に対して r0_1 が 1 に束縛した関数と
      // f1 に対して r1_1 を 1 に束縛した関数が等価かどうか調べる．

      { // 一致した．
	return case1_cplx(top, false, node0, node1, common_list, r0_1, r1_1);
      }

      // f0 に対して r0_1 が 0 に束縛した関数と
      // f1 に対して r1_1 を 1 に束縛した関数が等価かどうか調べる．

      // f0 に対して r0_1 が 1 に束縛した関数と
      // f1 に対して r1_1 を 0 に束縛した関数が等価かどうか調べる．
      { // 一致した．
	return case1_cplx(top, true, node0, node1, common_list, r0_1, r1_1);
      }
    }
    else if ( rest0_list.empty() && rest1_list.empty() ) {
      // どれかの子ノードを反転すると一致する場合
      { // 一致した．
	SizeType pos;
	return case2_cplx(top, node0, node1, pos);
      }
    }
  }

  auto sup0 = node0->sup_list();
  auto sup1 = node1->sup_list();
  if ( node0->is_cplx() && sup0_diff.size() > 0 && sup1_diff.empty() ) {
    for ( SizeType i = 0; i < nc0; ++ i ) {
      auto cnode = node0->child(i);
      auto isup = cnode->sup_list();
      if ( isup /*does not intersect*/ sup1 ) {
	// f0 に対して cnode が 0 になる割当を行うと f1 と一致する場合
	{
	  return case2_cplx(top, cnode, true, true, node0);
	}
	// f0 に対して cnode が 1 になる割当を行うと f1 と一致する場合
	{
	  return case2_cplx(top, cnode, false, false, node0);
	}
      }
    }
  }
  if ( node1->is_cplx() && sup1_diff.size() > 0 && sup0_diff.empty() ) {
    for ( SizeType i = 0; i < nc1; ++ i ) {
      auto cnode = node1->child(i);
      auto isup = cnode->sup_list();
      if ( isup /*does not intersect*/ sup1 ) {
	// f1 に対して cnode が 0 になる割当を行うと f0 と一致する場合
	{
	  return case2_cplx(top, cnode, false, true, node1);
	}
	// f1 に対して cnode が 1 になる割当を行うと f0 と一致する場合
	{
	  return case2_cplx(top, cnode, true, false, node1);
	}
      }
    }
  }

  if ( node0->is_all_lit() && node1->is_all_lit() ) {
    // どちらも単純な CPLX の場合，結果も単純な CPLX になる．
    return make_simple_cplx(f);
  }

  // node0 と node1 の推移的な子ノードのうち，共通なノードを求める．
  // それらを新しい子供とした CPLX ノードを作る．
  // 面倒なのは OR/XOR ノードが極大化されているということ．
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
  auto new_edge = merge(index, tmp0_edge, tmp_edge);

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
  auto tmp0_edge = make_or(rest0_list) * inv0;
  auto tmp1_edge = make_or(rest1_list) * inv1;
  auto new_edge = merge(index, tmp0_edge, tmp_edge);

  vector<DgEdge> child_list{new_edge};
  child_list.insert(child_list.end(),
		    common_list.begin(), common_list.end());
  return make_xor(child_list);
}

// @brief Case1-CPLX のマージを行う．
DgEdge
DgMgr::case1_cplx(
  SizeType index,
  bool inv,
  DgNode* node0,
  DgNode* node1,
  const vector<DgEdge>& common_list,
  DgEdge rest0,
  DgEdge rest1
)
{
  auto new_edge = merge(index, rest0, rest1);
  vector<DgEdge> tmp_list{new_edge};
  tmp_list.insert(tmp_list.end(), common_list.begin(), common_list.end());
  auto result = make_cplx(f, tmp_list);
  return result;
}

// @brief CASE2-OR のマージを行う．
DgEdge
DgMgr::case2_or(
  SizeType index,
  bool inv,
  DgNode* node0,
  bool inv0,
  SizeType pos
)
{
  // pos 以外の子ノードのORノードを作る．
  vector<DgEdge> tmp_list;
  tmp_list.reserve(node0->child_num());
  for ( SizeType i = 0; i < node0->child_num(); ++ i ) {
    if ( i != pos ) {
      tmp_list.push_back(node0->child(i));
    }
  }
  auto tmp_edge = make_or(tmp_list);
  // それと index とのANDを作る．
  auto new_edge = make_lit_and(index, inv, tmp_edge);
  // それと r1 との ORを作る．
  auto r1 = node0->child(pos) * inv0;
  auto result = make_or({new_edge, r1}) * inv0;
  return result;
}

// @brief CASE2-XOR のマージを行う．
DgEdge
DgMgr::case2_xor(
  SizeType index,
  bool inv,
  DgNode* node0,
  bool inv0,
  SizeType pos,
  bool inv1
)
{
  // pos 以外の子ノードのXORノードを作る．
  vector<DgEdge> tmp_list;
  tmp_list.reserve(node0->child_num());
  for ( SizeType i = 0; i < node0->child_num(); ++ i ) {
    if ( i != pos ) {
      tmp_list.push_back(node0->child(i));
    }
  }
  auto tmp_edge = make_xor(tmp_list);
  // それと index との AND/OR を作る．
  DgEdge new_edge;
  if ( inv1 ) {
    new_edge = make_lit_or(index, !inv, tmp_edge);
  }
  else {
    new_edge = make_lit_and(index, inv, tmp_edge);
  }
  // それと r1 との XOR を作る．
  auto r1 = node0->child(pos);
  auto result = make_xor({new_edge, r1});
  return result;
}

// @brief LITノードを作る．
DgEdge
DgMgr::make_lit(
  SizeType index
)
{
  auto node = new DgLitNode{index};
  return DgEdge{node};
}

BEGIN_NONAMESPACE

// 共に昇順に整列されているリストをマージする．
vector<SizeType>
merge_sup(
  const vector<SizeType>& list1,
  const vector<SizeType>& list2
)
{
  SizeType n1 = list1.size();
  SizeType n2 = list2.size();
  vector<SizeType> ans_list;
  ans_list.reserve(n1 + n2);
  SizeType i1 = 0;
  SizeType i2 = 0;
  while ( i1 < n1 && i2 < n2 ) {
    SizeType v1 = list1[i1];
    SizeType v2 = list2[i2];
    if ( v1 < v2 ) {
      ans_list.push_back(v1);
      ++ i1;
    }
    else if ( v1 > v2 ) {
      ans_list.push_back(v2);
      ++ i2;
    }
    else { // v1 == v2
      ans_list.push_back(v1);
      ++ i1;
      ++ i2;
    }
  }
  while ( i1 < n1 ) {
    SizeType v1 = list1[i1];
    ans_list.push_back(v1);
    ++ i1;
  }
  while ( i2 < n2 ) {
    SizeType v2 = list2[i2];
    ans_list.push_back(v2);
    ++ i2;
  }
  return ans_list;
}

END_NONAMESPACE

// @brief ORノードを作る．
DgEdge
DgMgr::make_or(
  const vector<DgEdge>& child_list
)
{
  if ( child_list.empty() ) {
    return DgEdge::make_zero();
  }
  if ( child_list.size() == 1 ) {
    return child_list[0];
  }

  // グローバル関数を作る．
  TvFunc f = child_list[0].global_func();
  for ( SizeType i = 1; i < child_list.size(); ++ i ) {
    f |= child_list[i].global_func();
  }
  // 等価なノードがあるか調べる．
  DgEdge result;
  if ( !find_node(f, result) ) {
    // child_list の中で肯定のORノードがあれば
    // その子ノードを本当の子ノードにする．
    vector<DgEdge> tmp_list;
    vector<SizeType> sup_list;
    for ( auto child: child_list ) {
      if ( child.inv() == false && child.node()->is_or() ) {
	for ( SizeType i = 0; i < child.node()->child_num(); ++ i ) {
	  tmp_list.push_back(child.node()->child(i));
	}
      }
      else {
	tmp_list.push_back(child);
      }
      sup_list = merge_sup(sup_list, child.node()->sup_list());
    }
    // top の昇順になるようにソートする．
    sort(tmp_list.begin(), tmp_list.end(),
	 [](DgEdge e1, DgEdge e2)
	 { return e1.node()->top() < e2.node()->top(); });
    auto node = new DgOrNode(f, sup_list, tmp_list);
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
    return DgEdge::make_zero();
  }
  if ( child_list.size() == 1 ) {
    return child_list[0];
  }

  // グローバル関数を作る．
  TvFunc f = child_list[0].global_func();
  for ( SizeType i = 1; i < child_list.size(); ++ i ) {
    f ^= child_list[i].global_func();
  }
  // 等価なノードがあるか調べる．
  DgEdge result;
  if ( !find_node(f, result) ) {
    // child_list の中でXORノードがあれば
    // その子ノードを本当の子ノードにする．
    // ORの場合と異なり極性は問わない．
    vector<DgEdge> tmp_list;
    vector<SizeType> sup_list;
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
      sup_list = merge_sup(sup_list, child.node()->sup_list());
    }
    // top の昇順になるようにソートする．
    sort(tmp_list.begin(), tmp_list.end(),
	 [](DgEdge e1, DgEdge e2)
	 { return e1.node()->top() < e2.node()->top(); });
    if ( oinv ) {
      f.invert();
    }
    auto node = new DgXorNode(f, sup_list, tmp_list);
    result = DgEdge{node, oinv};
    put_node(f, result);
  }
  return result;
}

// @brief 関数に対応する DgEdge を探す．
bool
DgMgr::find_node(
  const TvFunc& f,
  DgEdge& result
) const
{
  if ( mEdgeDict.count(f) == 0 ) {
    return false;
  }
  else {
    result = mEdgeDict.at(f);
    return true;
  }
}

// @brief 関数に対応する DgEdge を登録する．
void
DgMgr::put_node(
  const TvFunc& f,
  DgEdge result
)
{
  mEdgeDict.emplace(f, result);
  mEdgeDict.emplace(~f, ~result);
}

END_NAMESPACE_TRUSYN
