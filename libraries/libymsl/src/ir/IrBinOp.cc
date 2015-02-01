
/// @file IrBinOp.cc
/// @brief IrBinOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrBinOp.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrBinOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] opcode オペコード
// @param[in] src1, src2 オペランド
IrBinOp::IrBinOp(OpCode opcode,
		 IrNode* src1,
		 IrNode* src2) :
  mOpCode(opcode),
  mSrc1(src1),
  mSrc2(src2)
{
}

// @brief デストラクタ
IrBinOp::~IrBinOp()
{
}

// @brief opcode を返す．
OpCode
IrBinOp::opcode() const
{
  return mOpCode;
}

// @brief 第1ソースを返す．
IrNode*
IrBinOp::src1() const
{
  return mSrc1;
}

// @brief 第2ソースを返す．
IrNode*
IrBinOp::src2() const
{
  return mSrc2;
}

END_NAMESPACE_YM_YMSL
