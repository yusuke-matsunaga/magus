
/// @file IrTrue.cc
/// @brief IrTrue の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrTrue.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrTrue
//////////////////////////////////////////////////////////////////////


// @brief コンストラクタ
IrTrue::IrTrue()
{
}

// @brief デストラクタ
IrTrue::~IrTrue()
{
}

// @brief 型を返す．
ExprType
IrTrue::expr_type() const
{
  return kTrue;
}

END_NAMESPACE_YM_YMSL
