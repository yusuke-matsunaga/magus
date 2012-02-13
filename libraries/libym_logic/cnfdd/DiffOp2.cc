
/// @file DiffOp2.cc
/// @brief DiffOp2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "DiffOp2.h"


BEGIN_NAMESPACE_YM_CNFDD

//////////////////////////////////////////////////////////////////////
// クラス DiffOp2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr CNFddMgrImpl
// @param[in] dis_op disjunction operator
DiffOp2::DiffOp2(CNFddMgrImpl& mgr,
		 BinOp* dis_op) :
  BinOp(mgr, "diff_table"),
  mDisOp(dis_op)
{
}

// @brief デストラクタ
DiffOp2::~DiffOp2()
{
}

// @brief \f$\setminus\f$演算を行う関数
// @param[in] left, right オペランド
CNFddEdge
DiffOp2::apply(CNFddEdge left,
	       CNFddEdge right)
{
  // エラー状態のチェック
  if ( left.is_invalid() ) {
    // left が異常
    return left;
  }
  if ( right.is_invalid() ) {
    // ritht が異常
    return right;
  }

  return apply_step(left, right);
}

// cap_op の下請け関数
CNFddEdge
DiffOp2::apply_step(CNFddEdge f,
		    CNFddEdge g)
{
  if ( g.zattr() ) {
    // g が 0-element 属性を持っていたら答えは0
    return CNFddEdge::make_zero();
  }

  // 以降では g は 0-element 属性を持っていない．

  // 0-element 属性に対するルール
  // f が 0-element 属性をもっており g にないとき 0-element 属性を持たせる．
  bool zattr = f.zattr();
  f.normalize();

  CNFddEdge ans_e;

  // 特別な場合の処理
  // 1: f が 0 なら答は 0
  // 2: g が 0 なら答は f
  // 3: 同じCNFDDどうしなら答は 0
  if ( f.is_zero() || g.is_zero() ) {
    ans_e = f;
  }
  else if ( f == g ) {
    ans_e = CNFddEdge::make_zero();
  }
  else {
    // この時点で f,g は終端ではない．

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

      // p 枝に対しては g の 0枝とp枝が引く値となる．
      CNFddEdge tmp_p = mDisOp->apply(g_0, g_p);
      if ( tmp_p.is_invalid() ) {
	return tmp_p;
      }
      CNFddEdge r_p = apply_step(f_p, tmp_p);
      if ( r_p.is_invalid() ) {
	return r_p;
      }

      // n 枝に対しては g の 0枝とn枝が引く値となる．
      CNFddEdge tmp_n = mDisOp->apply(g_0, g_n);
      if ( tmp_n.is_invalid() ) {
	return tmp_n;
      }
      CNFddEdge r_n = apply_step(f_n, tmp_n);
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
