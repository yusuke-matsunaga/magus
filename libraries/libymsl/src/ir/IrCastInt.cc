
/// @file IrCastInt.cc
/// @brief IrCastInt の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrCastInt.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrCastInt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] src1 オペランド
IrCastInt::IrCastInt(IrNode* src1) :
  mSrc1(src1)
{
}

// @brief デストラクタ
IrCastInt::~IrCastInt()
{
}

// @brief opcode を返す．
OpCode
IrCastInt::opcode() const
{
  return kOpCastInt;
}

END_NAMESPACE_YM_YMSL
