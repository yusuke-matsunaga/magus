
/// @file IrBitNeg.cc
/// @brief IrBitNeg の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrBitNeg.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrBitNeg
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] src1 オペランド
IrBitNeg::IrBitNeg(IrNode* src1) :
  mSrc1(src1)
{
}

// @brief デストラクタ
IrBitNeg::~IrBitNeg()
{
}

// @brief opcode を返す．
OpCode
IrBitNeg::opcode() const
{
  return kOpBitNeg;
}

END_NAMESPACE_YM_YMSL
