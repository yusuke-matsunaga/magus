
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

// @brief opcode を返す．
OpCode
IrFalse::opcode() const
{
  return kOpFalse;
}

END_NAMESPACE_YM_YMSL
