
/// @file SwapOp.cc
/// @brief SwapOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "SwapOp.h"
#include "ZddMgrImpl.h"


BEGIN_NAMESPACE_YM_ZDD

//////////////////////////////////////////////////////////////////////
// クラス SwapOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr ZddMgrImpl
SwapOp::SwapOp(ZddMgrImpl& mgr) :
  mMgr(mgr)
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

  mLevel = mMgr.level(var);
  mCompTbl.clear();

  return swap_step(left);
}

// swap_op の下請け関数
ZddEdge
SwapOp::swap_step(ZddEdge f)
{
  // 特別な場合の処理
  // 1：fが0なら答は0，
  // 2: fが1なら答は mVar の1枝に base を持つノード
  if ( f.is_zero() ) {
    return ZddEdge::make_zero();
  }
  else if ( f.is_one() ) {
    return  mMgr.new_node(mLevel, ZddEdge::make_zero(), ZddEdge::make_one());
  }
  else {
    // この時点で f は終端ではない．

    hash_map<ZddEdge, ZddEdge>::iterator p = mCompTbl.find(f);
    if ( p != mCompTbl.end() ) {
      // 既に計算済みだった．
      return p->second;
    }
    else {
      // 演算結果テーブルには登録されていない
      ZddNode* f_vp = f.get_node();
      ymuint f_level = f_vp->level();
      ZddEdge result;
      if ( f_level < mLevel ) {
	ZddEdge f_0 = f_vp->edge0();
	f_0.add_zattr(f.zattr());
	ZddEdge r_0 = swap_step(f_0);
	if ( r_0.is_overflow() ) {
	  return ZddEdge::make_overflow();
	}
	ZddEdge f_1 = f_vp->edge1();
	ZddEdge r_1 = swap_step(f_1);
	if ( r_1.is_overflow() ) {
	  return ZddEdge::make_overflow();
	}
	result = mMgr.new_node(f_level, r_0, r_1);
      }
      else if ( f_level == mLevel ) {
	ZddEdge f_0 = f_vp->edge0();
	f_0.add_zattr(f.zattr());
	ZddEdge f_1 = f_vp->edge1();
	result = mMgr.new_node(f_level, f_1, f_0);
      }
      else {
	result = mMgr.new_node(mLevel, ZddEdge::make_zero(), f);
      }
      mCompTbl.insert(make_pair(f, result));
      return result;
    }
  }
}

END_NAMESPACE_YM_ZDD
