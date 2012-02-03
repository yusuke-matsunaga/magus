
/// @file MinOp.cc
/// @brief MinOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "MinOp.h"
#include "BinOp.h"


BEGIN_NAMESPACE_YM_CNFDD

//////////////////////////////////////////////////////////////////////
// クラス MinOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr CNFddMgrImpl
// @param[in] diff_op 集合差演算オブジェクト
MinOp::MinOp(CNFddMgrImpl& mgr,
	     BinOp* diff_op) :
  UniOp(mgr, "minimal_table"),
  mDiffOp(diff_op)
{
}

// @brief デストラクタ
MinOp::~MinOp()
{
}

// @brief cofactor0 演算を行う関数
// @param[in] left オペランド
CNFddEdge
MinOp::apply(CNFddEdge left)
{
  // エラー状態のチェック
  if ( left.is_invalid() ) {
    // left が異常
    return left;
  }

  return apply_step(left);
}

// 下請け関数
CNFddEdge
MinOp::apply_step(CNFddEdge f)
{
  // 特別な場合の処理
  // 1：fが0なら答は0，
  // 2: fが1なら答は1
  if ( f.is_const() ) {
    return f;
  }

  // この時点で f は終端ではない．
  CNFddEdge ans_e = get(f);
  if ( ans_e.is_error()) {
    // 演算結果テーブルには登録されていない
    CNFddNode* f_vp = f.get_node();
    ymuint f_level = f_vp->level();
    CNFddEdge f_0, f_p, f_n;
    split1(f_level, f_level, f, f_vp, f_0, f_p, f_n);

    CNFddEdge r_0 = apply_step(f_0);
    if ( r_0.is_invalid() ) {
      return r_0;
    }

    // p枝は0枝と重複している部分を取り除く
    CNFddEdge r_p = apply_step(f_p);
    if ( r_p.is_invalid() ) {
      return r_p;
    }
    r_p = mDiffOp->apply(r_p, r_0);
    if ( r_p.is_invalid() ) {
      return r_p;
    }

    // n枝は0枝と重複している部分を取り除く
    CNFddEdge r_n = apply_step(f_n);
    if ( r_n.is_invalid() ) {
      return r_n;
    }
    r_n = mDiffOp->apply(r_n, r_0);
    if ( r_n.is_invalid() ) {
      return r_n;
    }

    ans_e = new_node(f_level, r_0, r_p, r_n);
    put(f, ans_e);
  }
  return ans_e;
}

END_NAMESPACE_YM_CNFDD
