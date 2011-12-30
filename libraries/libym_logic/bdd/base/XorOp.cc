
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

  BddEdge result = get(f, g);
  if ( result.is_error() ) {
    // 演算結果テーブルには登録されていない
    BddEdge f_0, f_1;
    BddEdge g_0, g_1;
    ymuint level = split(f, g, f_0, f_1, g_0, g_1);
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

  return BddEdge(result, ans_pol);
}

END_NAMESPACE_YM_BDD

