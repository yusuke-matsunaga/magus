
/// @file IrBitAnd.cc
/// @brief IrBitAnd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrBitAnd.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrBitAnd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] src1, src2 オペランド
IrBitAnd::IrBitAnd(IrNode* src1,
		   IrNode* src2) :
  mSrc1(src1),
  mSrc2(src2)
{
}

// @brief デストラクタ
IrBitAnd::~IrBitAnd()
{
}

// @brief opcode を返す．
OpCode
IrBitAnd::opcode() const
{
  return kOpBitAnd;
}

END_NAMESPACE_YM_YMSL
