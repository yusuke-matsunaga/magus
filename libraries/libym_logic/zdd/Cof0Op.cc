
/// @file Cof0Op.cc
/// @brief Cof0Op の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "Cof0Op.h"
#include "ZddMgrImpl.h"


BEGIN_NAMESPACE_YM_ZDD

//////////////////////////////////////////////////////////////////////
// クラス Cof0Op
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr ZddMgrImpl
Cof0Op::Cof0Op(ZddMgrImpl& mgr) :
  mMgr(mgr)
{
}

// @brief デストラクタ
Cof0Op::~Cof0Op()
{
}

// @brief cofactor0 演算を行う関数
// @param[in] left オペランド
// @param[in] var 変数番号
ZddEdge
Cof0Op::apply(ZddEdge left,
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

  mVar = mMgr.var_of(var);
  mLevel = mVar->level();
  mCompTbl.clear();

  return cof0_step(left);
}

// 下請け関数
ZddEdge
Cof0Op::cof0_step(ZddEdge f)
{
  // 特別な場合の処理
  // 1：fが0なら答は0，
  // 2: fが1なら答は1
  if ( f.is_zero() ) {
    return ZddEdge::make_zero();
  }
  else if ( f.is_one() ) {
    return  ZddEdge::make_one();
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
      ZddVar* f_var = f_vp->var();
      ymuint f_level = f_var->level();
      ZddEdge result;
      if ( f_level < mLevel ) {
	ZddEdge f_0 = f_vp->edge0();
	f_0.add_zattr(f.zattr());
	ZddEdge r_0 = cof0_step(f_0);
	if ( r_0.is_overflow() ) {
	  return ZddEdge::make_overflow();
	}
	ZddEdge f_1 = f_vp->edge1();
	ZddEdge r_1 = cof0_step(f_1);
	if ( r_1.is_overflow() ) {
	  return ZddEdge::make_overflow();
	}
	result = mMgr.new_node(f_var, r_0, r_1);
      }
      else if ( f_level == mLevel ) {
	result = f_vp->edge0();
	result.add_zattr(f.zattr());
      }
      else {
	result = f;
      }
      mCompTbl.insert(make_pair(f, result));
      return result;
    }
  }
}

END_NAMESPACE_YM_ZDD
