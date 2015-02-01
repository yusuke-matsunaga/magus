
/// @file IrUniMinus.cc
/// @brief IrUniMinus の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrUniMinus.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrUniMinus
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] src1 オペランド
IrUniMinus::IrUniMinus(IrNode* src1) :
  mSrc1(src1)
{
}

// @brief デストラクタ
IrUniMinus::~IrUniMinus()
{
}

// @brief opcode を返す．
OpCode
IrUniMinus::opcode() const
{
  return kOpUniMinus;
}

END_NAMESPACE_YM_YMSL
