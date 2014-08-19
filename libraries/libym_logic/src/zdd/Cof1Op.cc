
/// @file Cof1Op.cc
/// @brief Cof1Op の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "Cof1Op.h"


BEGIN_NAMESPACE_YM_ZDD

//////////////////////////////////////////////////////////////////////
// クラス Cof1Op
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr マネージャ
Cof1Op::Cof1Op(ZddMgrImpl* mgr) :
  ZddUniOp(mgr)
{
}

// @brief デストラクタ
Cof1Op::~Cof1Op()
{
}

// @brief cofactor1 演算を行う関数
// @param[in] left オペランド
// @param[in] level 変数のレベル
ZddEdge
Cof1Op::apply(ZddEdge left,
	      ymuint level)
{
  // エラー状態のチェック
  if ( left.is_error() ) {
    return left;
  }

  mLevel = level;

  clear();
  return apply_step(left);
}

// 下請け関数
ZddEdge
Cof1Op::apply_step(ZddEdge f)
{
  // 特別な場合の処理
  // 1：fが0なら答は0，
  // 2: fが1なら答は0
  if ( f.is_const() ) {
    return ZddEdge::make_zero();
  }

  // この時点で f は終端ではない．
  ZddNode* f_node = f.get_node();
  ymuint f_level = f_node->level();
  if ( f_level == mLevel ) {
    return f_node->edge1();
  }
  if ( f_level > mLevel ) {
    return ZddEdge::make_zero();
  }

  ZddEdge result = get(f);
  if ( result.is_error() ) {
    // 演算結果テーブルには登録されていない
    ZddEdge f_0 = f_node->edge0();
    f_0.add_zattr(f.zattr());
    ZddEdge r_0 = apply_step(f_0);
    if ( r_0.is_invalid() ) {
      return r_0;
    }
    ZddEdge f_1 = f_node->edge1();
    ZddEdge r_1 = apply_step(f_1);
    if ( r_1.is_invalid() ) {
      return r_1;
    }
    result = new_node(f_level, r_0, r_1);
    put(f, result);
  }
  return result;
}

END_NAMESPACE_YM_ZDD
