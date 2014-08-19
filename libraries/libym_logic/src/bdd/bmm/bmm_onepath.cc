﻿
/// @file bmm_onepath.cc
/// @brief パスを求める関数の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "BddMgrModern.h"


BEGIN_NAMESPACE_YM_BDD

// '1' の終端へ至る経路をひとつとってくる．
// 結果はその経路を表す論理積の形のBDDで返される．
BddEdge
BddMgrModern::onepath(BddEdge e)
{
  if ( e.is_error() ) {
    return BddEdge::make_error();
  }
  if ( e.is_overflow() ) {
    return BddEdge::make_overflow();
  }
  if ( e.is_one() ) {
    return BddEdge::make_one();
  }

  BddNode* vp = e.get_node();
  bool inv = e.inv();
  ymuint level = vp->level();
  BddEdge l = vp->edge0(inv);
  BddEdge h = vp->edge1(inv);
  BddEdge result;
  if ( h.is_zero() ) {
    BddEdge chd = onepath(l);
    result = new_node(level, chd, BddEdge::make_zero());
  }
  else {
    BddEdge chd = onepath(h);
    result = new_node(level, BddEdge::make_zero(), chd);
  }
  return result;
}

// 最短の1パスを求める．
// 結果はその経路を表す論理積の形のBDDで返される．
BddEdge
BddMgrModern::shortest_onepath(BddEdge e)
{
  if ( e.is_error() ) {
    return BddEdge::make_error();
  }
  if ( e.is_overflow() ) {
    return BddEdge::make_overflow();
  }

  BddEdgeEdgeMap sp_assoc;
  return sp_step(e, sp_assoc);
}

BddEdge
BddMgrModern::sp_step(BddEdge e,
		      BddEdgeEdgeMap& sp_assoc)
{
  if ( e.is_one() ) {
    return BddEdge::make_one();
  }
  if ( e.is_zero() ) {
    return BddEdge::make_zero();
  }

  BddEdge result;
  BddEdgeEdgeMap::iterator p = sp_assoc.find(e);
  if ( p == sp_assoc.end() ) {
    BddNode* vp = e.get_node();
    bool inv = e.inv();
    BddEdge l = sp_step(vp->edge0(inv), sp_assoc);
    BddEdge h = sp_step(vp->edge1(inv), sp_assoc);
    ymint llen = sp_len(l);
    ymint hlen = sp_len(h);
    if ( hlen != -1 && llen > hlen + 1 ) {
      l = BddEdge::make_zero();
    }
    else if ( llen != -1 && llen < hlen + 1 ) {
      h = BddEdge::make_zero();
    }
    ymuint level = vp->level();
    result = new_node(level, l, h);
    sp_assoc[e] = result;
  }
  else {
    result = p->second;
  }
  return result;
}

ymint
BddMgrModern::sp_len(BddEdge e)
{
  if ( e.is_zero() ) {
    return -1;
  }
  int len = 0;
  while ( !e.is_one() ) {
    BddNode* vp = e.get_node();
    bool inv = e.inv();
    BddEdge l = vp->edge0(inv);
    if ( l.is_zero() ) {
      len ++;
      e = vp->edge1(inv);
    }
    else {
      e = l;
    }
  }
  return len;
}

// 最短1パスの長さを求める．
// 演算結果テーブルを拡張しようとしてエラーとなる可能性がある．
ymuint
BddMgrModern::shortest_onepath_len(BddEdge e)
{
  if ( e.is_invalid() ) {
    return 0;
  }
  BddEdgeIntMap assoc;
  ymint tmp = spl_step(e, assoc);
  assert_cond(tmp >= 0, __FILE__, __LINE__);
  return static_cast<ymuint>(tmp);
}

ymint
BddMgrModern::spl_step(BddEdge e,
		       BddEdgeIntMap& assoc)
{
  if ( e.is_one() ) {
    return 0;
  }
  if ( e.is_zero() ) {
    return -1;
  }

  ymint result;
  BddEdgeIntMap::iterator p = assoc.find(e);
  if ( p == assoc.end() ) {
    BddNode* vp = e.get_node();
    bool inv = e.inv();
    ymint ans1 = spl_step(vp->edge0(inv), assoc);
    ymint ans2 = spl_step(vp->edge1(inv), assoc) + 1;
    if ( ans1 != -1 && ans1 < ans2 ) {
      result = ans1;
    }
    else {
      result = ans2;
    }
    assoc[e] = result;
  }
  else {
    result = p->second;
  }
  return result;
}

END_NAMESPACE_YM_BDD
