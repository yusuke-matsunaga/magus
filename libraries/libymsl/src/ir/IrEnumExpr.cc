
/// @file IrEnumExpr.cc
/// @brief IrEnumExpr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrEnumExpr.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrEnumExpr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] enum_type enum 型
IrEnumExpr::IrEnumExpr(const Type* enum_type) :
  mEnumType(enum_type)
{
}

// @brief デストラクタ
IrEnumExpr::~IrEnumExpr()
{
}

// @brief 種類を返す．
ExprType
IrEnumExpr::expr_type() const
{
  return kEnumExpr;
}

// @brief enum 型を返す．
//
// kEnumExpr のみ有効
const Type*
IrEnumExpr::enum_type() const
{
  return mEnumType;
}

END_NAMESPACE_YM_YMSL
