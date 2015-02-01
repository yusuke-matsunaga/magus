
/// @file IrLoad.cc
/// @brief IrLoad の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrLoad.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrLoad
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] src1, src2 オペランド
IrLoad::IrLoad(IrNode* src1,
	       IrNode* src2) :
  mSrc1(src1),
  mSrc2(src2)
{
}

// @brief デストラクタ
IrLoad::~IrLoad()
{
}

// @brief opcode を返す．
OpCode
IrLoad::opcode() const
{
  return kOpLoad;
}

END_NAMESPACE_YM_YMSL
