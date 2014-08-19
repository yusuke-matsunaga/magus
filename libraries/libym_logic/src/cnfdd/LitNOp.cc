
/// @file LitNOp.cc
/// @brief LitNOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "LitNOp.h"


BEGIN_NAMESPACE_YM_CNFDD

//////////////////////////////////////////////////////////////////////
// クラス LitNOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr CNFddMgrImpl
LitNOp::LitNOp(CNFddMgrImpl& mgr) :
  UniVOp(mgr)
{
}

// @brief デストラクタ
LitNOp::~LitNOp()
{
}

// @brief cofactor0 演算を行う関数
// @param[in] left オペランド
CNFddEdge
LitNOp::apply(CNFddEdge left)
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
LitNOp::apply_step(CNFddEdge f)
{
  // 特別な場合の処理
  // 1：fが0なら答は0，
  // 2: fが1なら答は level() の n枝に base を持つノード
  if ( f.is_zero() ) {
    return CNFddEdge::make_zero();
  }
  if ( f.is_one() ) {
    CNFddEdge zero = CNFddEdge::make_zero();
    return new_node(level(), zero, zero, f);
  }

  // この時点で f は終端ではない．
  CNFddNode* f_node = f.get_node();
  ymuint f_level = f_node->level();
  if ( f_level > level() ) {
    CNFddEdge zero = CNFddEdge::make_zero();
    return new_node(level(), zero, zero, f);
  }
  if ( f_level == level() ) {
    // 0枝とn枝を取り替える．
    CNFddEdge e_0 = f_node->edge_0();
    e_0.add_zattr(f.zattr());
    CNFddEdge e_p = f_node->edge_p();
    CNFddEdge e_n = f_node->edge_n();
    return new_node(f_level, e_n, e_p, e_0);
  }

  CNFddEdge ans_e = get(f);
  if ( ans_e.is_error()) {
    // 演算結果テーブルには登録されていない
    CNFddEdge f_0, f_p, f_n;
    split1(f_level, f_level, f, f_node, f_0, f_p, f_n);

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
