
/// @file SmoothOp.cc
/// @brief SmoothOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "SmoothOp.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
// クラス SmoothOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr マネージャ
// @param[in] and_op AND 演算オブジェクト
SmoothOp::SmoothOp(BddMgrImpl* mgr,
		   BddBinOp* and_op) :
  BddBinOp(mgr, "smooth_op"),
  mAndOp(and_op)
{
}

// @brief デストラクタ
SmoothOp::~SmoothOp()
{
}

// @brief 演算を行う関数
// @param[in] e 根の枝
// @param[in] s 消去する変数のリストの根
// @return 演算結果を返す．
BddEdge
SmoothOp::apply(BddEdge e,
		BddEdge s)
{
  if ( e.is_invalid() ) {
    return e;
  }
  if ( s.is_invalid() ) {
    return s;
  }

  return apply_step(e, s);
}

// @brief apply() の下請け関数
BddEdge
SmoothOp::apply_step(BddEdge e,
		     BddEdge s)
{
  if ( e.is_leaf() || s.is_one() ) {
    return e;
  }

  BddNode* node = e.get_node();
  ymuint level = node->level();

  BddNode* snode = s.get_node();
  ymuint slevel = snode->level();
  while ( slevel < level ) {
    s = snode->edge1();
    snode = s.get_node();
    slevel = snode->level();
  }

  BddEdge result = get(e, s);
  if ( result.is_error() ) {
    bool inv = e.inv();
    BddEdge e0 = node->edge0(inv);
    BddEdge e1 = node->edge1(inv);
    if ( level == slevel ) {
      // 消去対象の変数だった．
      BddEdge r_0 = apply_step(e0, snode->edge1());
      if ( r_0.is_overflow() ) {
	return r_0;
      }
      BddEdge r_1 = apply_step(e1, snode->edge1());
      if ( r_1.is_overflow() ) {
	return r_1;
      }
      result = ~mAndOp->apply(~r_0, ~r_1);
      if ( result.is_overflow() ) {
	return result;
      }
    }
    else {
      BddEdge r_0 = apply_step(e0, s);
      if ( r_0.is_overflow() ) {
	return r_0;
      }
      BddEdge r_1 = apply_step(e1, s);
      if ( r_1.is_overflow() ) {
	return r_1;
      }
      result = new_node(level, r_0, r_1);
    }
    put(e, s, result);
  }

  return result;
}

END_NAMESPACE_YM_BDD
