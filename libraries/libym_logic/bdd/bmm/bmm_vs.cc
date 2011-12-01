
/// @file bmm_vs.cc
/// @brief 変数集合を扱う関数の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "BddMgrModern.h"


BEGIN_NAMESPACE_YM_BDD

// 2つの変数集合の集合積を求める．
// メモリ不足のためにエラーとなる可能性がある．
BddEdge
BddMgrModern::vscap(BddEdge e1,
		    BddEdge e2)
{
  if ( e1.is_error() || e2.is_error() ) {
    return BddEdge::make_error();
  }
  if ( e1.is_overflow() || e2.is_overflow() ) {
    return BddEdge::make_overflow();
  }
  if ( e1.is_one() || e2.is_one() ) {
    return BddEdge::make_one();
  }
  if ( e1.is_zero() || e2.is_zero() ) {
    return BddEdge::make_error();
  }

  Node* node1 = get_node(e1);
  Node* node2 = get_node(e2);
  Var* var1 = node1->var();
  Var* var2 = node2->var();
  ymuint level1 = var1->level();
  ymuint level2 = var2->level();
  for ( ; ; ) {
    if ( level1 == level2 ) {
      BddEdge tmp = vscap(node1->edge1(), node2->edge1());
      return new_node(var1, BddEdge::make_zero(), tmp);
    }
    else if ( level1 < level2 ) {
      e1 = node1->edge1();
      if ( e1.is_one() ) {
	return BddEdge::make_one();
      }
      node1 = get_node(e1);
      var1 = node1->var();
      level1 = var1->level();
    }
    else { // level1 > level2
      e2 = node2->edge1();
      if ( e2.is_one() ) {
	return BddEdge::make_one();
      }
      node2 = get_node(e2);
      var2 = node2->var();
      level2 = var2->level();
    }
  }
}

// e1 の変数集合から e2 の変数集合を引く．(集合差演算)
BddEdge
BddMgrModern::vsdiff(BddEdge e1,
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
  if ( e1.is_one() ) {
    return BddEdge::make_one();
  }
  if ( e2.is_one() ) {
    return e1;
  }

  Node* node1 = get_node(e1);
  Node* node2 = get_node(e2);
  Var* var1 = node1->var();
  Var* var2 = node2->var();
  ymuint level1 = var1->level();
  ymuint level2 = var2->level();
  for ( ; ; ) {
    if ( level1 < level2 ) {
      BddEdge tmp = vsdiff(node1->edge1(), e2);
      return new_node(var1, BddEdge::make_zero(), tmp);
    }
    if ( level1 > level2 ) {
      e2 = node2->edge1();
      if ( e2 == BddEdge::make_one() ) {
	return e1;
      }
      node2 = get_node(e2);
      var2 = node2->var();
      level2 = var2->level();
    }
    else {
      e1 = node1->edge1();
      e2 = node2->edge1();
      if ( e1.is_one() ) {
	return BddEdge::make_one();
      }
      if ( e2.is_one() ) {
	return e1;
      }
      node1 = get_node(e1);
      node2 = get_node(e2);
      var1 = node1->var();
      var2 = node2->var();
      level1 = var1->level();
      level2 = var2->level();
    }
  }
}

// e1 の変数集合と e2 の変数集合が共通部分を持っていたら true を返す．
bool
BddMgrModern::vsintersect(BddEdge e1,
			  BddEdge e2)
{
  if ( e1.is_error() || e2.is_error() ) {
    return false;
  }
  if ( e1.is_overflow() || e2.is_overflow() ) {
    return false;
  }
  if ( e1.is_zero() || e2.is_zero() ) {
    return false;
  }
  if ( e1.is_one() || e2.is_one() ) {
    return false;
  }

  Node* node1 = get_node(e1);
  Node* node2 = get_node(e2);
  ymuint level1 = node1->level();
  ymuint level2 = node2->level();
  for ( ; ; ) {
    if ( level1 == level2 ) {
      return true;
    }
    if ( level1 < level2 ) {
      e1 = node1->edge1();
      if ( e1.is_one() ) {
	return false;
      }
      node1 = get_node(e1);
      level1 = node1->level();
    }
    else {
      e2 = node2->edge1();
      if ( e2.is_one() ) {
	return false;
      }
      node2 = get_node(e2);
      level2 = node2->level();
    }
  }
}

END_NAMESPACE_YM_BDD
