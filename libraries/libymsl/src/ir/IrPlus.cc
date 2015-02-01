
/// @file IrPlus.cc
/// @brief IrPlus の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrPlus.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrPlus
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] src1, src2 オペランド
IrPlus::IrPlus(IrNode* src1,
	       IrNode* src2) :
  mSrc1(src1),
  mSrc2(src2)
{
}

// @brief デストラクタ
IrPlus::~IrPlus()
{
}

// @brief opcode を返す．
OpCode
IrPlus::opcode() const
{
  return kOpPlus;
}

END_NAMESPACE_YM_YMSL
