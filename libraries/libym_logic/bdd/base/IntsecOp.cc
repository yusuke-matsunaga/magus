
/// @file IntsecOp.cc
/// @brief IntsecOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "IntsecOp.h"
#include "BddMgrImpl.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
// クラス IntsecOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr マネージャ
IntsecOp::IntsecOp(BddMgrImpl* mgr) :
  BddBinOp(mgr, "intsec_op")
{
}

// @brief デストラクタ
IntsecOp::~IntsecOp()
{
}

// @brief 演算を行う関数
// @param[in] left, right オペランド
// @return 演算結果を返す．
BddEdge
IntsecOp::apply(BddEdge left,
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
IntsecOp::apply_step(BddEdge f,
		  BddEdge g)
{
  // 特別な場合の処理
  // 1: 片方が0なら絶対に交わらないので 0 を返す．
  // 2: 極性のみが異なっている場合にも交わらないので 0 を返す．
  // 3: 片方が1なら交わっているので 1 を返す．
  // 4: 同じ関数なら交わっているので 1 を返す．
  if ( f.is_zero() || g.is_zero() || check_reverse(f, g) ) {
    return BddEdge::make_zero();
  }
  if ( f.is_one() || g.is_one() || f == g ) {
    return BddEdge::make_one();
  }
  // この時点で f, g は終端ではない．

  // 極性は正規化されている(否定属性は1枝のみ，葉は0のみ)
  // ので f も g も正極性のときは f(0,0,...,0) = g(0,0,...,0) = 0
  // つまりすべての入力を 0 にしたときの関数値は 0 であるので
  // 交わっていることがわかる．
  tPol f_pol = f.pol();
  tPol g_pol = g.pol();
  if ( f_pol == kPolPosi && g_pol == kPolPosi ) {
    return BddEdge::make_one();
  }

  // 演算結果テーブルが当たりやすくなるように順序を正規化する
  if ( f > g ) {
    BddEdge tmp = f;
    f = g;
    g = tmp;
  }

  BddEdge result = get(f, g);
  if ( result.is_error() ) {
    // 演算結果テーブルには登録されていない
    BddEdge f_0, f_1;
    BddEdge g_0, g_1;
    ymuint level = split(f, g, f_0, f_1, g_0, g_1);
    result = apply_step(f_0, g_0);
    if ( result.is_overflow() ) {
      return BddEdge::make_overflow();
    }
    if ( result.is_zero() ) {
      result = apply_step(f_1, g_1);
      if ( result.is_overflow() ) {
	return BddEdge::make_overflow();
      }
    }
    put(f, g, result);
  }

  return result;
}

END_NAMESPACE_YM_BDD

