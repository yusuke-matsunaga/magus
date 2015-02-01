
/// @file IrBitOr.cc
/// @brief IrBitOr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrBitOr.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrBitOr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] src1, src2 オペランド
IrBitOr::IrBitOr(IrNode* src1,
		 IrNode* src2) :
  mSrc1(src1),
  mSrc2(src2)
{
}

// @brief デストラクタ
IrBitOr::~IrBitOr()
{
}

// @brief opcode を返す．
OpCode
IrBitOr::opcode() const
{
  return kOpBitOr;
}

END_NAMESPACE_YM_YMSL
