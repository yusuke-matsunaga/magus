
/// @file MergeOp.cc
/// @brief MergeOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "MergeOp.h"


BEGIN_NAMESPACE_YM_CNFDD

//////////////////////////////////////////////////////////////////////
// クラス MergeOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr CNFddMgrImpl
// @param[in] dis_op disjunction 演算オブジェクト
MergeOp::MergeOp(CNFddMgrImpl& mgr,
		 BinOp* dis_op) :
  BinOp(mgr, "merge_table"),
  mDisOp(dis_op)
{
}

// @brief デストラクタ
MergeOp::~MergeOp()
{
}

// @brief マージ演算を行う関数
// @param[in] left, right オペランド
CNFddEdge
MergeOp::apply(CNFddEdge left,
	       CNFddEdge right)
{
  // エラー状態のチェック
  if ( left.is_invalid() ) {
    // left が異常
    return left;
  }
  if ( right.is_invalid() ) {
    // right が異常
    return right;
  }

  return apply_step(left, right);
}

// apply の下請け関数
CNFddEdge
MergeOp::apply_step(CNFddEdge f,
		    CNFddEdge g)
{
  // 特別な場合の処理
  // 1: 片方が0なら答は0
  // 2: 片方が1なら答は他方
  // 3: 同じなら答は自分自身
  if ( f.is_zero() || g.is_zero() ) {
    return CNFddEdge::make_zero();
  }
  if ( f.is_one() ) {
    return g;
  }
  if ( g.is_one() || f == g ) {
    return f;
  }

  // この時点で f, g は終端ではない．

  // 演算結果テーブルが当たりやすくなるように順序を正規化する
  if ( f > g ) {
    CNFddEdge tmp = f;
    f = g;
    g = tmp;
  }

  CNFddEdge ans_e = get(f, g);
  if ( ans_e.is_error() ) {
    // 演算結果テーブルには登録されていない
    CNFddEdge f_0, f_p, f_n;
    CNFddEdge g_0, g_p, g_n;
    ymuint level = split(f, g, f_0, f_p, f_n, g_0, g_p, g_n);

    // 答の0枝はfの0枝とgの0枝の演算結果
    CNFddEdge r_0 = apply_step(f_0, g_0);
    if ( r_0.is_invalid() ) {
      return r_0;
    }

    // 答のp枝は f_p:g_0, f_p:g_p, f_0:g_p の3つの結果の disjunction
    CNFddEdge r_p0 = apply_step(f_p, g_0);
    if ( r_p0.is_invalid() ) {
      return r_p0;
    }
    CNFddEdge r_0p = apply_step(f_0, g_p);
    if ( r_0p.is_invalid() ) {
      return r_0p;
    }
    CNFddEdge r_pp = apply_step(f_p, g_p);
    if ( r_pp.is_invalid() ) {
      return r_pp;
    }
    CNFddEdge tmp = mDisOp->apply(r_0p, r_p0);
    if ( tmp.is_invalid() ) {
      return tmp;
    }
    CNFddEdge r_p = mDisOp->apply(tmp, r_pp);
    if ( r_p.is_invalid() ) {
      return r_p;
    }

    // 答のn枝は f_n:g_0, f_n:g_n, f_0:g_n の3つの結果の disjunction
    CNFddEdge r_n0 = apply_step(f_n, g_0);
    if ( r_n0.is_invalid() ) {
      return r_n0;
    }
    CNFddEdge r_0n = apply_step(f_0, g_n);
    if ( r_0n.is_invalid() ) {
      return r_0n;
    }
    CNFddEdge r_nn = apply_step(f_n, g_n);
    if ( r_nn.is_invalid() ) {
      return r_nn;
    }
    tmp = mDisOp->apply(r_0n, r_n0);
    if ( tmp.is_invalid() ) {
      return tmp;
    }
    CNFddEdge r_n = mDisOp->apply(tmp, r_nn);
    if ( r_n.is_invalid() ) {
      return r_n;
    }

    ans_e = new_node(level, r_0, r_p, r_n);
    put(f, g, ans_e);
  }

  return ans_e;
}

END_NAMESPACE_YM_CNFDD
