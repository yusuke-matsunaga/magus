
/// @file IrAssign.cc
/// @brief IrAssign の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrAssign.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrAssign
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] lhs 左辺式
// @param[in] rhs 右辺式
IrAssign::IrAssign(IrNode* lhs,
		   IrNode* rhs) :
  IrNode(kOpAssign, NULL),
  mLhsExpr(lhs),
  mRhsExpr(rhs)
{
}

// @brief デストラクタ
IrAssign::~IrAssign()
{
}

// @brief 左辺式を返す．
//
// kOpAssign のみ有効
IrNode*
IrAssign::lhs_expr() const
{
  return mLhsExpr;
}

// @brief 右辺式を返す．
//
// kOpAssign のみ有効
IrNode*
IrAssign::rhs_expr() const
{
  return mRhsExpr;
}

END_NAMESPACE_YM_YMSL
