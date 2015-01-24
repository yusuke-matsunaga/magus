
/// @file AstLeafExpr.cc
/// @brief AstLeafExpr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "AstLeafExpr.h"
#include "AstLeaf.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstLeafExpr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
AstLeafExpr::AstLeafExpr(AstLeaf* leaf) :
  AstExpr(leaf->file_region()),
  mLeaf(leaf)
{
}

// @breif デストラクタ
AstLeafExpr::~AstLeafExpr()
{
}

// @brief 種類を返す．
ExprType
AstLeafExpr::expr_type() const
{
  return kLeafExpr;
}

// @brief 終端を返す．
//
// kLeafExpr のみ有効
const AstLeaf*
AstLeafExpr::leaf() const
{
  return mLeaf;
}

END_NAMESPACE_YM_YMSL
