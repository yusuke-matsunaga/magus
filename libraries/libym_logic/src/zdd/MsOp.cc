
/// @file MsOp.cc
/// @brief MsOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "MsOp.h"
#include "ZddBinOp.h"


BEGIN_NAMESPACE_YM_ZDD

//////////////////////////////////////////////////////////////////////
// クラス MsOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr マネージャ
// @param[in] diff_op 集合差演算オブジェクト
MsOp::MsOp(ZddMgrImpl* mgr,
	   ZddBinOp* diff_op) :
  ZddOp(mgr),
  mCompTbl(mgr, "minimum_set_table"),
  mDiffOp(diff_op)
{
}

// @brief デストラクタ
MsOp::~MsOp()
{
}

// @brief minimum_set 演算を行う関数
// @param[in] left オペランド
ZddEdge
MsOp::apply(ZddEdge left)
{
  // エラー状態のチェック
  if ( left.is_invalid() ) {
    return left;
  }
  mCompTbl.clear();
  return apply_step(left);
}

// @brief 次の GC で回収されるノードに関連した情報を削除する．
void
MsOp::sweep()
{
  if ( mCompTbl.used_num() > 0 ) {
    mCompTbl.sweep();
  }
}

// apply_step の下請け関数
ZddEdge
MsOp::apply_step(ZddEdge f)
{
  if ( f.is_const() ) {
    return f;
  }

  // この時点で f は終端ではない．
  ZddEdge result = mCompTbl.get(f);
  if ( result.is_error() ) {
    // 演算結果テーブルには登録されていない
    ZddNode* f_node = f.get_node();
    ymuint f_level = f_node->level();
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
    r_1 = mDiffOp->apply(r_1, r_0);
    if ( r_1.is_invalid() ) {
      return r_1;
    }
    result = new_node(f_level, r_0, r_1);
    mCompTbl.put(f, result);
  }
  return result;
}

END_NAMESPACE_YM_ZDD
