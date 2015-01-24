
/// @file YmslExpr.cc
/// @brief YmslExpr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmslExpr.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス YmslExpr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
YmslExpr::YmslExpr()
{
}

// @brief デストラクタ
YmslExpr::~YmslExpr()
{
}

// @brief 終端式を返す．
//
// kLeafExpr の時のみ有効
YmslLeaf*
YmslExpr::leaf() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief オペランドの数を返す．
//
// 演算子の時のみ有効
ymuint
YmslExpr::operand_num() const
{
  return 0;
}

// @brief オペランドを返す．
// @param[in] pos 位置 ( 0 <= pos < operand_num() )
//
// 演算子の時のみ有効
YmslExpr*
YmslExpr::operand(ymuint pos) const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

END_NAMESPACE_YM_YMSL
