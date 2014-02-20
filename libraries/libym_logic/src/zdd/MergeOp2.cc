
/// @file MergeOp2.cc
/// @brief MergeOp2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "MergeOp2.h"
#include "NeOp.h"
#include "MsOp.h"


BEGIN_NAMESPACE_YM_ZDD

//////////////////////////////////////////////////////////////////////
// クラス MergeOp2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr マネージャ
// @param[in] cup_op union 演算オブジェクト
// @param[in] ne_op n-elemnt 演算オブジェクト
// @param[in] ms_op minimum set 演算オブジェクト
MergeOp2::MergeOp2(ZddMgrImpl* mgr,
		   ZddBinOp* cup_op,
		   NeOp* ne_op,
		   MsOp* ms_op) :
  ZddBinOpBase(mgr),
  mCompTbl(mgr, "merge_table"),
  mCupOp(cup_op),
  mNeOp(ne_op),
  mMsOp(ms_op)
{
}

// @brief デストラクタ
MergeOp2::~MergeOp2()
{
}

// @brief 次の GC で回収されるノードに関連した情報を削除する．
void
MergeOp2::sweep()
{
  if ( mCompTbl.used_num() > 0 ) {
    mCompTbl.sweep();
  }
}

// @brief マージ演算を行う関数
// @param[in] left, right オペランド
// @param[in] limit 要素数の上限値
ZddEdge
MergeOp2::apply(ZddEdge left,
		ZddEdge right,
		ymuint limit)
{
  // エラー状態のチェック
  if ( left.is_invalid() ) {
    return left;
  }
  if ( right.is_invalid() ) {
    return right;
  }

  return apply_step(left, right, limit);
}

// cap_op の下請け関数
ZddEdge
MergeOp2::apply_step(ZddEdge f,
		     ZddEdge g,
		     ymuint limit)
{
  // 特別な場合の処理
  // 1: 片方のZDDが0なら答は0，
  // 2: 片方のZDDが1なら答は他方
  // 3: 同じZDDどうしなら答は自分自身
  if ( f.is_zero() || g.is_zero() ) {
    return ZddEdge::make_zero();
  }
  if ( f.is_one() ) {
    return mNeOp->apply(g, limit);
  }
  if ( g.is_one() || f == g ) {
    return mNeOp->apply(f, limit);
  }

  // この時点で f,g は終端ではない．

  // 演算結果テーブルが当たりやすくなるように順序を正規化する
  if ( f > g ) {
    ZddEdge tmp = f;
    f = g;
    g = tmp;
  }

  ZddEdge ans_e = get(f, g, limit);
  if ( ans_e.is_error() ) {
    // 演算結果テーブルには登録されていない
    ZddEdge f_0, f_1;
    ZddEdge g_0, g_1;
    ymuint level = split(f, g, f_0, f_1, g_0, g_1);
    ZddEdge r_0 = apply_step(f_0, g_0, limit);
    if ( r_0.is_invalid() ) {
      return r_0;
    }
    ZddEdge r_1;
    if ( limit > 0 ) {
      ymuint limit1 = limit - 1;
      ZddEdge r_01 = apply_step(f_0, g_1, limit1);
      if ( r_01.is_invalid() ) {
	return r_01;
      }
      ZddEdge r_10 = apply_step(f_1, g_0, limit1);
      if ( r_10.is_invalid() ) {
	return r_10;
      }
      ZddEdge r_11 = apply_step(f_1, g_1, limit1);
      if ( r_11.is_invalid() ) {
	return r_11;
      }
      ZddEdge tmp = mCupOp->apply(r_01, r_10);
      if ( tmp.is_invalid() ) {
	return tmp;
      }
      r_1 = mCupOp->apply(tmp, r_11);
      if ( r_1.is_invalid() ) {
	return r_1;
      }

    }
    else {
      r_1 = ZddEdge::make_zero();
    }
    ans_e = new_node(level, r_0, r_1);
    put(f, g, limit, ans_e);
  }

  return ans_e;
}

END_NAMESPACE_YM_ZDD
