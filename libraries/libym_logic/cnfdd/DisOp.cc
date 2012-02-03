
/// @file DisOp.cc
/// @brief DisOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "DisOp.h"


BEGIN_NAMESPACE_YM_CNFDD

//////////////////////////////////////////////////////////////////////
// クラス DisOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr CNFddMgrImpl
DisOp::DisOp(CNFddMgrImpl& mgr) :
  CNFddBinOp(mgr, "disjunction_table")
{
}

// @brief デストラクタ
DisOp::~DisOp()
{
}

// @brief \f$\vee\f$演算を行う関数
// @param[in] left, right オペランド
CNFddEdge
DisOp::apply(CNFddEdge left,
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
DisOp::apply_step(CNFddEdge f,
		  CNFddEdge g)
{
  // 0-element 属性に対するルール
  // f, g のいずれかが 0-element 属性をもっていたら答にも 0-element 属性を持たせる．
  bool zattr = f.zattr() || g.zattr();
  f.normalize();
  g.normalize();

  CNFddEdge ans_e;

  // 特別な場合の処理
  // 1: 片方のCNFDDが0なら答は他方，
  // 2: 同じCNFDDどうしなら答は自分自身
  if ( f.is_zero() ) {
    ans_e = g;
  }
  else if ( g.is_zero() || f == g ) {
    ans_e = f;
  }
  else {
    // この時点で f,g は終端ではない．

    // 演算結果テーブルが当たりやすくなるように順序を正規化する
    if ( f > g ) {
      CNFddEdge tmp = f;
      f = g;
      g = tmp;
    }

    ans_e = get(f, g);
    if ( ans_e.is_error() ) {
      // 演算結果テーブルには登録されていない
      CNFddEdge f_0, f_p, f_n;
      CNFddEdge g_0, g_p, g_n;
      ymuint level = split(f, g, f_0, f_p, f_n, g_0, g_p, g_n);

      CNFddEdge r_0 = apply_step(f_0, g_0);
      if ( r_0.is_invalid() ) {
	return r_0;
      }

      CNFddEdge r_p = apply_step(f_p, g_p);
      if ( r_p.is_invalid() ) {
	return r_p;
      }

      CNFddEdge r_n = apply_step(f_n, g_n);
      if ( r_n.is_invalid() ) {
	return r_n;
      }

      ans_e = new_node(level, r_0, r_p, r_n);
      put(f, g, ans_e);
    }
  }
  return ans_e.add_zattr(zattr);
}

END_NAMESPACE_YM_CNFDD
