
/// @file XcOp.cc
/// @brief XcOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "XcOp.h"
#include "BddBinOp.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
// クラス XcOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr マネージャ
// @param[in] xor_op XOR演算オブジェクト
XcOp::XcOp(BddMgrImpl* mgr,
	   BddBinOp* xor_op) :
  BddUniOp(mgr),
  mXorOp(xor_op)
{
}

// @brief デストラクタ
XcOp::~XcOp()
{
}

// @brief 演算を行う関数
// @param[in] left オペランド
// @param[in] id コファクタをとる変数番号
// @return 演算結果を返す．
BddEdge
XcOp::apply(BddEdge left,
	    VarId id)
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
  clear();
  return apply_step(left);
}

// @brief 実際の演算を行う関数
BddEdge
XcOp::apply_step(BddEdge f)
{
  if ( f.is_leaf() ) {
    // 終端なら(0/1)どちらでも答は0
    return BddEdge::make_zero();
  }

  // この時点で fは終端ではない．

  // 極性情報は落してしまう．
  f.normalize();
  BddNode* node = f.get_node();
  ymuint level = node->level();
  if ( level > mLevel ) {
    // コファクターをとる変数よりも下のレベルなら答は 0
    return BddEdge::make_zero();
  }

  BddEdge result = get(f);
  if ( result.is_error() ) {
    // 演算結果テーブルには登録されていない
    BddEdge f_0 = node->edge0();
    BddEdge f_1 = node->edge1();
    if ( level == mLevel ) {
      result = mXorOp->apply(f_0, f_1);
    }
    else {
      // level < mLevel
      BddEdge r_0 = apply_step(f_0);
      if ( r_0.is_overflow() ) {
	return BddEdge::make_overflow();
      }
      BddEdge r_1 = apply_step(f_1);
      if ( r_1.is_overflow() ) {
	return BddEdge::make_overflow();
      }
      result = new_node(level, r_0, r_1);
    }
    put(f, result);
  }

  return result;
}

END_NAMESPACE_YM_BDD
