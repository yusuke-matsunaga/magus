
/// @file GcOp.cc
/// @brief GcOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "GcOp.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
// クラス GcOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr マネージャ
GcOp::GcOp(BddMgrImpl* mgr) :
  BddBinOp(mgr, "gcof_op")
{
}

// @brief デストラクタ
GcOp::~GcOp()
{
}

// @brief 演算を行う関数
// @param[in] left, right オペランド
// @return 演算結果を返す．
BddEdge
GcOp::apply(BddEdge left,
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
  if ( right.is_zero() ) {
    // ゼロでは割れない
    return BddEdge::make_error();
  }

  return apply_step(left, right);
}

// @brief 実際の演算を行う関数
BddEdge
GcOp::apply_step(BddEdge f,
		 BddEdge g)
{
  if ( g.is_zero() ) {
    // エラー
    return BddEdge::make_error();
  }
  if ( f.is_leaf() || g.is_one() ) {
    return f;
  }
  if ( f == g ) {
    return BddEdge::make_one();
  }
  if ( check_reverse(f, g) ) {
    return BddEdge::make_zero();
  }

  // この時点で f,g は終端ではない．

  // (~f) / c の結果は ~(f / c) なので f を正規化する．
  tPol f_pol = f.pol();
  f.normalize();

  BddEdge result = get(f, g);
  if ( result.is_error() ) {
    // 演算結果テーブルには登録されていない
    BddNode* f_v = f.get_node();
    BddNode* g_v = g.get_node();
    tPol g_p = g.pol();
    ymuint f_level = f_v->level();
    ymuint g_level = g_v->level();

    BddEdge f_0, f_1;
    if ( f_level <= g_level ) {
      f_0 = f_v->edge0();
      f_1 = f_v->edge1();
    }
    else {
      f_0 = f_1 = f;
    }

    if ( f_level < g_level ) {
      BddEdge r_0 = apply_step(f_0, g);
      if ( r_0.is_invalid() ) {
	return r_0;
      }
      BddEdge r_1 = apply_step(f_1, g);
      if ( r_1.is_invalid() ) {
	return r_1;
      }
      result = new_node(f_level, r_0, r_1);
    }
    else { // f_level >= g_level
      BddEdge g_0 = g_v->edge0(g_p);
      BddEdge g_1 = g_v->edge1(g_p);
      if ( g_0.is_zero() ) {
	result = apply_step(f_1, g_1);
      }
      else if ( g_1.is_zero() ) {
	result = apply_step(f_0, g_0);
      }
      else {
	BddEdge r_0 = apply_step(f_0, g_0);
	if ( r_0.is_invalid() ) {
	  return r_0;
	}
	BddEdge r_1 = apply_step(f_1, g_1);
	if ( r_1.is_invalid() ) {
	  return r_1;
	}
	result = new_node(g_level, r_0, r_1);
      }
    }
    put(f, g, result);
  }

  // 極性を元に戻す．
  result.addpol(f_pol);

  return result;
}

END_NAMESPACE_YM_BDD
