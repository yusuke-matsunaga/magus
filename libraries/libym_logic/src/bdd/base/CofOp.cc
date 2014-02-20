
/// @file CofOp.cc
/// @brief CofOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "CofOp.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
// クラス CofOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr マネージャ
CofOp::CofOp(BddMgrImpl* mgr) :
  BddUniOp(mgr)
{
}

// @brief デストラクタ
CofOp::~CofOp()
{
}

// @brief 演算を行う関数
// @param[in] left オペランド
// @param[in] id コファクタをとる変数番号
// @param[in] pol 極性
// @return 演算結果を返す．
BddEdge
CofOp::apply(BddEdge left,
	     VarId id,
	     tPol pol)
{
  // エラー状態のチェック
  if ( left.is_error() ) {
    // どちらかがエラー
    return BddEdge::make_error();
  }
  if ( left.is_overflow() ) {
    // どちらかがオーバーフロー
    return BddEdge::make_overflow();
  }

  mLevel = mgr()->level(id);
  mPol = pol;
  clear();
  return apply_step(left);
}

// @brief 実際の演算を行う関数
BddEdge
CofOp::apply_step(BddEdge f)
{
  if ( f.is_leaf() ) {
    // 終端なら(0/1)答は自分自身
    return f;
  }

  // この時点で fは終端ではない．

  tPol f_pol = f.pol();
  BddNode* node = f.get_node();
  ymuint level = node->level();
  if ( level > mLevel ) {
    // コファクターをとる変数よりも下のレベルなら答は自分自身
    return f;
  }
  else if ( level == mLevel ) {
    if ( mPol == kPolPosi ) {
      return node->edge1(f_pol);
    }
    else { // pol == kPolNega
      return node->edge0(f_pol);
    }
  }

  // (~f) / c の結果は ~(f / c) なので f を正規化する．
  f.normalize();

  BddEdge result = get(f);
  if ( result.is_error() ) {
    // 演算結果テーブルには登録されていない
    BddEdge f_0 = node->edge0();
    BddEdge r_0 = apply_step(f_0);
    if ( r_0.is_overflow() ) {
      return BddEdge::make_overflow();
    }

    BddEdge f_1 = node->edge1();
    BddEdge r_1 = apply_step(f_1);
    if ( r_1.is_overflow() ) {
      return BddEdge::make_overflow();
    }
    result = new_node(level, r_0, r_1);

    // 演算結果テーブルに登録する．
    put(f, result);
  }

  // 極性をもとに戻す．
  result.addpol(f_pol);

  return result;
}

END_NAMESPACE_YM_BDD
