
/// @file IrLogNot.cc
/// @brief IrLogNot の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrLogNot.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrLogNot
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] src1 オペランド
IrLogNot::IrLogNot(IrNode* src1) :
  mSrc1(src1)
{
}

// @brief デストラクタ
IrLogNot::~IrLogNot()
{
}

// @brief opcode を返す．
OpCode
IrLogNot::opcode() const
{
  return kOpLogNot;
}

END_NAMESPACE_YM_YMSL
