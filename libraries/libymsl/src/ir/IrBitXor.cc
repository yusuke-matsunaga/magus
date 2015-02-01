
/// @file IrBitXor.cc
/// @brief IrBitXor の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrBitXor.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrBitXor
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] src1, src2 オペランド
IrBitXor::IrBitXor(IrNode* src1,
		   IrNode* src2) :
  mSrc1(src1),
  mSrc2(src2)
{
}

// @brief デストラクタ
IrBitXor::~IrBitXor()
{
}

// @brief opcode を返す．
OpCode
IrBitXor::opcode() const
{
  return kOpBitXor;
}

END_NAMESPACE_YM_YMSL
