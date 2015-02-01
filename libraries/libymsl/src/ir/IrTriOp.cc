
/// @file IrTriOp.cc
/// @brief IrTriOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrTriOp.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrTriOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] opcode オペコード
// @param[in] src1, src2, src3 オペランド
IrTriOp::IrTriOp(OpCode opcode,
		 IrNode* src1,
		 IrNode* src2,
		 IrNode* src3) :
  mOpCode(opcode),
  mSrc1(src1),
  mSrc2(src2),
  mSrc3(src3)
{
}

// @brief デストラクタ
IrTriOp::~IrTriOp()
{
}

// @brief opcode を返す．
OpCode
IrTriOp::opcode() const
{
  return mOpCode;
}

// @brief 第1ソースを返す．
IrNode*
IrTriOp::src1() const
{
  return mSrc1;
}

// @brief 第2ソースを返す．
IrNode*
IrTriOp::src2() const
{
  return mSrc2;
}

// @brief 第3ソースを返す．
IrNode*
IrTriOp::src3() const
{
  return mSrc3;
}

END_NAMESPACE_YM_YMSL
