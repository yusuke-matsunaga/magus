
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
// @param[in] type 型
// @param[in] src1, src2 オペランド
IrBinOp::IrBinOp(OpCode opcode,
		 const Type* type,
		 IrNode* src1,
		 IrNode* src2) :
  IrNode(opcode, type),
  mSrc1(src1),
  mSrc2(src2)
{
}

// @brief デストラクタ
IrBinOp::~IrBinOp()
{
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
