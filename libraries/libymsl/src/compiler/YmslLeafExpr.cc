
/// @file YmslLeafExpr.cc
/// @brief YmslLeafExpr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmslLeafExpr.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス YmslLeafExpr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] leaf 終端
YmslLeafExpr::YmslLeafExpr(YmslLeaf* leaf) :
  mLeaf(leaf)
{
}

// @brief デストラクタ
YmslLeafExpr::~YmslLeafExpr()
{
}

// @brief 種類を返す．
ExprType
YmslLeafExpr::expr_type() const
{
  return kLeafExpr;
}

// @brief 終端式を返す．
//
// kLeafExpr の時のみ有効
YmslLeaf*
YmslLeafExpr::leaf() const
{
  return mLeaf;
}

END_NAMESPACE_YM_YMSL
