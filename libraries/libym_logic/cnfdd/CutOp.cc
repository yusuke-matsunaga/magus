
/// @file CutOp.cc
/// @brief CutOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "CutOp.h"


BEGIN_NAMESPACE_YM_CNFDD

//////////////////////////////////////////////////////////////////////
// クラス CutOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr CNFddMgrImpl
CutOp::CutOp(CNFddMgrImpl& mgr) :
  UniNOp(mgr, "cut_off_table")
{
}

// @brief デストラクタ
CutOp::~CutOp()
{
}

// @brief cut_off 演算を行う関数
// @param[in] left オペランド
// @param[in] limit 要素数の制限値
CNFddEdge
CutOp::apply(CNFddEdge left,
	     ymuint limit)
{
  // エラー状態のチェック
  if ( left.is_invalid() ) {
    // left が異常
    return left;
  }

  return apply_step(left, limit);
}

// 下請け関数
CNFddEdge
CutOp::apply_step(CNFddEdge f,
		  ymuint limit)
{
  // 特別な場合の処理
  // 1：fが0なら答は0，
  // 2: fが1なら答は1
  if ( f.is_const() ) {
    return f;
  }

  // この時点で f は終端ではない．
  CNFddEdge ans_e = get(f, limit);
  if ( ans_e.is_error()) {
    // 演算結果テーブルには登録されていない
    CNFddNode* f_vp = f.get_node();
    ymuint f_level = f_vp->level();
    CNFddEdge f_0, f_p, f_n;
    split1(f_level, f_level, f, f_vp, f_0, f_p, f_n);

    CNFddEdge r_0 = apply_step(f_0, limit);
    if ( r_0.is_invalid() ) {
      return r_0;
    }

    if ( limit > 0 ) {
      CNFddEdge r_p = apply_step(f_p, limit - 1);
      if ( r_p.is_invalid() ) {
	return r_p;
      }
      CNFddEdge r_n = apply_step(f_n, limit - 1);
      if ( r_n.is_invalid() ) {
	return r_n;
      }
      ans_e = new_node(f_level, r_0, r_p, r_n);
    }
    else {
      ans_e = r_0;
    }

    put(f, limit, ans_e);
  }
  return ans_e;
}

END_NAMESPACE_YM_CNFDD
