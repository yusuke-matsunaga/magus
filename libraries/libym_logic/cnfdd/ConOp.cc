
/// @file CapOp.cc
/// @brief CapOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CapOp.h"
#include "ZddMgrImpl.h"
#include "CompTbl.h"


BEGIN_NAMESPACE_YM_CNFDD

//////////////////////////////////////////////////////////////////////
// クラス CapOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr ZddMgrImpl
CapOp::CapOp(ZddMgrImpl& mgr) :
  mMgr(mgr),
  mCapTable(mgr, "cap_table")
{
}

// @brief デストラクタ
CapOp::~CapOp()
{
}

// @brief \f$\cap\f$演算を行う関数
// @param[in] left, right オペランド
ZddEdge
CapOp::apply(ZddEdge left,
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

  return cap_step(left, right);
}

// cap_op の下請け関数
ZddEdge
CapOp::cap_step(ZddEdge f,
		ZddEdge g)
{
  // 0-element 属性に対するルール
  // f, g ともに 0-element 属性をもっていたら答にも 0-element 属性を持たせる．
  bool zattr = f.zattr() && g.zattr();
  f.normalize();
  g.normalize();

  ZddEdge ans_e;

  // 特別な場合の処理
  // 1: 片方のCNFDDが0なら答は0，
  // 2: 同じCNFDDどうしなら答は自分自身
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

    ans_e = mCapTable.get(f, g);
    if ( ans_e.is_error() ) {
      // 演算結果テーブルには登録されていない
      ZddEdge f_0, f_1;
      ZddEdge g_0, g_1;
      ymuint level = split(f, g, f_0, f_1, g_0, g_1);
      ZddEdge r_0 = cap_step(f_0, g_0);
      if ( r_0.is_overflow() ) {
	return ZddEdge::make_overflow();
      }
      ZddEdge r_1 = cap_step(f_1, g_1);
      if ( r_1.is_overflow() ) {
	return ZddEdge::make_overflow();
      }
      ans_e = mMgr.new_node(level, r_0, r_1);
      mCapTable.put(f, g, ans_e);
    }
  }
  return ans_e.add_zattr(zattr);
}

END_NAMESPACE_YM_CNFDD
