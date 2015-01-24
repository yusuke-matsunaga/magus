
/// @file AstTrue.cc
/// @brief AstTrue の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "AstTrue.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstTrue
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] loc ファイル位置
AstTrue::AstTrue(const FileRegion& loc) :
  AstExpr(loc)
{
}

// @brief デストラクタ
AstTrue::~AstTrue()
{
}

// @brief 種類を返す．
ExprType
AstTrue::expr_type() const
{
  return kTrue;
}

END_NAMESPACE_YM_YMSL
