
/// @file IrCastFloat.cc
/// @brief IrCastFloat の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrCastFloat.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrCastFloat
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] src1 オペランド
IrCastFloat::IrCastFloat(IrNode* src1) :
  mSrc1(src1)
{
}

// @brief デストラクタ
IrCastFloat::~IrCastFloat()
{
}

// @brief opcode を返す．
OpCode
IrCastFloat::opcode() const
{
  return kOpCastFloat;
}

END_NAMESPACE_YM_YMSL
