
/// @file Cof0Op.cc
/// @brief Cof0Op の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "Cof0Op.h"


BEGIN_NAMESPACE_YM_ZDD

//////////////////////////////////////////////////////////////////////
// クラス Cof0Op
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr マネージャ
Cof0Op::Cof0Op(ZddMgrImpl* mgr) :
  ZddUniOp(mgr)
{
}

// @brief デストラクタ
Cof0Op::~Cof0Op()
{
}

// @brief cofactor0 演算を行う関数
// @param[in] left オペランド
// @param[in] level 変数のレベル
ZddEdge
Cof0Op::apply(ZddEdge left,
	      ymuint level)
{
  // エラー状態のチェック
  if ( left.is_invalid() ) {
    return left;
  }

  mLevel = level;

  clear();

  return apply_step(left);
}

// 下請け関数
ZddEdge
Cof0Op::apply_step(ZddEdge f)
{
  // 特別な場合の処理
  // 1：fが0なら答は0，
  // 2: fが1なら答は1
  if ( f.is_zero() ) {
    return ZddEdge::make_zero();
  }
  if ( f.is_one() ) {
    return  ZddEdge::make_one();
  }

  // この時点で f は終端ではない．
  ZddNode* f_node = f.get_node();
  ymuint f_level = f_node->level();
  if ( f_level == mLevel ) {
    ZddEdge result = f_node->edge0();
    result.add_zattr(f.zattr());
    return result;
  }
  if ( f_level > mLevel ) {
    return f;
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
