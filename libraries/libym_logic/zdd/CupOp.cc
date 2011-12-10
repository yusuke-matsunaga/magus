
/// @file CupOp.cc
/// @brief CupOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CupOp.h"
#include "ZddMgrImpl.h"
#include "CompTbl.h"


BEGIN_NAMESPACE_YM_ZDD

//////////////////////////////////////////////////////////////////////
// クラス CupOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr ZddMgrImpl
CupOp::CupOp(ZddMgrImpl& mgr) :
  mMgr(mgr),
  mCupTable(mgr, "cup_table")
{
}

// @brief デストラクタ
CupOp::~CupOp()
{
}

// @brief \f$\cap\f$演算を行う関数
// @param[in] left, right オペランド
ZddEdge
CupOp::apply(ZddEdge left,
	     ZddEdge right)
{
  // エラー状態のチェック
  if ( left.is_error() || right.is_error() ) {
    // どちらかがエラー
    return ZddEdge::make_error();
  }
  if ( left.is_overflow() || right.is_overflow() ) {
    // どちらかがオーバーフロー
    return ZddEdge::make_overflow();
  }

  return cup_step(left, right);
}

// cup の下請け関数
ZddEdge
CupOp::cup_step(ZddEdge f,
		ZddEdge g)
{
  // 0-element 属性に対するルール
  // f, g のどちらかが 0-element 属性をもっていたら答にも 0-element 属性を持たせる．
  bool zattr = f.zattr() || g.zattr();
  f.normalize();
  g.normalize();

  ZddEdge ans_e;

  // 特別な場合の処理
  // 1: 片方のZDDが0なら答は他方，
  // 2: 同じZDDどうしなら答は自分自身
  if ( g.is_zero() || f == g ) {
    ans_e = f;
  }
  else if ( f.is_zero() ) {
    ans_e = g;
  }
  else {
    // この時点で f,g は終端ではない．

    // 演算結果テーブルが当たりやすくなるように順序を正規化する
    if ( f > g ) {
      ZddEdge tmp = f;
      f = g;
      g = tmp;
    }

    ans_e = mCupTable.get(f, g);
    if ( ans_e.is_error() ) {
      // 演算結果テーブルには登録されていない
      ZddEdge f_0, f_1;
      ZddEdge g_0, g_1;
      ZddVar* var = split(f, g, f_0, f_1, g_0, g_1);
      ZddEdge r_0 = cup_step(f_0, g_0);
      if ( r_0.is_overflow() ) {
	return ZddEdge::make_overflow();
      }
      ZddEdge r_1 = cup_step(f_1, g_1);
      if ( r_1.is_overflow() ) {
	return ZddEdge::make_overflow();
      }
      ans_e = mMgr.new_node(var, r_0, r_1);
      mCupTable.put(f, g, ans_e);
    }
  }
  return ans_e.add_zattr(zattr);
}

END_NAMESPACE_YM_ZDD
