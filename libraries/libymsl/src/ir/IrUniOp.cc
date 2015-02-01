
/// @file IrUniOp.cc
/// @brief IrUniOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrUniOp.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrUniOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] opcode オペコード
// @param[in] src1 オペランド
IrUniOp::IrUniOp(OpCode opcode,
		 IrNode* src1) :
  mOpCode(opcode),
  mSrc1(src1)
{
}

// @brief デストラクタ
IrUniOp::~IrUniOp()
{
}

// @brief opcode を返す．
OpCode
IrUniOp::opcode() const
{
  return mOpCode;
}

// @brief 第1ソースを返す．
IrNode*
IrUniOp::src1() const
{
  return mSrc1;
}

END_NAMESPACE_YM_YMSL
