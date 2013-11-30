
/// @file XorOp.cc
/// @brief XorOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "XorOp.h"
#include "BddMgrImpl.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
// クラス XorOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr マネージャ
XorOp::XorOp(BddMgrImpl* mgr) :
  BddBinOp(mgr, "xor_op")
{
}

// @brief デストラクタ
XorOp::~XorOp()
{
}

// @brief 演算を行う関数
// @param[in] left, right オペランド
// @return 演算結果を返す．
BddEdge
XorOp::apply(BddEdge left,
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
XorOp::apply_step(BddEdge f,
		  BddEdge g)
{

  // 特別な場合の処理
  // 1: 片方が0なら他方を返す．
  // 2: 片方が1なら他方の否定を返す．
  // 3: 同じBDD同士のXORは0を返す．
  // 4: 極性のみが異なる関数同士なら1を返す．
  if ( f.is_zero() ) {
    return g;
  }
  if ( g.is_zero() ) {
    return f;
  }
  if ( f.is_one() ) {
    return ~g;
  }
  if ( g.is_one() ) {
    return ~f;
  }
  if ( f == g ) {
    return BddEdge::make_zero();
  }
  if ( check_reverse(f, g) ) {
    return BddEdge::make_one();
  }
  // この時点で f, g は終端ではない．

  // 極性情報は落してしまう．
  tPol f_pol = f.pol();
  tPol g_pol = g.pol();
  tPol ans_pol = f_pol * g_pol;
  f.normalize();
  g.normalize();

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

  BddEdge result;
  if ( f_0.is_zero() && f_1.is_one() ) {
    // f が肯定のリテラルで最上位のレベルの場合
    // f_0 と f_1 が異なっているということは f_level == level である．
    result = new_node(level, ~g_0, g_1);
  }
  else if ( f_0.is_one() && f_1.is_zero() ) {
    // f が否定のリテラルで最上位のレベルの場合
    // f_0 と f_1 が異なっているということは f_level == level である．
    result = new_node(level, g_0, ~g_1);
  }
  else if ( g_0.is_zero() && g_1.is_one() ) {
    // g が肯定のリテラルで最上位のレベルの場合
    // g_0 と g_1 が異なっているということは g_level == level である．
    result = new_node(level, ~f_0, f_1);
  }
  else if ( g_0.is_one() && g_1.is_zero() ) {
    // g が否定のリテラルで最上位のレベルの場合
    // g_0 と g_1 が異なっているということは g_level == level である．
    result = new_node(level, f_0, ~f_1);
  }
  else {
    // 演算結果テーブルを探す．
    result = get(f, g);
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
  }

  return BddEdge(result, ans_pol);
}

END_NAMESPACE_YM_BDD

