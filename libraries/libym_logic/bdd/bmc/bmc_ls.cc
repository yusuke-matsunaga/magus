
/// @file bmc_ls.cc
/// @brief リテラル集合を扱う関数の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "BddMgrClassic.h"


BEGIN_NAMESPACE_YM_BDD

BEGIN_NONAMESPACE

// 比較用の関数オブジェクト
struct LitCompLess
{
  bool
  operator()(const Literal& lit1,
	     const Literal& lit2) const
  {
    if ( lit1.varid() < lit2.varid() ) {
      return true;
    }
    if ( lit1.varid() > lit2.varid() ) {
      return false;
    }
    if ( lit1.pol() == kPolPosi && lit2.pol() == kPolNega ) {
      return true;
    }
    return false;
  }
};

// BddEdge::make_zero() でない方の枝を選ぶ．
// 0枝が選ばれた場合には which = 0
// 1枝が選ばれた場合には which = 1 とする．
inline
BddEdge
select_edge(BddNode* node,
	    tPol pol,
	    int& which)
{
  BddEdge e0 = node->edge0(pol);
  BddEdge e1 = node->edge1(pol);
  if ( e0.is_zero() ) {
    which = 1;
    return e1;
  }
  else {
    which = 0;
    return e0;
  }
}

END_NONAMESPACE

// 2つのリテラル集合の集合積を求める．
// メモリ不足のためにエラーとなる可能性がある．
BddEdge
BddMgrClassic::lscap(BddEdge e1,
		     BddEdge e2)
{
  if ( e1.is_error() || e2.is_error() ) {
    return BddEdge::make_error();
  }
  if ( e1.is_overflow() || e2.is_overflow() ) {
    return BddEdge::make_overflow();
  }
  if ( e1.is_zero() || e2.is_zero() ) {
    return BddEdge::make_error();
  }
  if ( e1.is_one() || e2.is_one() ) {
    return BddEdge::make_one();
  }

  BddNode* node1 = e1.get_node();
  BddNode* node2 = e2.get_node();
  tPol pol1 = e1.pol();
  tPol pol2 = e2.pol();
  ymuint level1 = node1->level();
  ymuint level2 = node2->level();
  for ( ; ; ) {
    if ( level1 == level2 ) {
      int which1;
      e1 = select_edge(node1, pol1, which1);
      int which2;
      e2 = select_edge(node2, pol2, which2);
      if ( which1 == which2 ) {
	BddEdge tmp = lscap(e1, e2);
	if ( which1 == 0 ) {
	  return new_node(level1, tmp, BddEdge::make_zero());
	}
	else {
	  return new_node(level1, BddEdge::make_zero(), tmp);
	}
      }
      if ( e1.is_one() || e2.is_one() ) {
	return BddEdge::make_one();
      }
      node1 = e1.get_node();
      node2 = e2.get_node();
      pol1 = e1.pol();
      pol2 = e2.pol();
      level1 = node1->level();
      level2 = node2->level();
    }
    else if ( level1 < level2 ) {
      int which1;
      e1 = select_edge(node1, pol1, which1);
      if ( e1.is_one() ) {
	return BddEdge::make_one();
      }
      node1 = e1.get_node();
      pol1 = e1.pol();
      level1 = node1->level();
    }
    else { // level1 > level2
      int which2;
      e2 = select_edge(node2, pol2, which2);
      if ( e2.is_one() ) {
	return BddEdge::make_one();
      }
      node2 = e2.get_node();
      pol2 = e2.pol();
      level2 = node2->level();
    }
  }
}

// e1 のリテラル集合から e2 のリテラル集合を引く．(集合差演算)
// 同じ変数番号であっても極性が異なれば異なる要素とみなす．
// lsdiff() のためのサブルーティン
BddEdge
BddMgrClassic::lsdiff(BddEdge e1,
		      BddEdge e2)
{
  if ( e1.is_error() || e2.is_error() ) {
    return BddEdge::make_error();
  }
  if ( e1.is_overflow() || e2.is_overflow() ) {
    return BddEdge::make_overflow();
  }
  if ( e1.is_zero() || e2.is_zero() ) {
    return BddEdge::make_error();
  }
  if ( e1.is_one() || e2.is_one() ) {
    return e1;
  }

  BddNode* node1 = e1.get_node();
  BddNode* node2 = e2.get_node();
  tPol pol1 = e1.pol();
  tPol pol2 = e2.pol();
  ymuint level1 = node1->level();
  ymuint level2 = node2->level();
  for ( ; ; ) {
    if ( level1 < level2 ) {
      int which1;
      e1 = select_edge(node1, pol1, which1);
      BddEdge tmp = lsdiff(e1, e2);
      if ( which1 == 0 ) {
	return new_node(level1, tmp, BddEdge::make_zero());
      }
      else {
	return new_node(level1, BddEdge::make_zero(), tmp);
      }
    }
    if ( level1 > level2 ) {
      int which2;
      e2 = select_edge(node2, pol2, which2);
      if ( e2.is_one() ) {
	return e1;
      }
      node2 = e2.get_node();
      pol2 = e2.pol();
      level2 = node2->level();
    }
    else {
      int which1;
      e1 = select_edge(node1, pol1, which1);
      int which2;
      e2 = select_edge(node2, pol2, which2);
      if ( which1 != which2 ) {
	BddEdge tmp = lsdiff(e1, e2);
	if ( which1 == 0 ) {
	  return new_node(level1, tmp, BddEdge::make_zero());
	}
	else {
	  return new_node(level1, BddEdge::make_zero(), tmp);
	}
      }
      if ( e1.is_one() || e2.is_one() ) {
	return e1;
      }
      node1 = e1.get_node();
      node2 = e2.get_node();
      pol1 = e1.pol();
      pol2 = e2.pol();
      level1 = node1->level();
      level2 = node2->level();
    }
  }
}

