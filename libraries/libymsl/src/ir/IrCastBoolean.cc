
/// @file IrCastBoolean.cc
/// @brief IrCastBoolean の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrCastBoolean.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrCastBoolean
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] src1 オペランド
IrCastBoolean::IrCastBoolean(IrNode* src1) :
  mSrc1(src1)
{
}

// @brief デストラクタ
IrCastBoolean::~IrCastBoolean()
{
}

// @brief opcode を返す．
OpCode
IrCastBoolean::opcode() const
{
  return kOpCastBoolean;
}

END_NAMESPACE_YM_YMSL
