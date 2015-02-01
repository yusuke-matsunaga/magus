
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

// @brief opcode を返す．
OpCode
IrTrue::opcode() const
{
  return kOpTrue;
}

END_NAMESPACE_YM_YMSL
