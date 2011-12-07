
/// @file SwapOp.cc
/// @brief SwapOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "SwapOp.h"
#include "ZddMgrImpl.h"
#include "CompTbl.h"


BEGIN_NAMESPACE_YM_ZDD

//////////////////////////////////////////////////////////////////////
// クラス SwapOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr ZddMgrImpl
SwapOp::SwapOp(ZddMgrImpl& mgr) :
  mMgr(mgr),
  mSwapTable(mgr, "swap_table")
{
}

// @brief デストラクタ
SwapOp::~SwapOp()
{
}

// @brief swap 演算を行う関数
// @param[in] left オペランド
// @param[in] var 変数番号
ZddEdge
SwapOp::apply(ZddEdge left,
	      VarId var)
{
  // エラー状態のチェック
  if ( left.is_error() ) {
    // どちらかがエラー
    return ZddEdge::make_error();
  }
  if ( left.is_overflow() ) {
    // どちらかがオーバーフロー
    return ZddEdge::make_overflow();
  }

  mVar = var;
  mVarEdge = mMgr.new_node(ZddEdge::make_zero(), ZddEdge::make_one());
  return swap_step(left)
}

// swap_op の下請け関数
ZddEdge
SwapOp::swap_step(ZddEdge f)
{
  bool zattr = f.zattr();
  f.normalize();

  ZddEdge ans_e;

  // 特別な場合の処理
  // 1：片方のZDDが0なら答は0，
  // 2：同じZDDどうしなら答は自分自身
  if ( f.is_zero() || g.is_zero() ) {
    ans_e = ZddEdge::make_zero();
  }
  else if ( f == g ) {
    ans_e = f;
  }
  else {
    // この時点で f,g は終端ではない．

    // 演算結果テーブルが当たりやすくなるように順序を正規化する
    if ( f > g ) {
      ZddEdge tmp = f;
      f = g;
      g = tmp;
    }

    ZddEdge result = mSwapTable.get(f, g);
    if ( result.is_error() ) {
      // 演算結果テーブルには登録されていない
      ZddEdge f_0, f_1;
      ZddEdge g_0, g_1;
      ZddVar* var = split(f, g, f_0, f_1, g_0, g_1);
      ZddEdge r_0 = cap_step(f_0, g_0);
      if ( r_0.is_overflow() ) {
	return ZddEdge::make_overflow();
      }
      ZddEdge r_1 = cap_step(f_1, g_1);
      if ( r_1.is_overflow() ) {
	return ZddEdge::make_overflow();
      }
      result = mMgr.new_node(var, r_0, r_1);
      mSwapTable.put(f, g, result);
    }
  }
  return ans_e.add_zattr(zattr);
}

END_NAMESPACE_YM_ZDD
