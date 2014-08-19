
/// @file CofPOp.cc
/// @brief CofPOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "CofPOp.h"


BEGIN_NAMESPACE_YM_CNFDD

//////////////////////////////////////////////////////////////////////
// クラス CofPOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr CNFddMgrImpl
CofPOp::CofPOp(CNFddMgrImpl& mgr) :
  UniVOp(mgr)
{
}

// @brief デストラクタ
CofPOp::~CofPOp()
{
}

// @brief cofactor0 演算を行う関数
// @param[in] left オペランド
CNFddEdge
CofPOp::apply(CNFddEdge left)
{
  // エラー状態のチェック
  if ( left.is_invalid() ) {
    // left が異常
    return left;
  }

  clear();

  return apply_step(left);
}

// 下請け関数
CNFddEdge
CofPOp::apply_step(CNFddEdge f)
{
  // 特別な場合の処理
  // 1：fが0なら答は0，
  // 2: fが1なら答は0
  if ( f.is_const() ) {
    return CNFddEdge::make_zero();
  }

  // この時点で f は終端ではない．
  CNFddNode* f_vp = f.get_node();
  ymuint f_level = f_vp->level();
  if ( f_level > level() ) {
    return CNFddEdge::make_zero();
  }
  if ( f_level == level() ) {
    return f_vp->edge_p();
  }

  CNFddEdge ans_e = get(f);
  if ( ans_e.is_error()) {
    // 演算結果テーブルには登録されていない
    CNFddEdge f_0, f_p, f_n;
    split1(f_level, f_level, f, f_vp, f_0, f_p, f_n);

    CNFddEdge r_0 = apply_step(f_0);
    if ( r_0.is_invalid() ) {
      return r_0;
    }

    CNFddEdge r_p = apply_step(f_p);
    if ( r_p.is_invalid() ) {
      return r_p;
    }

    CNFddEdge r_n = apply_step(f_n);
    if ( r_n.is_invalid() ) {
      return r_n;
    }

    ans_e = new_node(f_level, r_0, r_p, r_n);
    put(f, ans_e);
  }
  return ans_e;
}

END_NAMESPACE_YM_CNFDD
