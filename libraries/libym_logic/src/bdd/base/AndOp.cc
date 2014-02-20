
/// @file AndOp.cc
/// @brief AndOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AndOp.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
// クラス AndOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr マネージャ
AndOp::AndOp(BddMgrImpl* mgr) :
  BddBinOp(mgr, "and_op")
{
}

// @brief デストラクタ
AndOp::~AndOp()
{
}

// @brief 演算を行う関数
// @param[in] left, right オペランド
// @return 演算結果を返す．
BddEdge
AndOp::apply(BddEdge left,
	     BddEdge right)
{
  // エラー状態のチェック
  if ( left.is_error() || right.is_error() ) {
    // どちらかがエラー
    return BddEdge::make_error();
  }
  if ( left.is_overflow() || right.is_overflow() ) {
    // どちらかがオーバーフロー
    return BddEdge::make_overflow();
  }

  return apply_step(left, right);
}

// @brief 実際の演算を行う関数
BddEdge
AndOp::apply_step(BddEdge f,
		  BddEdge g)
{
  // 特別な場合の処理
  // 1：片方のBDDが0なら答えは0，互いに否定の関係でも答えは0．
  // 2：両方とも1なら答えは1．
  // 3：片方が1ならもう片方を返す．
  // 4：同じBDDどうしのANDは自分自身
  // 2 は 3 でカバーされている．
  if ( f.is_zero() || g.is_zero() || check_reverse(f, g) ) {
    return BddEdge::make_zero();
  }
  if ( f.is_one() ) {
    return g;
  }
  if ( g.is_one() || f == g ) {
    return f;
  }
  // この時点で f,g は終端ではない．

  // 演算結果テーブルが当たりやすくなるように順序を正規化する
  if ( f > g ) {
    BddEdge tmp = f;
    f = g;
    g = tmp;
  }

  tPol f_pol = f.pol();
  tPol g_pol = g.pol();
  BddNode* f_vp = f.get_node();
  BddNode* g_vp = g.get_node();
  ymuint f_level = f_vp->level();
  ymuint g_level = g_vp->level();
  ymuint level = f_level;
  if ( level > g_level ) {
    level = g_level;
  }

  BddEdge f_0, f_1;
  split1(level, f_level, f, f_vp, f_pol, f_0, f_1);

  BddEdge g_0, g_1;
  split1(level, g_level, g, g_vp, g_pol, g_0, g_1);

  // 片方が最上位のリテラル関数の場合の特別な処理
  if ( f_0.is_zero() && f_1.is_one() ) {
    // f が肯定のリテラルの場合
    // f_0 と f_1 が異なるということは f_level == level
    return new_node(f_level, BddEdge::make_zero(), g_1);
  }
  if ( f_0.is_one() && f_1.is_zero() ) {
    // f が否定のリテラルの場合
    // f_0 と f_1 が異なるということは f_level == level
    return new_node(f_level, g_0, BddEdge::make_zero());
  }
  if ( g_0.is_zero() && g_1.is_one() ) {
    // g が肯定のリテラルの場合
    // g_0 と g_1 が異なるということは f_level == level
    return new_node(g_level, BddEdge::make_zero(), f_1);
  }
  if ( g_0.is_one() && g_1.is_zero() ) {
    // g が否定のリテラルの場合
    // g_0 と g_1 が異なるということは f_level == level
    return new_node(g_level, f_0, BddEdge::make_zero());
  }

  BddEdge result = get(f, g);
  if ( result.is_error() ) {
    // 演算結果テーブルには登録されていない

    BddEdge r_0 = apply_step(f_0, g_0);
    if ( r_0.is_overflow() ) {
      return BddEdge::make_overflow();
    }

    BddEdge r_1 = apply_step(f_1, g_1);
    if ( r_1.is_overflow() ) {
      return BddEdge::make_overflow();
    }

    result = new_node(level, r_0, r_1);
    put(f, g, result);
  }

  return result;
}

END_NAMESPACE_YM_BDD
