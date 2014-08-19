
/// @file NeOp.cc
/// @brief NeOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "NeOp.h"


BEGIN_NAMESPACE_YM_ZDD

//////////////////////////////////////////////////////////////////////
// クラス NeOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr マネージャ
NeOp::NeOp(ZddMgrImpl* mgr) :
  ZddOp(mgr),
  mCompTbl(mgr, "n_element_table")
{
}

// @brief デストラクタ
NeOp::~NeOp()
{
}

// @brief n_element 演算を行う関数
// @param[in] left オペランド
// @param[in] limit 要素数の制限値
ZddEdge
NeOp::apply(ZddEdge left,
	    ymuint limit)
{
  // エラー状態のチェック
  if ( left.is_invalid() ) {
    return left;
  }
  mCompTbl.clear();
  return apply_step(left, limit);
}

// @brief 次の GC で回収されるノードに関連した情報を削除する．
void
NeOp::sweep()
{
  if ( mCompTbl.used_num() > 0 ) {
    mCompTbl.sweep();
  }
}

// apply_step の下請け関数
ZddEdge
NeOp::apply_step(ZddEdge f,
		 ymuint n)
{
  if ( f.is_const() ) {
    return f;
  }

  // この時点で f は終端ではない．
  ZddEdge result = mCompTbl.get(f, n);
  if ( result.is_error() ) {
    // 演算結果テーブルには登録されていない
    ZddNode* f_node = f.get_node();
    ymuint f_level = f_node->level();
    ZddEdge f_0 = f_node->edge0();
    f_0.add_zattr(f.zattr());
    ZddEdge r_0 = apply_step(f_0, n);
    if ( r_0.is_invalid() ) {
      return r_0;
    }

    if ( n > 0 ) {
      ZddEdge f_1 = f_node->edge1();
      ZddEdge r_1 = apply_step(f_1, n - 1);
      if ( r_1.is_invalid() ) {
	return r_1;
      }
      result = new_node(f_level, r_0, r_1);
    }
    else {
      result = r_0;
    }
    mCompTbl.put(f, n, result);
  }
  return result;
}

END_NAMESPACE_YM_ZDD
