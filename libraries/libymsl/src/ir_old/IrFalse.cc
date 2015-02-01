
/// @file IrFalse.cc
/// @brief IrFalse の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrFalse.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrFalse
//////////////////////////////////////////////////////////////////////


// @brief コンストラクタ
IrFalse::IrFalse()
{
}

// @brief デストラクタ
IrFalse::~IrFalse()
{
}

// @brief 型を返す．
ExprType
IrFalse::expr_type() const
{
  return kFalse;
}

END_NAMESPACE_YM_YMSL
