
/// @file AeOp.cc
/// @brief AeOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AeOp.h"
#include "BddBinOp.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
// クラス AeOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr マネージャ
// @param[in] and_op AND 演算オブジェクト
// @param[in] smooth_op smoothing 演算オブジェクト
AeOp::AeOp(BddMgrImpl* mgr,
	   BddBinOp* and_op,
	   BddBinOp* smooth_op) :
  BddTriOp(mgr, "ite_op"),
  mAndOp(and_op),
  mSmoothOp(smooth_op)
{
}

// @brief デストラクタ
AeOp::~AeOp()
{
}

// @brief 演算を行う関数
// @param[in] e1, e2, e3 オペランド
// @return 演算結果を返す．
BddEdge
AeOp::apply(BddEdge e1,
	    BddEdge e2,
	    BddEdge s)
{
  // エラー状態のチェック
  if ( e1.is_error() || e2.is_error() || s.is_error() ) {
    // どちらかがエラー
    return BddEdge::make_error();
  }
  if ( e1.is_overflow() || e2.is_overflow() || s.is_overflow() ) {
    // どちらかがオーバーフロー
    return BddEdge::make_overflow();
  }

  return apply_step(e1, e2, s);
}

// @brief 実際の演算を行う関数
BddEdge
AeOp::apply_step(BddEdge f,
		 BddEdge g,
		 BddEdge s)
{
  if ( f.is_zero() || g.is_zero() ) {
    // どちらかが0なら答は0
    return f;
  }
  if ( check_reverse(f, g) ) {
    return BddEdge::make_zero();
  }
  if ( f.is_one() && g.is_one() ) {
    // 両方1なら答は1
    return f;
  }
  if ( f == g ) {
    // fとgが等しければ単独のsmooth演算を呼ぶ．
    return mSmoothOp->apply(f, s);
  }
  if ( f.is_one() ) {
    // fが1ならgのsmooth演算を呼ぶ．
    return mSmoothOp->apply(g, s);
  }
  if ( g.is_one() ) {
    // gが1ならfのsmooth演算を呼ぶ．
    return mSmoothOp->apply(f, s);
  }
  if ( s.is_one() ) {
    // sが1ならOR演算を呼ぶ．
    return ~mAndOp->apply(~f, ~g);
  }

  // f と g は対称なので正規化する．
  if ( f > g ) {
    BddEdge tmp = f;
    f = g;
    g = tmp;
  }

  BddNode* f_vp = f.get_node();
  BddNode* g_vp = g.get_node();
  BddNode* s_vp = s.get_node();
  ymuint f_level = f_vp->level();
  ymuint g_level = g_vp->level();
  ymuint s_level = s_vp->level();

  ymuint top = f_level;
  if ( top > g_level) {
    top = g_level;
  }

  while ( s_level < top ) {
    s = s_vp->edge1();
    s_vp = s.get_node();
    s_level = s_vp->level();
  }

  BddEdge result = get(f, g, s);
  if ( result.is_error() ) {
    tPol f_pol = f.pol();
    tPol g_pol = g.pol();
    BddEdge f_0, f_1;
    BddEdge g_0, g_1;
    split1(top, f_level, f, f_vp, f_pol, f_0, f_1);
    split1(top, g_level, g, g_vp, g_pol, g_0, g_1);
    if ( s_level == top ) {
      // top は消去対象の変数
      BddEdge r_0 = apply_step(f_0, g_0, s_vp->edge1());
      if ( r_0.is_overflow() ) {
	return r_0;
      }
      BddEdge r_1 = apply_step(f_1, g_1, s_vp->edge1());
      if ( r_1.is_overflow() ) {
	return r_1;
      }
      result = ~mAndOp->apply(~r_0, ~r_1);
      if ( result.is_overflow() ) {
	return result;
      }
    }
    else {
      BddEdge r_0 = apply_step(f_0, g_0, s);
      if ( r_0.is_overflow() ) {
	return r_0;
      }
      BddEdge r_1 = apply_step(f_1, g_1, s);
      if ( r_1.is_overflow() ) {
	return r_1;
      }
      result = new_node(top, r_0, r_1);
    }
    put(f, g, s, result);
  }
  return result;
}

END_NAMESPACE_YM_BDD
