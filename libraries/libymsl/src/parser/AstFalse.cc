
/// @file AstFalse.cc
/// @brief AstFalse の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "AstFalse.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstFalse
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] loc ファイル位置
AstFalse::AstFalse(const FileRegion& loc) :
  AstExpr(loc)
{
}

// @brief デストラクタ
AstFalse::~AstFalse()
{
}

// @brief 種類を返す．
ExprType
AstFalse::expr_type() const
{
  return kFalse;
}

END_NAMESPACE_YM_YMSL
