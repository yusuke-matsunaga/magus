
/// @file AndOp.cc
/// @brief AndOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "AndOp.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
// クラス AndOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr マネージャ
AndOp::AndOp(BddMgrImpl* mgr) :
  mMgr(mgr)
{
}

// @brief デストラクタ
AndOp::~AndOp()
{
}

// @brief 演算を行う関数
// @param[in] left, right オペランド
// @return 演算結果を買えす．
BddEdge
AndOp::apply(BddEdge left,
	     BddEdge right)
{
  // エラー状態のチェック
  if ( left.is_error() || right.is_error() ) {
    // どちらかがエラー
    return BddEdge::make_error();
  }
  if ( left.is_overflow() || right.is_overflow() ) {
    // どちらかがオーバーフロー
    return BddEdge::make_overflow();
  }

  return apply_step(left, right);
}

// @brief 実際の演算を行う関数
BddEdge
AndOp::apply_step(BddEdge f,
		  BddEdge g)
{
  // 特別な場合の処理
  // 1：片方のBDDが0なら答えは0，互いに否定の関係でも答えは0．
  // 2：両方とも1なら答えは1．
  // 3：片方が1ならもう片方を返す．
  // 4：同じBDDどうしのANDは自分自身
  // 2 は 3 でカバーされている．
  if ( f.is_zero() || g.is_zero() || check_reverse(f, g) ) {
    return BddEdge::make_zero();
  }
  if ( f.is_one() ) {
    return g;
  }
  if ( g.is_one() || f == g ) {
    return f;
  }
  // この時点で f,g は終端ではない．

  // 演算結果テーブルが当たりやすくなるように順序を正規化する
  if ( f > g ) {
    BddEdge tmp = f;
    f = g;
    g = tmp;
  }

  BddEdge result = mCompTable.get(f, g);
  if ( result.is_error() ) {
    // 演算結果テーブルには登録されていない
    BddEdge f_0, f_1;
    BddEdge g_0, g_1;
    ymuint level = split(f, g, f_0, f_1, g_0, g_1);
    BddEdge r_0 = apply_step(f_0, g_0);
    if ( r_0.is_overflow() ) {
      return BddEdge::make_overflow();
    }
    BddEdge r_1 = apply_step(f_1, g_1);
    if ( r_1.is_overflow() ) {
      return BddEdge::make_overflow();
    }
    result = new_node(level, r_0, r_1);
    mCompTable.put(f, g, result);
  }

  return result;
}

END_NAMESPACE_YM_BDD

