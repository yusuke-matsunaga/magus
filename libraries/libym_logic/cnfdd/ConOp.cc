
/// @file ConOp.cc
/// @brief ConOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ConOp.h"


BEGIN_NAMESPACE_YM_CNFDD

//////////////////////////////////////////////////////////////////////
// クラス ConOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr CNFddMgrImpl
ConOp::ConOp(CNFddMgrImpl& mgr) :
  CNFddBinOp(mgr, "conjunction_table")
{
}

// @brief デストラクタ
ConOp::~ConOp()
{
}

// @brief \f$\cap\f$演算を行う関数
// @param[in] left, right オペランド
CNFddEdge
ConOp::apply(CNFddEdge left,
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
ConOp::apply_step(CNFddEdge f,
		  CNFddEdge g)
{
  // 0-element 属性に対するルール
  // f, g ともに 0-element 属性をもっていたら答にも 0-element 属性を持たせる．
  bool zattr = f.zattr() && g.zattr();
  f.normalize();
  g.normalize();

  CNFddEdge ans_e;

  // 特別な場合の処理
  // 1: 片方のCNFDDが0なら答は0，
  // 2: 同じCNFDDどうしなら答は自分自身
  if ( f.is_zero() || g.is_zero() ) {
    ans_e = CNFddEdge::make_zero();
  }
  else if ( f == g ) {
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