// e1 の変数集合と e2 の変数集合が共通部分を持っていたら true を返す．
bool
BddMgrClassic::lsintersect(BddEdge e1,
			   BddEdge e2)
{
  if ( e1.is_invalid() || e2.is_invalid() ) {
    return false;
  }
  if ( e1.is_zero() || e2.is_zero() ) {
    return false;
  }
  if ( e1.is_one() || e2.is_one() ) {
    return false;
  }

  BddNode* node1 = e1.get_node();
  BddNode* node2 = e2.get_node();
  tPol pol1 = e1.pol();
  tPol pol2 = e2.pol();
  ymuint level1 = node1->level();
  ymuint level2 = node2->level();
  for ( ; ; ) {
    if ( level1 == level2 ) {
      int which1;
      e1 = select_edge(node1, pol1, which1);
      int which2;
      e2 = select_edge(node2, pol2, which2);
      if ( which1 == which2 ) {
	return true;
      }
      if ( e1.is_one() || e2.is_one() ) {
	return false;
      }
      node1 = e1.get_node();
      node2 = e2.get_node();
      pol1 = e1.pol();
      pol2 = e2.pol();
      level1 = node1->level();
      level2 = node2->level();
    }
    else if ( level1 < level2 ) {
      int which1;
      e1 = select_edge(node1, pol1, which1);
      if ( e1.is_one() ) {
	return false;
      }
      node1 = e1.get_node();
      pol1 = e1.pol();
      level1 = node1->level();
    }
    else {
      int which2;
      e2 = select_edge(node2, pol2, which2);
      if ( e2.is_one() ) {
	return false;
      }
      node2 = e2.get_node();
      pol2 = e2.pol();
      level2 = node2->level();
    }
  }
}

// リテラルの vector に変換する．
ymuint
BddMgrClassic::to_literalvector(BddEdge e,
				LiteralVector& dst)
{
  dst.clear();

  if ( e.is_invalid() ) {
    return 0;
  }

  ymuint n = node_count(vector<BddEdge>(1, e));
  dst.reserve(n);

  BddNode* vp = e.get_node();
  tPol pol = e.pol();
  while ( vp ) {
    ymuint level = vp->level();
    VarId varid(level);
    BddEdge e0 = vp->edge0(pol);
    BddEdge e1 = vp->edge1(pol);
    if ( e0 == BddEdge::make_zero() ) {
      dst.push_back(Literal(varid, kPolPosi));
      vp = e1.get_node();
      pol = e1.pol();
    }
    else {
      dst.push_back(Literal(varid, kPolNega));
      vp = e0.get_node();
      pol = e0.pol();
    }
  }
  sort(dst.begin(), dst.begin(), LitCompLess());

  return n;
}

// リテラルの list に変換する．
ymuint
BddMgrClassic::to_literallist(BddEdge e,
			      LiteralList& dst)
{
  dst.clear();

  if ( e.is_invalid() ) {
    return 0;
  }

  BddNode* vp = e.get_node();
  tPol pol = e.pol();
  while ( vp ) {
    ymuint level = vp->level();
    VarId varid(level);
    BddEdge e0 = vp->edge0(pol);
    BddEdge e1 = vp->edge1(pol);
    if ( e0 == BddEdge::make_zero() ) {
      dst.push_back(Literal(varid, kPolPosi));
      vp = e1.get_node();
      pol = e1.pol();
    }
    else {
      dst.push_back(Literal(varid, kPolNega));
      vp = e0.get_node();
      pol = e0.pol();
    }
  }
  dst.sort(LitCompLess());

  return dst.size();
}

END_NAMESPACE_YM_BDD
